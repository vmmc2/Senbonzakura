#include "senbonzakura/lexer.hpp"

#include <any>
#include <format>
#include <optional>
#include <string>

#include <utf8.h>

#include "senbonzakura/diagnostic_reporter.hpp"
#include "senbonzakura/token.hpp"

Lexer::Lexer(const std::string &file_path,
             const std::u32string &source_code_codepoints,
             DiagnosticReporter &diagnostic_reporter)
    : file_path_(file_path), source_code_codepoints_(source_code_codepoints),
      diagnostic_reporter_(diagnostic_reporter) {}

void Lexer::AddToken(TokenType token_type) {
  AddToken(token_type, nullptr);

  return;
}

void Lexer::AddToken(TokenType token_type, std::any value) {
  std::u32string u32_lexeme{
      source_code_codepoints_.substr(start_, current_ - start_)};
  std::string lexeme = U32StringToUtf8(u32_lexeme);
  tokens_.emplace_back(Token(line_, column_, token_type, value, lexeme));

  return;
}

char32_t Lexer::Advance() {
  current_++;

  return source_code_codepoints_[current_ - 1];
}

void Lexer::Character() { return; }

// TODO: Verify if it makes sense to add the line: column_delta_ = current_ -
// start_; at the end of this method.
void Lexer::Comment() {
  while (!IsAtEnd() && Peek(0) != '\n') {
    Advance();
  }

  return;
}

void Lexer::Identifier() {
  while (IsAlphaNumeric(Peek(0))) {
    Advance();
  }

  std::u32string u32_lexeme{
      source_code_codepoints_.substr(start_, current_ - start_)};
  std::string lexeme = U32StringToUtf8(u32_lexeme);
  TokenType token_type = TokenType::kIdentifier;

  if (keywords_to_tokentypes_.contains(lexeme)) {
    token_type = keywords_to_tokentypes_[lexeme];
  }

  AddToken(token_type);

  column_delta_ = current_ - start_;

  return;
}

void Lexer::Integer() {
  while (IsDigit(Peek(0))) {
    Advance();
  }

  std::u32string u32_num_str{
      source_code_codepoints_.substr(start_, current_ - start_)};
  std::string num_str = U32StringToUtf8(u32_num_str);
  std::any value = std::stoi(num_str);

  AddToken(TokenType::kInteger, value);

  column_delta_ = current_ - start_;

  return;
}

bool Lexer::IsAlpha(char32_t c) {
  return (c >= U'a' && c <= U'z') || (c >= U'A' && c <= U'Z');
}

bool Lexer::IsAlphaNumeric(char32_t c) {
  return (IsAlpha(c) || IsDigit(c) || c == U'_' || c == U'\'');
}

bool Lexer::IsAtEnd() { return current_ >= source_code_codepoints_.length(); }

bool Lexer::IsDigit(char32_t c) { return (c >= U'0' && c <= U'9'); }

bool Lexer::IsHexDigit(char32_t c) {
  return (c >= U'0' && c <= U'9') || (c >= U'a' && c <= U'f') ||
         (c >= U'A' && c <= U'F');
}

