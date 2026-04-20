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
    
    ASSERT_EQ(wordsRES, wordsEXP);
    ASSERT_NE(wordsRES, wordsBAD);
}
TEST(stringToWords_Test, IAmIronMan){
    
    std::string hello = "I am Iron Man";
    std::vector<std::string> wordsEXP = {"I", "am", "Iron", "Man"};
    std::vector<std::string> wordsRES;

    stringToWords(hello, wordsRES);
    
    ASSERT_EQ(wordsRES, wordsEXP);
}
TEST(stringToWords_Test, QWERTY){
    
    std::string hello = "QWERTY, Q, W, E, R, T. QWERTY, Q, Q, Q, q, q, q?";
    std::vector<std::string> wordsEXP = {"QWERTY,", "Q,", "W,", "E,", "R,", "T.", "QWERTY,", "Q,", "Q,", "Q,", "q,", "q,", "q?"};
    std::vector<std::string> wordsRES;

    stringToWords(hello, wordsRES);
    
    ASSERT_EQ(wordsRES, wordsEXP);
}
TEST(dateCompare, equal){
    Date a1("2020-02-12");
    Date a2("2020.02.12");
    Date a3("2020 02 12");
    Date a4("2020-2-12");
    Date a5("2020-02-12; 11:39:43:199");

    Date a[] = {a1,a2,a3,a4,a5};
    
    for (int i=0;i<5;i++){
        for (int j=0;j<i;j++){
            ASSERT_EQ(Date::compare(a[i],a[j]), 0);
        }
    }
}
TEST(dateCompare, more){
    Date a("2020-02-12");
    
    Date b2("2020-02-10");
    Date b1("2020-01-12");
    Date b3("2000-02-12");
    Date b4("2000-01-10");
 
    Date b[] = {b1,b2,b3,b4};

    for (int i=0;i<4;i++){
        ASSERT_EQ(Date::compare(a, b[i]), 1);
    }
}
TEST(dateCompare, less){
    Date a("2020-02-12");
    
    Date c1("2020-02-22");
    Date c2("2020-03-12");
    Date c3("2022-02-12");
    Date c4("2022-03-22");

    Date c[] = {c1,c2,c3,c4};

    for (int i=0;i<4;i++){
        ASSERT_EQ(Date::compare(a, c[i]), -1);
    }
}
TEST(dateToString, toString){
    Date a1("2020-02-12");
    Date a2("2020.02.12");
    Date a3("2020 02 12");
    Date a4("2020-2-12");
    Date a5("2020-02-12; 11:39:43:199");

    Date a[] = {a1,a2,a3,a4,a5};
    
    for (int i=0;i<5;i++){
        for (int j=0;j<i;j++){
            ASSERT_EQ(a[i].toString(),a[j].toString());
        }
    }
}

