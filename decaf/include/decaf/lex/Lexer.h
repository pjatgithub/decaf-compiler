#ifndef DECAF_LEX_LEXER_H
#define DECAF_LEX_LEXER_H

#include "decaf/lex/Token.h"
#include "decaf/lex/LexingError.h"
#include <istream>
#include <tl/expected.hpp>
#include <string>

namespace decaf {

namespace lex {

class Lexer {
public:
    template<class T> using Result = tl::expected<T, LexingError>;

    Lexer(const char *name, std::istream &ist);

    Lexer(const char *name, std::istream *ist);

    ~Lexer();

    Result<Token> getToken();

    const std::string &getSourceName() const;

private:
    Lexer(const char *name, std::istream *ist, bool own);

    std::string sourceName;

    std::istream *inStream;

    int lastErrno;

    int lastChar;

    unsigned int lastLine;

    unsigned int lastCol;

    bool unget;

    bool own;

    struct LexerImpl;
};

} // namespace lex

} // namespace decaf

#endif // DECAF_LEX_LEXER_H

