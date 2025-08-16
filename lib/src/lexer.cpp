#include "senbonzakura/lexer.hpp"

#include <any>
#include <format>
#include <string>

#include "senbonzakura/diagnostic_reporter.hpp"
#include "senbonzakura/token.hpp"
#include "senbonzakura/token_type.hpp"

Lexer::Lexer(const std::string &file_path, const std::string &source_code,
             DiagnosticReporter &diagnostic_reporter)
    : file_path_(file_path), source_code_(source_code),
      diagnostic_reporter_(diagnostic_reporter) {}

void Lexer::AddToken(TokenType token_type) {
  AddToken(token_type, {});

  return;
}

void Lexer::AddToken(TokenType token_type, std::any value) {
  std::string lexeme{source_code_.substr(start_, current_ - start_)};
  tokens_.emplace_back(Token(line_, column_, token_type, value, lexeme));

  return;
}

char Lexer::Advance() {
  current_++;

  return source_code_[current_ - 1];
}

void Lexer::Character() {
  if (IsAtEnd()) {
    diagnostic_reporter_.ReportCompilerError(
        SourceCodeLocation{
            .source_name = file_path_,
            .line = line_,
            .column = column_,
        },
        Severity::kFatal,
        std::format(
            "[E]: An unterminated character literal was found within the "
            "source file."));
    return;
  }

  if (Peek(0) == '\'') {
    diagnostic_reporter_.ReportCompilerError(
        SourceCodeLocation{
            .source_name = file_path_,
            .line = line_,
            .column = column_,
        },
        Severity::kFatal,
        std::format("[E]: An empty character literal ('') was found within the "
                    "source file. This is not a valid ASCII character."));
    return;
  }

  char current_char = Advance();

  // Verifies if the character literal has been correctly terminated.
  if (IsAtEnd() || Peek(0) != '\'') {
    diagnostic_reporter_.ReportCompilerError(
        std::move(SourceCodeLocation{
            .source_name = file_path_, .line = line_, .column = column_}),
        Severity::kFatal,
        std::format("[E]: A character literal that does not follow the Eta "
                    "Specification Document was found within the "
                    "source file."));
    return;
  }

  // Consumes the enclosing single-quote character: '\''.
  Advance();
  AddToken(TokenType::kCharacter, current_char);
  column_delta_ = current_ - start_;

  return;
}

void Lexer::Comment() {
  while (!IsAtEnd() && Peek(0) != '\n') {
    column_delta_++;
    Advance();
  }

  return;
}

void Lexer::Identifier() {
  while (IsAlphaNumeric(Peek(0))) {
    Advance();
  }

  std::string lexeme{source_code_.substr(start_, current_ - start_)};
  TokenType token_type = TokenType::kIdentifier;

  if (keywords_to_tokentypes_.contains(lexeme)) {
    token_type = keywords_to_tokentypes_[lexeme];
  }

  if (token_type == TokenType::kFalse) {
    AddToken(token_type, false);
  } else if (token_type == TokenType::kTrue) {
    AddToken(token_type, true);
  } else {
    AddToken(token_type);
  }

  column_delta_ = current_ - start_;

  return;
}

void Lexer::Integer() {
  while (IsDigit(Peek(0))) {
    Advance();
  }

  std::string int_as_str{source_code_.substr(start_, current_ - start_)};
  std::any value = std::stoi(int_as_str);

  AddToken(TokenType::kInteger, value);

  column_delta_ = current_ - start_;

  return;
}

bool Lexer::IsAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::IsAlphaNumeric(char c) {
  return (IsAlpha(c) || IsDigit(c) || c == '_' || c == '\'');
}

bool Lexer::IsAtEnd() { return current_ >= source_code_.length(); }

bool Lexer::IsDigit(char c) { return (c >= '0' && c <= '9'); }

