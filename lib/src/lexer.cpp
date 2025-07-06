#include "../include/senbonzakura/lexer.hpp"

#include <any>
#include <string>

#include "../include/senbonzakura/token.hpp"

Lexer::Lexer(std::string source_code) : source_code_(source_code) {}

void Lexer::AddToken(TokenType token_type) {
  AddToken(token_type, nullptr);

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

  std::string lexeme{source_code_.substr(start_, current_ - start_)};
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
  std::any value = std::stoi(source_code_.substr(start_, current_ - start_));
  AddToken(TokenType::kInteger, value);

  column_delta_ = current_ - start_;

  return;
}

bool Lexer::IsAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::IsAlphaNumeric(char c) {
  return (IsAlpha(c) || IsDigit(c) || c == '_');
}

bool Lexer::IsAtEnd() { return current_ >= source_code_.length(); }

bool Lexer::IsDigit(char c) { return (c >= '0' && c <= '9'); }

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
    AddToken(TokenType::kLeftBrace);
    break;
  case ('}'):
    AddToken(TokenType::kRightBrace);
    break;
  case ('['):
    AddToken(TokenType::kLeftBracket);
    break;
  case (']'):
    AddToken(TokenType::kRightBracket);
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
      Comment();
    } else {
      AddToken(TokenType::kSlash);
    }
    break;
  case ('!'):
    if (Peek(0) != '=') {
      AddToken(TokenType::kBang);
    } else {
      Advance();
      AddToken(TokenType::kBangEqual);
      column_delta_ = 2;
    }
    break;
  case ('='):
    if (Peek(0) != '=') {
      AddToken(TokenType::kEqual);
    } else {
      Advance();
      AddToken(TokenType::kEqualEqual);
      column_delta_ = 2;
    }
    AddToken(Peek(0) != '=' ? TokenType::kEqual : TokenType::kEqualEqual);
    break;
  case ('>'):
    if (Peek(0) != '=') {
      AddToken(TokenType::kGreater);
    } else {
      Advance();
      AddToken(TokenType::kGreaterEqual);
      column_delta_ = 2;
    }
    break;
  case ('<'):
    if (Peek(0) != '=') {
      AddToken(TokenType::kLess);
    } else {
      Advance();
      AddToken(TokenType::kLessEqual);
      column_delta_ = 2;
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
    break;
  case ('"'):
    String();
    break;
  default:
    if (IsAlpha(current_char)) {
      Identifier();
    } else if (IsDigit(current_char)) {
      Integer();
    } else {
      // TODO: Report lexical error.
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

char Lexer::Peek(int offset) {
  char peeked_char = (current_ + offset) >= source_code_.length()
                         ? '\0'
                         : source_code_[current_ + offset];
  return peeked_char;
}

void Lexer::String() {
  while (!IsAtEnd() && Peek(0) != '"') {
    if (Peek(0) == '\n') {
      line_++;
    }
    Advance();
  }

  if (Peek(0) == '"') {
    Advance();
    AddToken(TokenType::kString,
             source_code_.substr(start_ + 1, current_ - start_ - 2));
  } else {
    // TODO: Report error because the string literal was not terminated with a
    // '"' character.
  }

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
  }
  tokens_.emplace_back(Token(line_, column_, TokenType::kFileEnd, nullptr, ""));

  return tokens_;
}
