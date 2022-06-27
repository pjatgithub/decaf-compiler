#include "decaf/lex/Lexer.h"
#include "decaf/common/ExpectedUtil.h"
#include <tl/expected.hpp>
#include <cctype>
#include <cstdio>
#include <string>
#include <utility>
#include <cerrno>
#include <cstring>
#include <climits>

namespace decaf {

namespace lex {

struct Lexer::LexerImpl {
    static constexpr int START = 1 << CHAR_BIT;

    enum class CommentState {
        STARTING_SLASH,
        SINGLE_LINE,
        MULTIPLE_LINE,
        ENDING_ASTERISK,
        FINISH
    };

    static Result<char> getChar(Lexer &lexer) {
        int ch, err;

        if (lexer.lastChar == EOF || lexer.unget) {
            ch = lexer.lastChar;
            err = lexer.lastErrno;
            lexer.unget = false;
        } else {
            ch = lexer.inStream->get();
            err = errno;

            if (lexer.lastChar == '\n') {
                lexer.lastLine++;
                lexer.lastCol = 1;
            } else {
                lexer.lastCol++;
            }

            lexer.lastChar = ch;
            lexer.lastErrno = err;
        }

        if (ch != EOF || errno == 0) {
            return {tl::in_place, ch};
        } else {
            return tl::make_unexpected(
                LexingError::io(std::strerror(err)));
        }
    }

    static void ungetChar(Lexer &lexer) {
        if (lexer.lastChar != START) {
            lexer.unget = true;
        }
    }

    static bool isValidIntLiteral(const std::string &literal) {
        if (literal.find("0x") != std::string::npos) {
            // hexdecimal
            if (literal.size() == 2) {
                return false;
            }

            for (std::size_t i = 2; i < literal.size(); i++) {
                if (!std::isxdigit(literal[i])) {
                    return false;
                }
            }
        } else {
            // decimal
            for (auto &ch : literal) {
                if (!std::isdigit(ch)) {
                    return false;
                }
            }
        }

        return true;
    }
};

Lexer::Lexer(const char *name, std::istream &ist):
    Lexer(name, &ist, false) {}

Lexer::Lexer(const char *name, std::istream *ist):
    Lexer(name, ist, true) {}

Lexer::Lexer(const char *name, std::istream *ist, bool own):
    sourceName(name), inStream(ist), lastErrno(0), lastChar(LexerImpl::START),
    lastLine(1), lastCol(0), unget(false), own(own) {}

Lexer::~Lexer() {
    if (own) {
        delete inStream;
    }
}

Lexer::Result<Token> Lexer::getToken() {
    int ch = EXPECTED_TRY(LexerImpl::getChar(*this));

    while (std::isspace(ch) || ch == '/') {
        if (ch == '/') {
            unsigned line = lastLine;
            unsigned col = lastCol;
            LexerImpl::CommentState state =
                LexerImpl::CommentState::STARTING_SLASH;

            while (state != LexerImpl::CommentState::FINISH) {
                ch = EXPECTED_TRY(LexerImpl::getChar(*this));

                switch (state) {
                    case LexerImpl::CommentState::STARTING_SLASH:
                        if (ch == '/') {
                            state = LexerImpl::CommentState::SINGLE_LINE;
                        } else if (ch == '*') {
                            state = LexerImpl::CommentState::MULTIPLE_LINE;
                        } else {
                            LexerImpl::ungetChar(*this);
                            return Result<Token>(
                                tl::in_place,
                                TokenType::PUNCTUATOR_SLASH,
                                std::make_pair(line, col));
                        }
                        break;
                    case LexerImpl::CommentState::SINGLE_LINE:
                        if (ch == '\n' || ch == EOF) {
                            state = LexerImpl::CommentState::FINISH;
                        }
                        break;
                    case LexerImpl::CommentState::MULTIPLE_LINE:
                        if (ch == '*') {
                            state = LexerImpl::CommentState::ENDING_ASTERISK;
                        } else if (ch == EOF) {
                            // LexingError
                            return tl::make_unexpected(
                                LexingError::invalidToken(
                                    "unterminated /* comment", line, col));
                        }
                        break;
                    case LexerImpl::CommentState::ENDING_ASTERISK:
                        if (ch == '/') {
                            state = LexerImpl::CommentState::FINISH;
                        } else {
                            state = LexerImpl::CommentState::MULTIPLE_LINE;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        ch = EXPECTED_TRY(LexerImpl::getChar(*this));
    }

    if (ch == EOF) {
        return Result<Token>(
                tl::in_place,
                TokenType::TOKEN_EOF,
                std::make_pair(0, 0));
    }

    unsigned line = lastLine;
    unsigned col = lastCol;

    if (std::isalpha(ch)) {
        std::string identifier{static_cast<char>(ch)};

        while (std::isalnum(ch = EXPECTED_TRY(LexerImpl::getChar(*this)))) {
            identifier += static_cast<char>(ch);
        }

        LexerImpl::ungetChar(*this);
        auto type = fromKeywordName(identifier.c_str());

        if (type) {
            return Result<Token>(
                tl::in_place, *type, std::make_pair(line, col));
        } else {
            return Result<Token>(
                tl::in_place,
                TokenType::TOKEN_IDENTIFIER,
                std::make_pair(line, col),
                std::move(identifier));
        }
    }

    if (std::isdigit(ch)) {
        std::string intLiteral{static_cast<char>(ch)};

        while (std::isalnum(ch = EXPECTED_TRY(LexerImpl::getChar(*this)))) {
            intLiteral += static_cast<char>(ch);
        }

        LexerImpl::ungetChar(*this);

        if (LexerImpl::isValidIntLiteral(intLiteral)) {
            return Result<Token>(
                tl::in_place,
                TokenType::TOKEN_INT_LITERAL,
                std::make_pair(line, col),
                std::move(intLiteral));
        } else {
            std::string msg("invalid integer literal '");
            msg += intLiteral;
            msg += '\'';
            return tl::make_unexpected(
                LexingError::invalidToken(std::move(msg), line, col));
        }
    }

    std::string lexeme{static_cast<char>(ch)};
    ch = EXPECTED_TRY(LexerImpl::getChar(*this));
    std::optional<TokenType> puncType;

    if (ch != EOF) {
        lexeme += static_cast<char>(ch);
        auto type = fromPunctuatorName(lexeme.c_str());

        if (!type) {
            lexeme.pop_back();
            LexerImpl::ungetChar(*this);
        } else {
            puncType = std::move(type);
        }
    }

    if (!puncType) {
        auto type = fromPunctuatorName(lexeme.c_str());

        if (!type) {
            std::string msg("unrecognized token '");
            msg += lexeme;
            msg += '\'';
            return tl::make_unexpected(
                LexingError::invalidToken(std::move(msg), line, col));
        } else {
            puncType = std::move(type);
        }
    }

    return Result<Token>(
        tl::in_place,
        *puncType,
        std::make_pair(line, col));
}

const std::string &Lexer::getSourceName() const {
    return sourceName;
}

} // namespace lex

} // namespace decaf

