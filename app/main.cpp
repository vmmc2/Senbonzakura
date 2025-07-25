#include <iostream>
#include <string>
#include <vector>

#include <CLI/CLI.hpp>

#include "senbonzakura/diagnostic_reporter.hpp"
#include "senbonzakura/file_scanner.hpp"
#include "senbonzakura/file_writer.hpp"
#include "senbonzakura/lexer.hpp"

int main(int argc, const char **argv) {
  std::string output_directory_path;
  std::vector<std::string> eta_programs_filepaths;

  CLI::App senbonzakura_compiler_app{
      "This is the Senbonzakura Compiler. A compiler written using C++ 20 for "
      "the Eta programming language."};

  senbonzakura_compiler_app
      .add_option("-l,--lex", eta_programs_filepaths,
                  "This command is responsible for performing the lexing stage "
                  "of the compilation process and output the generated "
                  "sequence of tokens. Requires the path to at least one "
                  "'.eta' file. Can deal with multiples, though.")
      ->required()
      ->delimiter(' ')
      ->expected(1, -1);

  auto d_option = senbonzakura_compiler_app
                      .add_option("-D,--destiny_directory",
                                  output_directory_path,
                                  "The directory where the generated "
                                  "diagnostic files will be placed.")
                      ->needs("--lex");

  try {
    CLI11_PARSE(senbonzakura_compiler_app, argc, argv);
  } catch (const CLI::ParseError &parse_error) {
    std::cout
        << std::format(
               "[Fatal]: An internal error has occurred while trying to parse "
               "the arguments provided to the Senbonzakura CLI App.")
        << std::endl;
    return senbonzakura_compiler_app.exit(parse_error);
  }

  FileWriter file_writer{eta_programs_filepaths, output_directory_path};

  for (const std::string &current_eta_filepath : eta_programs_filepaths) {
    DiagnosticReporter diagnostic_reporter;
    FileScanner file_scanner{current_eta_filepath, diagnostic_reporter};

    file_scanner.ScanFile();
    const std::u32string &file_content =
        file_scanner.GetFileContentCodepoints();

    Lexer lexer{current_eta_filepath, file_content, diagnostic_reporter};
    const std::vector<Token> tokens = lexer.LexTokens();

    file_writer.WriteLexerOutput(current_eta_filepath, tokens);

    if (diagnostic_reporter.HasFatalErrors()) {
      diagnostic_reporter.OutputSystemErrors();
      diagnostic_reporter.OutputCompilerErrors();
      return 0;
    }
  }

  return 0;
}