// TODO: Take a look at the error message at the end of this method and fix it.
void Lexer::LexToken() {
  char32_t current_char = Advance();
  column_delta_ = 1;

  switch (current_char) {
  case (U'('):
    AddToken(TokenType::kLeftParen);
    break;
  case (U')'):
    AddToken(TokenType::kRightParen);
    break;
  case (U'{'):
    AddToken(TokenType::kLeftBrace);
    break;
  case (U'}'):
    AddToken(TokenType::kRightBrace);
    break;
  case (U'['):
    AddToken(TokenType::kLeftBracket);
    break;
  case (U']'):
    AddToken(TokenType::kRightBracket);
    break;
  case (U','):
    AddToken(TokenType::kComma);
    break;
  case (U':'):
    AddToken(TokenType::kColon);
    break;
  case (U';'):
    AddToken(TokenType::kSemiColon);
    break;
  case (U'+'):
    AddToken(TokenType::kPlus);
    break;
  case (U'-'):
    AddToken(TokenType::kMinus);
    break;
  case (U'%'):
    AddToken(TokenType::kRemainder);
    break;
  case (U'*'):
    if (Peek(0) == U'>' && Peek(1) == U'>') {
      Advance();
      Advance();
      AddToken(TokenType::kHighMul);
      column_delta_ = current_ - start_;
    } else {
      AddToken(TokenType::kStar);
    }
    break;
  case (U'/'):
    if (Match(U'/')) {
      Comment();
    } else {
      AddToken(TokenType::kSlash);
    }
    break;
  case (U'!'):
    if (Peek(0) == U'=') {
      Advance();
      AddToken(TokenType::kBangEqual);
      column_delta_ = 2;
    } else {
      AddToken(TokenType::kBang);
    }
    break;
  case (U'='):
    if (Peek(0) == U'=') {
      Advance();
      AddToken(TokenType::kEqualEqual);
      column_delta_ = 2;
    } else {
      AddToken(TokenType::kEqual);
    }
    break;
  case (U'>'):
    if (Peek(0) == U'=') {
      Advance();
      AddToken(TokenType::kGreaterEqual);
      column_delta_ = 2;
    } else {
      AddToken(TokenType::kGreater);
    }
    break;
  case (U'<'):
    if (Peek(0) == U'=') {
      Advance();
      AddToken(TokenType::kLessEqual);
      column_delta_ = 2;
    } else {
      AddToken(TokenType::kLess);
    }
    break;
  case (U'&'):
    AddToken(TokenType::kLogicalAnd);
    break;
  case (U'|'):
    AddToken(TokenType::kLogicalOr);
    break;
  case (U' '):
  case (U'\r'):
  case (U'\t'):
    break;
  case (U'\n'):
    line_++;
    column_ = 1;
    column_delta_ = 0;
    break;
  case (U'"'):
    String();
    break;
  case (U'\''):
    Character();
    break;
  default:
    if (IsAlpha(current_char)) {
      Identifier();
    } else if (IsDigit(current_char)) {
      Integer();
    } else {
      diagnostic_reporter_.Report(
          std::move(SourceCodeLocation{
              .source_name = file_path_, .line = line_, .column = column_}),
          Severity::kFatal,
          std::format(
              "[E]: Unrecgonized character present inside the source file: {}.",
              U32StringToUtf8(std::u32string(1, current_char))));
    }
  }

  UpdateColumnNumber();

  return;
}

bool Lexer::Match(char32_t expected) {
  if (IsAtEnd() || (source_code_codepoints_[current_] != expected)) {
    return false;
  } else {
    Advance();
    return true;
  }
}

char32_t Lexer::Peek(std::size_t offset) {
  if ((current_ + offset) >= source_code_codepoints_.length()) {
    return U'\0';
  }
  return source_code_codepoints_[current_ + offset];
}

std::optional<char32_t>
Lexer::ProcessSingleEscape(char32_t current_backslash_char,
                           char32_t next_char_after_backslash) {
  switch (next_char_after_backslash) {
  case (U'\\'):
    // Returns the '\' character.
    return U'\\';
  case (U'n'):
    // Returns a newline character '\n'
    return U'\n';
  case (U'\''):
    // Returns a single-quote character '\''
    return U'\'';
  case (U'\"'):
    // Returns a double-quote character '\"'
    return U'\"';
    // TODO: Evaluate if I need to add other escape characters if Eta supports
    // them:  (ex: \t, \r, \b, \f,
    // \v) case U't':  return U'\t';
  // Possible beginning of a: \x{HHHHHH}
  case (U'x'): {
    if (Peek(0) == U'{') {
      // Consumes the '{' character.
      Advance();
      std::u32string hex_digits;
      // Collect hex digits, limiting them by 6 hex digits.
      while (IsHexDigit(Peek(0)) && hex_digits.length() < 6) {
        hex_digits += Advance();
      }

      // Verifies the enclosing of the sequence and if it is valid.
      if (hex_digits.empty() || Peek(0) != U'}') {
        diagnostic_reporter_.Report(
            std::move(SourceCodeLocation{
                .source_name = file_path_, .line = line_, .column = column_}),
            Severity::kFatal,
            std::format("[E]: Invalid or unclosed Unicode escape sequence: "
                        "\\x{} present within the source file.",
                        U32StringToUtf8(hex_digits)));
        return std::nullopt;
      }
      if (hex_digits.length() == 0) {
        diagnostic_reporter_.Report(
            std::move(SourceCodeLocation{
                .source_name = file_path_, .line = line_, .column = column_}),
            Severity::kFatal,
            std::format("[E]: Empty hexadecimal digits in Unicode escape "
                        "sequence within the source file."));
        return std::nullopt;
      }

      // Consumes the '}' character.
      Advance();

      try {
        unsigned long unicode_value =
            std::stoul(U32StringToUtf8(hex_digits), nullptr, 16);
        return static_cast<char32_t>(unicode_value);
      } catch (const std::out_of_range &oor) {
        diagnostic_reporter_.Report(
            std::move(SourceCodeLocation{
                .source_name = file_path_, .line = line_, .column = column_}),
            Severity::kFatal,
            std::format("[E]: There is a Unicode escape value 0x{} is out of "
                        "valid range within the source file.",
                        U32StringToUtf8(hex_digits)));
        return std::nullopt;
      } catch (const std::invalid_argument &ia) {
        diagnostic_reporter_.Report(
            std::move(SourceCodeLocation{
                .source_name = file_path_, .line = line_, .column = column_}),
            Severity::kFatal,
            std::format("[E]: Invalid hexadecimal digits in Unicode escape "
                        "sequence: \\x{} present within the source file.",
                        U32StringToUtf8(hex_digits)));
        return std::nullopt;
      }
    } else { // É '\x' mas não é '\x{'
      diagnostic_reporter_.Report(
          std::move(SourceCodeLocation{
              .source_name = file_path_, .line = line_, .column = column_}),
          Severity::kFatal,
          std::format("[E]: Unknown escape sequence '\\x' (missing opening "
                      "brace) present within the source file."));
      return std::nullopt;
    }
  }
  default:
    diagnostic_reporter_.Report(
        std::move(SourceCodeLocation{
            .source_name = file_path_, .line = line_, .column = column_}),
        Severity::kFatal,
        std::format(
            "[E]: Unknown escape sequence present within the source file."));
    return std::nullopt;
  }
}

