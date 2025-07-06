#include "../include/senbonzakura/lexer.hpp"

#include <any>
#include <string>

#include "../include/senbonzakura/token.hpp"

// TODO: Add functionality to automatically update the column number.

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

void Lexer::Identifier() { return; }

void Lexer::Integer() { return; }

bool Lexer::IsAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::IsAlphaNumeric(char c) { return (IsAlpha(c) || IsDigit(c)); }

bool Lexer::IsAtEnd() { return current_ >= source_code_.length(); }

bool Lexer::IsDigit(char c) { return (c >= '0' && c <= '9'); }

// TODO: Refactor this specific method, so it can be shorter.
void Lexer::LexToken() {
  char current_char = Advance();

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
    AddToken(TokenType::kStar);
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
      AddToken(TokenType::kBangEqual);
    }
    break;
  case ('='):
    if (Peek(0) != '=') {
      AddToken(TokenType::kEqual);
    } else {
      AddToken(TokenType::kEqualEqual);
    }
    break;
  case ('>'):
    if (Peek(0) != '=') {
      AddToken(TokenType::kGreater);
    } else {
      AddToken(TokenType::kGreaterEqual);
    }
    break;
  case ('<'):
    if (Peek(0) != '=') {
      AddToken(TokenType::kLess);
    } else {
      AddToken(TokenType::kLessEqual);
    }
    break;
  case ('&'):
    if (Peek(0) == '&') {
      AddToken(TokenType::kLogicalAnd);
    } else {
      // TODO: Report lexical error.
    }
    break;
  case ('|'):
    if (Peek(0) == '|') {
      AddToken(TokenType::kLogicalOr);
    } else {
      // TODO: Report lexical error.
    }
    break;
    // TODO: Deal with whitespace characters.
    // TODO: Deal with the 'High Mul' token.
    // TODO: Deal with the 'String' token.
  default:
    // TODO: Deal with identifiers and also keywords from the Eta language.
    if (IsAlpha(current_char)) {
      Identifier();
      // TODO: Deal with integer constants.
    } else if (IsDigit(current_char)) {
      Integer();
    }
  }

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

void Lexer::String() {}

const std::vector<Token> &Lexer::LexTokens() {
  while (!IsAtEnd()) {
    start_ = current_;
    LexToken();
  }
  tokens_.emplace_back(Token(line_, column_, TokenType::kFileEnd, nullptr, ""));

  return tokens_;
}
