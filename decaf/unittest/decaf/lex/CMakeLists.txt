add_executable(LexerTest LexerTest.cc)
target_link_libraries(LexerTest PRIVATE lexer GTest::gtest_main)
gtest_discover_tests(LexerTest TokenTypeTest)

add_executable(TokenTypeTest TokenTypeTest.cc)
target_link_libraries(TokenTypeTest PRIVATE lexer GTest::gtest_main)
gtest_discover_tests(TokenTypeTest)

