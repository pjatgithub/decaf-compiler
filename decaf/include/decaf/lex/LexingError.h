#ifndef DECAF_LEX_LEXINGERROR_H
#define DECAF_LEX_LEXINGERROR_H

#include "decaf/lex/Token.h"
#include <string>
#include <utility>

namespace decaf {

namespace lex {

class LexingError {
public:
    enum class ErrorType {
        INVALID_TOKEN,
        IO,
    };

    ErrorType getType() const { return type; }

    const std::string &getMessage() const { return message; }

    LexingError(ErrorType t, const char *m, unsigned line, unsigned col)
        : type(t), message(m), location(line, col) {}

    LexingError(ErrorType t, std::string &&m, unsigned line, unsigned col)
        : type(t), message(m), location(line, col) {}

    static LexingError io(std::string &&m) {
        return LexingError(ErrorType::IO, std::forward<std::string>(m), 0, 0);
    }

    static LexingError io(const char *m) {
        return LexingError(ErrorType::IO, m, 0, 0);
    }

    static LexingError invalidToken(
        std::string &&m,
        unsigned line,
        unsigned col
    )
    {
        return LexingError(
            ErrorType::INVALID_TOKEN,
            std::forward<std::string>(m),
            line,
            col);
    }

    static LexingError invalidToken(
        const char *m,
        unsigned line,
        unsigned col
    )
    {
        return LexingError(ErrorType::INVALID_TOKEN, m, line, col);
    }

private:
    ErrorType type;

    std::string message;

    SourceLocation location;
};

} // namespace lex

} // namespace decaf

#endif // DECAF_LEX_LEXINGERROR_H

