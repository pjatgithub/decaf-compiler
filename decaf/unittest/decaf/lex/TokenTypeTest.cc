#include "decaf/lex/TokenType.h"
#include <gtest/gtest.h>

namespace {

using namespace decaf::lex;

TEST(TokenTypeTest, testGetTokenName) {
    ASSERT_STREQ("int", getTokenName(TokenType::KEYWORD_INT));
    ASSERT_EQ(nullptr, getTokenName(TokenType::TOKEN_UNKNOWN));
    ASSERT_EQ(nullptr, getTokenName(TokenType::TOKEN_EOF));
    ASSERT_STREQ(
        "BOOLEANLITERAL",
        getTokenName(TokenType::TOKEN_BOOLEAN_LITERAL));
}

TEST(TokenTypeTest, testFromKeywordName) {
    auto tokenType = fromKeywordName("int");
    ASSERT_TRUE(tokenType.has_value());
    ASSERT_EQ(TokenType::KEYWORD_INT, *tokenType);

    ASSERT_FALSE(fromKeywordName("++").has_value());
    ASSERT_FALSE(fromKeywordName("123456").has_value());
}

TEST(TokenTypeTest, testFromPunctuatorName) {
    auto tokenType = fromPunctuatorName("++");
    ASSERT_TRUE(tokenType.has_value());
    ASSERT_EQ(TokenType::PUNCTUATOR_PLUS_PLUS, *tokenType);

    ASSERT_FALSE(fromPunctuatorName("int").has_value());
    ASSERT_FALSE(fromPunctuatorName("123").has_value());
}

} // namespace

