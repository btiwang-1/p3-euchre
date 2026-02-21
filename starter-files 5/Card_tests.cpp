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

TEST(test_equality) {
    Card c1(NINE, CLUBS);
    Card c2(NINE, CLUBS);
    Card c3(NINE, SPADES);
    Card c4(TWO, CLUBS);

    ASSERT_TRUE(c1 == c2);   
    ASSERT_FALSE(c1 == c3);  
    ASSERT_FALSE(c1 == c4);  
}

TEST(test_not_equal) {
    Card c1(NINE, CLUBS);
    Card c2(NINE, CLUBS);
    Card c3(NINE, SPADES);
    Card c4(TWO, CLUBS);

    ASSERT_FALSE(c1 != c2);  
    ASSERT_TRUE(c1 != c3);  
    ASSERT_TRUE(c1 != c4);  
}


TEST(test_all_comparisons) {
    Card c1(FIVE, HEARTS);
    Card c2(ACE, HEARTS);
    Card c3(FIVE, DIAMONDS);

    ASSERT_TRUE(c1 < c2);
    ASSERT_FALSE(c2 < c1);
    ASSERT_TRUE(c2 > c1);
    ASSERT_TRUE(c1 <= c2)
    ASSERT_TRUE(c2 >= c1);

    ASSERT_TRUE(c1 < c3); //in the instance of a tie
    ASSERT_TRUE(c1 <= c3); // in tie
    ASSERT_FALSE(c3 < c1); 
}

TEST(test_lowestrank_trump) {
    Suit trump = SPADES;
    Card c1(TEN, DIAMONDS);  
    Card c2(TEN, SPADES);  

    ASSERT_TRUE(Card_less(c1, c2, trump));  
    ASSERT_FALSE(Card_less(c2, c1, trump));
}

TEST(test_get_suit_withtrump) {
    Suit trump = DIAMONDS;
    Card rightBOwer(JACK, DIAMONDS);
    ASSERT_EQUAL(rightBOwer.get_suit(trump), DIAMONDS);
    Card leftbower(JACK, HEARTS);
    ASSERT_EQUAL(leftbower.get_suit(trump), DIAMONDS);
    Card reguCARD(TEN, CLUBS);
    ASSERT_EQUAL(reguCARD.get_suit(trump), DIAMONDS);
}

TEST(test_cardless_ledsuit) {
    Suit trump = DIAMONDS;
    Card ledCard(NINE, SPADES);
    Card sameSUITAsLed(TWO, SPADES);
    Card sameNumAsLed(NINE, CLUBS);

    ASSERT_FALSE(Card_less(sameSUITAsLed, ledCard, ledCard, trump));
    ASSERT_TRUE(Card_less(sameNumAsLed, ledCard, ledCard, trump));
}


TEST(test_card_less_bowers) {
    Suit trump = SPADES;
    Card rightBower(JACK, SPADES);
    Card leftBower(JACK, CLUBS); 
    Card sameColorAsTrump(FOUR, SPADES);
    Card difference(ACE, DIAMONDS);
    ASSERT_TRUE(Card_less(difference, sameColorAsTrump, trump));
    ASSERT_TRUE(Card_less(leftBower, rightBower, trump));
    ASSERT_TRUE(Card_less(sameColorAsTrump, rightBower, trump));
    ASSERT_TRUE(Card_less(sameColorAsTrump, leftBower, trump));

    ASSERT_FALSE(Card_less(rightBower, leftBower, trump));
    ASSERT_FALSE(Card_less(rightBower, sameColorAsTrump, trump));
    ASSERT_FALSE(Card_less(leftBower, sameColorAsTrump, trump));
}

TEST(test_left_bower_vs_nontrump_same_color) {
    Suit trump = SPADES;
    Card leftBower(JACK, CLUBS);    
    Card sameSuitAsTrump(THREE, SPADES);

    ASSERT_TRUE(Card_less(sameSuitAsTrump, leftBower, trump));
    ASSERT_FALSE(Card_less(leftBower, sameSuitAsTrump, trump));
}

TEST_MAIN()
