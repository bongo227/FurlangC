#include <gtest/gtest.h>
#include "lexer_test.cpp"
#include "parser_test.cpp"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}