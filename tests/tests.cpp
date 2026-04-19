#include <gtest/gtest.h>
#include "server.h"
#include "date.h"
#include "authenticator.h"

TEST(stringToWords_Test, HelloWorld){
    
    std::string hello = "Hello, world!";
    std::vector<std::string> wordsEXP = {"Hello,","world!"};
    std::vector<std::string> wordsBAD = {"Hello","world"};
    std::vector<std::string> wordsRES;

    stringToWords(hello, wordsRES);
    
    EXPECT_EQ(wordsRES, wordsEXP);
    EXPECT_NE(wordsRES, wordsBAD);
}
TEST(stringToWords_Test, IAmIronMan){
    
    std::string hello = "I am Iron Man";
    std::vector<std::string> wordsEXP = {"I", "am", "Iron", "Man"};
    std::vector<std::string> wordsRES;

    stringToWords(hello, wordsRES);
    
    EXPECT_EQ(wordsRES, wordsEXP);
}
TEST(stringToWords_Test, QWERTY){
    
    std::string hello = "QWERTY, Q, W, E, R, T. QWERTY, Q, Q, Q, q, q, q?";
    std::vector<std::string> wordsEXP = {"QWERTY,", "Q,", "W,", "E,", "R,", "T.", "QWERTY,", "Q,", "Q,", "Q,", "q,", "q,", "q?"};
    std::vector<std::string> wordsRES;

    stringToWords(hello, wordsRES);
    
    EXPECT_EQ(wordsRES, wordsEXP);
}
