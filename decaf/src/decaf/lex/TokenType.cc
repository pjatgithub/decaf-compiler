#include "decaf/lex/TokenType.h"
#include <cstring>

namespace decaf {

namespace lex {

const char *getTokenName(TokenType type) {
    switch (type) {
#ifdef TOKEN
#undef TOKEN
#endif
#define TOKEN(token, tokenStr)                                      \
        case TokenType::TOKEN_ ## token: return tokenStr;

#ifdef PUNCTUATOR
#undef PUNCTUATOR
#endif
#define PUNCTUATOR(token, tokenStr)                                 \
        case TokenType::PUNCTUATOR_ ## token: return tokenStr;

#ifdef KEYWORD
#undef KEYWORD
#endif
#define KEYWORD(token, tokenStr)                                    \
        case TokenType::KEYWORD_ ## token: return tokenStr;

#include "decaf/lex/TokenType.def"
        default:
            break;
    }

    return nullptr;
}

std::optional<TokenType> fromKeywordName(const char *name) {
#
#ifdef TOKEN
#undef TOKEN
#endif
#define TOKEN(token, tokenStr)

#ifdef PUNCTUATOR
#undef PUNCTUATOR
#endif
#define PUNCTUATOR(token, tokenStr)

#ifdef KEYWORD
#undef KEYWORD
#endif
#define KEYWORD(token, tokenStr)                                    \
    if (!std::strcmp(tokenStr, name)) {                             \
        return TokenType::KEYWORD_ ## token;                        \
    }

#include "decaf/lex/TokenType.def"

    return {};
}

std::optional<TokenType> fromPunctuatorName(const char *name) {
#ifdef TOKEN
#undef TOKEN
#endif
#define TOKEN(token, tokenStr)

#ifdef KEYWORD
#undef KEYWORD
#endif
#define KEYWORD(token, tokenStr)

#ifdef PUNCTUATOR
#undef PUNCTUATOR
#endif
#define PUNCTUATOR(token, tokenStr)                                 \
    if (!std::strcmp(tokenStr, name)) {                             \
        return TokenType::PUNCTUATOR_ ## token;                     \
    }

#include "decaf/lex/TokenType.def"

    return {};
}

} // namespace lex

} // namespace decaf

