add_library(lexer OBJECT Lexer.cc TokenType.cc)

target_link_libraries(lexer PUBLIC expected)

