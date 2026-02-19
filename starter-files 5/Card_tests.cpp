#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <sstream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(test_card_def_ctor){
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}

TEST(test_rank_and_suit){
    Rank r = THREE;
    Suit s = CLUBS;
    Card c(r,s);
    ASSERT_EQUAL(r,c.get_rank());
    ASSERT_EQUAL(s,c.get_suit());
}

TEST(test_face_card){
    Card c(QUEEN,SPADES);
    ASSERT_TRUE(c.is_face_or_ace());
}

TEST(test_right_bower){
    Suit trump = HEARTS;
    Card c(JACK, HEARTS);
    ASSERT_TRUE(c.is_right_bower(trump));
}

TEST(test_left_bower){
    Suit trump = HEARTS;
    Card c(JACK, DIAMONDS);
    ASSERT_TRUE(c.is_left_bower(trump));
}

TEST(test_trump){
    Suit trump = CLUBS;
    Card c1(TEN, SPADES);
    Card c2(NINE, CLUBS);
    ASSERT_FALSE(c1.is_trump(trump));
    ASSERT_TRUE(c2.is_trump(trump));
}

TEST(test_output){
    Card three_spades = Card(THREE, SPADES);
    ostringstream oss;
    oss << three_spades;
    ASSERT_EQUAL(oss.str(), "Three of Spades");
}
TEST(test_input){
    istringstream iss("Three of Spades");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(THREE, SPADES));
}
// Add more test cases here

TEST_MAIN()
