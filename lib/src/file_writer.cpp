#include "senbonzakura/file_writer.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>

FileWriter::FileWriter(const std::vector<std::string> &eta_programs_filepaths,
                       const std::string output_directory)
    : eta_programs_filepaths_{eta_programs_filepaths},
      output_directory_{std::move(output_directory)} {}

const std::vector<std::string> &FileWriter::GetProgramFilepaths() const {
  return eta_programs_filepaths_;
}

const std::string &FileWriter::GetOutputDirectory() const {
  return output_directory_;
}

void FileWriter::WriteLexerOutput(const std::string &current_eta_filepath_str,
                                  std::vector<Token> tokens) const {
  std::filesystem::path current_eta_file_path{current_eta_filepath_str};
  std::filesystem::path output_directory_path{output_directory_};

  if (std::filesystem::exists(output_directory_path) &&
      std::filesystem::is_directory(output_directory_path)) {
    std::ofstream output_file_stream{
        std::format("{}/{}.lexed", output_directory_,
                    current_eta_file_path.filename().string())};
    for (const Token &token : tokens) {
      if (token.token_type_ != TokenType::kFileEnd) {
        output_file_stream << token << std::endl;
      }
    }
  } else {
    throw std::runtime_error{std::format(
        "The path '' does not exists or does not represent a directory.",
        output_directory_)};
  }

  return;
}