bool Lexer::IsHexDigit(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

void Lexer::LexToken() {
  char current_char = Advance();
  column_delta_ = 1;

  switch (current_char) {
  case ('('):
    AddToken(TokenType::kLeftParen);
    break;
  case (')'):
    AddToken(TokenType::kRightParen);
    break;
  case ('{'):
    AddToken(TokenType::kLeftBracket);
    break;
  case ('}'):
    AddToken(TokenType::kRightBracket);
    break;
  case ('['):
    AddToken(TokenType::kLeftSquareBracket);
    break;
  case (']'):
    AddToken(TokenType::kRightSquareBracket);
    break;
  case (','):
    AddToken(TokenType::kComma);
    break;
  case (':'):
    AddToken(TokenType::kColon);
    break;
  case (';'):
    AddToken(TokenType::kSemiColon);
    break;
  case ('+'):
    AddToken(TokenType::kPlus);
    break;
  case ('-'):
    AddToken(TokenType::kMinus);
    break;
  case ('%'):
    AddToken(TokenType::kRemainder);
    break;
  case ('*'):
    if (Peek(0) == '>' && Peek(1) == '>') {
      Advance();
      Advance();
      AddToken(TokenType::kHighMul);
      column_delta_ = current_ - start_;
    } else {
      AddToken(TokenType::kStar);
    }
    break;
  case ('/'):
    if (Match('/')) {
      column_delta_ = 2;
      Comment();
    } else {
      AddToken(TokenType::kSlash);
    }
    break;
  case ('_'):
    AddToken(TokenType::kUnderscore);
    break;
  case ('!'):
    if (Peek(0) == '=') {
      Advance();
      AddToken(TokenType::kBangEqual);
      column_delta_ = 2;
    } else {
      AddToken(TokenType::kBang);
    }
    break;
  case ('='):
    if (Peek(0) == '=') {
      Advance();
      AddToken(TokenType::kEqualEqual);
      column_delta_ = 2;
    } else {
      AddToken(TokenType::kEqual);
    }
    break;
  case ('>'):
    if (Peek(0) == '=') {
      Advance();
      AddToken(TokenType::kGreaterEqual);
      column_delta_ = 2;
    } else {
      AddToken(TokenType::kGreater);
    }
    break;
  case ('<'):
    if (Peek(0) == '=') {
      Advance();
      AddToken(TokenType::kLessEqual);
      column_delta_ = 2;
    } else {
      AddToken(TokenType::kLess);
    }
    break;
  case ('&'):
    AddToken(TokenType::kLogicalAnd);
    break;
  case ('|'):
    AddToken(TokenType::kLogicalOr);
    break;
  case (' '):
  case ('\r'):
  case ('\t'):
    break;
  case ('\n'):
    line_++;
    column_ = 1;
    column_delta_ = 0;
    break;
  case ('\"'):
    String();
    break;
  case ('\''):
    Character();
    break;
  default:
    if (IsAlpha(current_char)) {
      Identifier();
    } else if (IsDigit(current_char)) {
      Integer();
    } else {
      diagnostic_reporter_.ReportCompilerError(
          std::move(SourceCodeLocation{
              .source_name = file_path_, .line = line_, .column = column_}),
          Severity::kFatal,
          std::format(
              "[E]: Unrecgonized character present inside the source file: {}.",
              std::string(1, current_char)));
    }
  }

  UpdateColumnNumber();

  return;
}

bool Lexer::Match(char expected) {
  if (IsAtEnd() || (source_code_[current_] != expected)) {
    return false;
  } else {
    Advance();
    return true;
  }
}

char Lexer::Peek(std::size_t offset) {
  if ((current_ + offset) >= source_code_.length()) {
    return '\0';
  }
  return source_code_[current_ + offset];
}

void Lexer::String() {
  std::string literal_content;

  while (!IsAtEnd() && Peek(0) != '\"') {
    char current_char = Advance();

    if (current_char == '\n') {
      line_++;
      column_ = 1;
      diagnostic_reporter_.ReportCompilerError(
          std::move(SourceCodeLocation{
              .source_name = file_path_, .line = line_, .column = column_}),
          Severity::kFatal,
          std::format("[E]: A string literal cannot span multiple lines in a "
                      "source file."));
      return;
    } else {
      literal_content += current_char;
    }
  }

  // Verifies if the string literal has been correctly terminated.
  if (IsAtEnd() || Peek(0) != '\"') {
    diagnostic_reporter_.ReportCompilerError(
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

void Lexer::UpdateColumnNumber() {
  column_ += column_delta_;

  return;
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
  tokens_.emplace_back(Token(line_, column_, TokenType::kFileEnd, {}, ""));

  return tokens_;
}