void Lexer::ReportError(const std::string &message, unsigned int col_offset) {
  diagnostic_reporter_.Report(
      std::move(SourceCodeLocation{.source_name = file_path_,
                                   .line = line_,
                                   .column = column_ + col_offset}),
      Severity::kFatal, message);
}

void Lexer::String() {
  std::u32string literal_content;

  while (!IsAtEnd() && Peek(0) != U'"') {
    char32_t current_char = Advance();

    if (current_char == U'\n') {
      line_++;
      column_ = 1;
      diagnostic_reporter_.Report(
          std::move(SourceCodeLocation{
              .source_name = file_path_, .line = line_, .column = column_}),
          Severity::kFatal,
          std::format("[E]: A string literal cannot span multiple lines in a "
                      "source file."));
      return;
    }

    if (current_char == U'\\') {
      if (IsAtEnd()) {
        diagnostic_reporter_.Report(
            std::move(SourceCodeLocation{
                .source_name = file_path_, .line = line_, .column = column_}),
            Severity::kFatal,
            std::format("[E]: Unfinished escape sequence at end of the source "
                        "file: {}.",
                        file_path_));
        return;
      }

      char32_t escape_char_indicator = Advance();
      std::optional<char32_t> escaped_cp =
          ProcessSingleEscape(current_char, escape_char_indicator);

      if (escaped_cp) {
        literal_content += escaped_cp.value();
      } else {
        // The method 'ProcessSingleEscape' already reports an error.
        return;
      }
    } else {
      literal_content += current_char;
    }
  }

  // Verifies if the string literal has been correctly terminated.
  if (IsAtEnd() || Peek(0) != U'"') {
    diagnostic_reporter_.Report(
        std::move(SourceCodeLocation{
            .source_name = file_path_, .line = line_, .column = column_}),
        Severity::kFatal,
        std::format("[E]: An unterminated string literal was found within the "
                    "source file."));
    return;
  }

  // Consumes the enclosing double-quote character: '"'.
  Advance();

  AddToken(TokenType::kString, literal_content);

  column_delta_ = current_ - start_;

  return;
}

// TODO: Check if this is OK.
void Lexer::UpdateColumnNumber() {
  column_ += column_delta_;

  return;
}

std::string Lexer::U32StringToUtf8(const std::u32string &u32str) {
  std::string s;
  s.reserve(u32str.length() * 4); // Optimization: Max of 4 bytes per char32_t.
  utf8::utf32to8(u32str.begin(), u32str.end(), std::back_inserter(s));
  return s;
}

const std::vector<Token> &Lexer::LexTokens() {
  while (!IsAtEnd()) {
    start_ = current_;
    LexToken();
    if (diagnostic_reporter_.HasFatalErrors()) {
      break;
    }
  }
  // Add a file end token at the end of the token sequence in order to make the
  // parsing stage easier.
  tokens_.emplace_back(Token(line_, column_, TokenType::kFileEnd, nullptr, ""));

  return tokens_;
}
