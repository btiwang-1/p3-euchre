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
    ASSERT_TRUE(c1 <= c2);
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
TEST(test_get_suit_left_bower) {
    Suit trump = DIAMONDS;
    Card leftBower(JACK, HEARTS); 
    ASSERT_EQUAL(leftBower.get_suit(trump), DIAMONDS);
    
    Card nCard(ACE, HEARTS);
    ASSERT_EQUAL(nCard.get_suit(trump), HEARTS);
}
TEST(test_card_less_left_bower_as_led_suit) {
    Suit trump = DIAMONDS;
    Card led_card(ACE, HEARTS);
    Card leftBower(JACK, HEARTS); 
    Card ace_hearts(ACE, HEARTS);  

    ASSERT_FALSE(Card_less(leftBower, ace_hearts, led_card, trump));
    ASSERT_TRUE(Card_less(ace_hearts, leftBower, led_card, trump));
}
TEST(test_suit_tie_break) {
    Card nineOfDia(NINE, DIAMONDS);
    Card nineOfClu(NINE, CLUBS);
    Card nineOfHea(NINE, HEARTS);
    Card nineOfSpa(NINE, SPADES);

    ASSERT_TRUE(nineOfClu < nineOfDia);
    ASSERT_TRUE(nineOfHea < nineOfClu);
    ASSERT_TRUE(nineOfSpa < nineOfHea);
}
TEST(test_left_bower_vs_nontrump_same_color) {
    Suit trump = SPADES;
    Card leftBower(JACK, CLUBS);    
    Card sameSuitAsTrump(THREE, SPADES);

    ASSERT_TRUE(Card_less(sameSuitAsTrump, leftBower, trump));
    ASSERT_FALSE(Card_less(leftBower, sameSuitAsTrump, trump));
}
TEST(test_led_suit_vs_cross_suit) {
    Suit trump = CLUBS;
    Card led_card(NINE, HEARTS);
    Card cross_suit(ACE, SPADES);
    Card led_suit_card(TWO, HEARTS);

    ASSERT_FALSE(Card_less(led_suit_card, cross_suit, led_card, trump));
    ASSERT_TRUE(Card_less(cross_suit, led_suit_card, led_card, trump));
}
TEST(test_right_bower_vs_left_bower) {
    Suit trump = HEARTS;
    Card rightBower(JACK, HEARTS);
    Card leftBower(JACK, DIAMONDS);
    ASSERT_FALSE(Card_less(rightBower, leftBower, trump));
    ASSERT_TRUE(Card_less(leftBower, rightBower, trump));
}

TEST(test_is_right_bower_false_for_left_bower) {
    Suit trump = CLUBS;
    Card leftBower(JACK, SPADES);
    ASSERT_FALSE(leftBower.is_right_bower(trump));
}

TEST(test_get_suit_next_suit_not_jack) {
    Suit trump = DIAMONDS;
    Card heartsAce(ACE, HEARTS); 
    ASSERT_EQUAL(heartsAce.get_suit(trump), HEARTS);
}

TEST(test_led_suit_rank_vs_cross_suit_ace) {
    Suit trump = SPADES;
    Card ledCard(NINE, HEARTS);
    Card crossAce(ACE, CLUBS);
    Card ledNine(NINE, HEARTS);
    ASSERT_FALSE(Card_less(ledNine, crossAce, ledCard, trump));
    ASSERT_TRUE(Card_less(crossAce, ledNine, ledCard, trump));
}

TEST(test_card_less_trump_vs_led_ace) {
    Suit trump = HEARTS;
    Card ledCard(ACE, SPADES);
    Card trumpNine(NINE, HEARTS);
    Card ledAce(ACE, SPADES);
    ASSERT_FALSE(Card_less(trumpNine, ledAce, ledCard, trump));
    ASSERT_TRUE(Card_less(ledAce, trumpNine, ledCard, trump));
}
TEST(test_all_is_face_or_ace) {
    Card jack(JACK, SPADES);
    Card queen(QUEEN, HEARTS);
    Card king(KING, CLUBS);
    Card ace(ACE, DIAMONDS);
    Card ten(TEN, SPADES);
    Card two(TWO, HEARTS);

    ASSERT_TRUE(jack.is_face_or_ace());
    ASSERT_TRUE(queen.is_face_or_ace());
    ASSERT_TRUE(king.is_face_or_ace());
    ASSERT_TRUE(ace.is_face_or_ace());

    ASSERT_FALSE(ten.is_face_or_ace());
    ASSERT_FALSE(two.is_face_or_ace());
}
TEST_MAIN()
