#ifndef DECAF_LEX_TOKENTYPE_H
#define DECAF_LEX_TOKENTYPE_H

#include <optional>

namespace decaf {

namespace lex {

enum class TokenType: unsigned short {
#define TOKEN(token, tokenStr)              TOKEN_ ## token,
#define PUNCTUATOR(token, tokenStr)         PUNCTUATOR_ ## token,
#define KEYWORD(token, tokenStr)            KEYWORD_ ## token,
#include "decaf/lex/TokenType.def"
};

const char *getTokenName(TokenType type);

std::optional<TokenType> fromKeywordName(const char *name);

std::optional<TokenType> fromPunctuatorName(const char *name);

} // namespace lex

} // namespace decaf

#endif // DECAF_LEX_TOKENTYPE_H

