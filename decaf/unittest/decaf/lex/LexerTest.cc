#include "decaf/lex/Lexer.h"
#include <gtest/gtest.h>
#include <sstream>

#define DECAF_LEXER_ASSERT_NEXT_TOKEN(lexer, typeExpected, dataExpected)    \
    do {                                                                    \
        auto token = lexer.getToken();                                      \
        ASSERT_TRUE(token.has_value());                                     \
        ASSERT_EQ(typeExpected, token->getType());                          \
        ASSERT_STREQ(dataExpected, token->getData().c_str());               \
    } while (false)

#define DECAF_LEXER_ASSERT_NEXT_TOKEN_KEYWORD(lexer, keywordExpected)       \
    DECAF_LEXER_ASSERT_NEXT_TOKEN(lexer, keywordExpected, "")

#define DECAF_LEXER_ASSERT_NEXT_TOKEN_PUNCTUATOR(lexer, puncExpected)       \
    DECAF_LEXER_ASSERT_NEXT_TOKEN(lexer, puncExpected, "")

#define DECAF_LEXER_ASSERT_NEXT_TOKEN_IDENTIFIER(lexer, nameExpected)       \
    DECAF_LEXER_ASSERT_NEXT_TOKEN(                                          \
        lexer,                                                          \
        TokenType::TOKEN_IDENTIFIER,                                    \
        nameExpected)

#define DECAF_LEXER_ASSERT_NEXT_TOKEN_SEMICOLON(lexer)                      \
    DECAF_LEXER_ASSERT_NEXT_TOKEN(                                          \
            lexer,                                                          \
            TokenType::PUNCTUATOR_SEMICOLON,                                \
            "")

#define DECAF_LEXER_ASSERT_NEXT_TOKEN_EOF(lexer)                            \
    DECAF_LEXER_ASSERT_NEXT_TOKEN(lexer, TokenType::TOKEN_EOF, "")

#define DECAF_LEXER_ASSERT_NEXT_ERROR(lexer, errorTypeExpected)             \
    do {                                                                    \
        auto token = lexer.getToken();                                      \
        ASSERT_FALSE(token.has_value());                                    \
        ASSERT_EQ(errorTypeExpected, token.error().getType());              \
    } while (false)

namespace {

using namespace decaf::lex;

TEST(LexerTest, testGetDeclaration) {
    Lexer lexer("declaration.decaf", new std::istringstream("int bar;"));
    DECAF_LEXER_ASSERT_NEXT_TOKEN_KEYWORD(lexer, TokenType::KEYWORD_INT);
    DECAF_LEXER_ASSERT_NEXT_TOKEN_IDENTIFIER(lexer, "bar");
    DECAF_LEXER_ASSERT_NEXT_TOKEN_SEMICOLON(lexer);
    DECAF_LEXER_ASSERT_NEXT_TOKEN_EOF(lexer);
}

TEST(LexerTest, testGetAssignment) {
    Lexer lexer("assignment.decaf", new std::istringstream("num = 10;"));
    DECAF_LEXER_ASSERT_NEXT_TOKEN_IDENTIFIER(lexer, "num");
    DECAF_LEXER_ASSERT_NEXT_TOKEN_PUNCTUATOR(
        lexer,
        TokenType::PUNCTUATOR_EQUAL);
    DECAF_LEXER_ASSERT_NEXT_TOKEN(lexer, TokenType::TOKEN_INT_LITERAL, "10");
    DECAF_LEXER_ASSERT_NEXT_TOKEN_SEMICOLON(lexer);
    DECAF_LEXER_ASSERT_NEXT_TOKEN_EOF(lexer);
}

TEST(LexerTest, testGetAssignmentWithComment) {
    Lexer lexer(
        "assignment_comment.decaf",
        new std::istringstream(
            "// This is a single-line comment.\n"
            "/* This is a multi-line\n"
            "   comment.*/"
            "num += 1;"));
    DECAF_LEXER_ASSERT_NEXT_TOKEN_IDENTIFIER(lexer, "num");
    DECAF_LEXER_ASSERT_NEXT_TOKEN_PUNCTUATOR(
        lexer,
        TokenType::PUNCTUATOR_PLUS_EQUAL);
    DECAF_LEXER_ASSERT_NEXT_TOKEN(lexer, TokenType::TOKEN_INT_LITERAL, "1");
    DECAF_LEXER_ASSERT_NEXT_TOKEN_SEMICOLON(lexer);
    DECAF_LEXER_ASSERT_NEXT_TOKEN_EOF(lexer);
}

TEST(LexerTest, testGetUnterminatedComment) {
    Lexer lexer(
        "unterminated_comment.decaf",
        new std::istringstream(
            "/* This is an unterminated comment."));
    DECAF_LEXER_ASSERT_NEXT_ERROR(
        lexer, LexingError::ErrorType::INVALID_TOKEN);
    DECAF_LEXER_ASSERT_NEXT_TOKEN_EOF(lexer);
}

TEST(LexerTest, testGetInvalidInt) {
    Lexer lexer("invalid_int.decaf", new std::istringstream("a = 0xbar;"));
    DECAF_LEXER_ASSERT_NEXT_TOKEN(lexer, TokenType::TOKEN_IDENTIFIER, "a");
    DECAF_LEXER_ASSERT_NEXT_TOKEN_PUNCTUATOR(
        lexer,
        TokenType::PUNCTUATOR_EQUAL);
    DECAF_LEXER_ASSERT_NEXT_ERROR(
            lexer,
            LexingError::ErrorType::INVALID_TOKEN);
    DECAF_LEXER_ASSERT_NEXT_TOKEN_SEMICOLON(lexer);
    DECAF_LEXER_ASSERT_NEXT_TOKEN_EOF(lexer);
}

} // namespace

