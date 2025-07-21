#include "senbonzakura/token.hpp"

#include <format>
#include <iostream>

#include <utf8.h>

Token::Token(int line, int column, TokenType token_type, std::any value,
             std::string lexeme)
    : line_(line), column_(column), token_type_(token_type),
      value_(std::move(value)), lexeme_(std::move(lexeme)) {}

std::string U32StringToUtf8(const std::u32string &u32str) {
  std::string s;
  s.reserve(u32str.length() * 4); // Optimization: Max of 4 bytes per char32_t.
  utf8::utf32to8(u32str.begin(), u32str.end(), std::back_inserter(s));
  return s;
}

std::u32string EscapeU32StringForDisplay(const std::u32string &u32str) {
  std::u32string escaped_str;
  for (char32_t cp : u32str) {
    switch (cp) {
    case U'\n':
      escaped_str += U"\\n";
      break; // Nova linha -> \n
    case U'\t':
      escaped_str += U"\\t";
      break; // Tab -> \t
    case U'\r':
      escaped_str += U"\\r";
      break; // Retorno de carro -> \r
    case U'\\':
      escaped_str += U"\\\\";
      break; // Barra invertida -> \\
            case U'"':  escaped_str += U"\\\""; break;  // Aspa dupla -> \"
      // Adicione outros escapes comuns se necessário (ex: \0, \a, \b, \f, \v)
    default:
      // Caracteres ASCII imprimíveis (sem escapamento especial acima)
      if (cp >= 0x20 && cp <= 0x7E) {
        escaped_str += cp;
      } else {
        // Para outros caracteres Unicode ou não-imprimíveis, use \x{...}
        // Certifique-se de que std::format suporte char32_t ou converta para
        // int
        std::string hex_part =
            std::format("{:x}", static_cast<unsigned int>(cp));
        // Adicione padding de zeros para 4 ou mais dígitos hexadecimais se
        // desejar Ex: if (hex_part.length() < 4) hex_part = std::string(4 -
        // hex_part.length(), '0') + hex_part;
        escaped_str += U"\\x{";
        for (char c : hex_part) {
          escaped_str += static_cast<char32_t>(c);
        }
        escaped_str += U"}";
      }
      break;
    }
  }
  return escaped_str;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << std::format("[{:03}:{:03}] Type: {}", token.line_, token.column_,
                    token.token_type_);

  switch (token.token_type_) {
  case TokenType::kString: {
    try {
      // Tenta converter o 'value_' para std::u32string
      const std::u32string &s_value =
          std::any_cast<const std::u32string &>(token.value_);
      // Converte a std::u32string para std::string (UTF-8) para impressão
      os << std::format(" - Value: {}",
                        U32StringToUtf8(EscapeU32StringForDisplay(s_value)));
    } catch (const std::bad_any_cast &e) {
      os << " - Value: <Error: Bad Any Cast for string>";
    }
    break;
  }
  case TokenType::kInteger: {
    try {
      int i_value = std::any_cast<int>(token.value_);
      os << std::format(" - Value: {}", i_value);
    } catch (const std::bad_any_cast &e) {
      os << " - Value: <Error: Bad Any Cast for integer>";
    }
    break;
  }
  case TokenType::kTrue:
  case TokenType::kFalse: {
    try {
      bool b_value = std::any_cast<bool>(token.value_);
      os << std::format(" - Value: {}", b_value ? "true" : "false");
    } catch (const std::bad_any_cast &e) {
      os << " - Value: <Error: Bad Any Cast for boolean>";
    }
    break;
  }
  default:
    os << std::format(" - Lexeme: \"{}\"",
                      token.lexeme_); // Mantém o lexema para outros tipos
    break;
  }
  return os;
}
