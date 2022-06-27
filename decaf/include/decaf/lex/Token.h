#ifndef DECAF_LEX_TOKEN_H
#define DECAF_LEX_TOKEN_H

#include "decaf/lex/TokenType.h"
#include <utility>
#include <string>

namespace decaf {

namespace lex {

using SourceLocation = std::pair<unsigned, unsigned>;

class Token {
public:
    TokenType getType() const { return type; }

    const SourceLocation &getLocation() const { return location; }

    const std::string &getData() const { return data; }

    Token(TokenType tokType, SourceLocation srcLoc)
        : type(tokType), location(srcLoc), data() {}

    Token(
        TokenType tokType,
        SourceLocation srcLoc,
        const std::string &tokData
    ) : type(tokType), location(srcLoc), data(tokData) {}

    Token(
        TokenType tokType,
        SourceLocation srcLoc,
        std::string &&tokData
    ) : type(tokType), location(srcLoc), data(tokData) {}

private:
    SourceLocation location;

    std::string data;

    TokenType type;
};

} // namespace lex

} // namespace decaf


#endif // DECAF_LEX_TOKEN_H

