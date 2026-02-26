#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

// Test Cases for Simple Player
TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());
    delete alice;
}

TEST(test_player_add_card_and_lead) {
    Player * p = Player_factory("Simpleton", "Simple");
    Card c(ACE, CLUBS);
    p->add_card(c);
    ASSERT_EQUAL(c, p->lead_card(HEARTS)); 
    delete p;
}

TEST(test_simple_player_leads_highest_non_trump) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(ACE, CLUBS));
    p->add_card(Card(KING, CLUBS));
    Card led = p->lead_card(HEARTS);  
    ASSERT_EQUAL(Card(ACE, CLUBS), led);
    Card next = p->lead_card(HEARTS);
    ASSERT_EQUAL(Card(KING, CLUBS), next);
    delete p;
}

TEST(test_simple_player_lead_prefers_non_trump) {
    Player * p = Player_factory("Alice", "Simple");
    Card trump(ACE, HEARTS);
    Card nonTrump(KING, CLUBS);
    p->add_card(trump);
    p->add_card(nonTrump);
    Card led = p->lead_card(HEARTS);
    ASSERT_EQUAL(nonTrump, led);
    delete p;
}

TEST(test_simple_player_lead_only_trump) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(ACE, HEARTS));
    Card led = p->lead_card(HEARTS);
    ASSERT_EQUAL(Card(ACE, HEARTS), led); 
    delete p;
}

TEST(test_simple_player_play_follows_suit) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(ACE, CLUBS));
    p->add_card(Card(QUEEN, CLUBS));
    p->add_card(Card(TEN, HEARTS));
    Card led(KING, CLUBS);
    Card play = p->play_card(led, HEARTS);
    ASSERT_EQUAL(Card(ACE, CLUBS), play); 
    delete p;
}

TEST(test_simple_player_make_trump_r2_same_color) {
    Player * p = Player_factory("Bob", "Simple");
    p->add_card(Card(JACK, SPADES));
    Suit order;
    ASSERT_TRUE(p->make_trump(Card(NINE, CLUBS), false, 2, order));
    ASSERT_EQUAL(SPADES, order);
    delete p;
}

TEST(test_simple_player_play_single_card) {
    Player * p = Player_factory("Alice", "Simple");
    Card only(ACE, CLUBS);
    p->add_card(only);
    Card led(KING, HEARTS);
    Card play = p->play_card(led, HEARTS);
    ASSERT_EQUAL(only, play); 
    delete p;
}


TEST(test_simple_player_make_trump_r1_true) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(JACK, CLUBS));
    p->add_card(Card(QUEEN, CLUBS));
    Suit order;
    ASSERT_TRUE(p->make_trump(Card(NINE, CLUBS), false, 1, order));
    ASSERT_EQUAL(CLUBS, order);
    delete p;
}

TEST(test_simple_player_make_trump_r1_false) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(NINE, CLUBS));
    p->add_card(Card(TEN, CLUBS));
    Suit order;
    ASSERT_FALSE(p->make_trump(Card(NINE, CLUBS), false, 1, order));
    delete p;
}

TEST(test_simple_player_make_trump_r2_dealer) {
    Player * p = Player_factory("Alice", "Simple");
    Card upcard(NINE, DIAMONDS);
    Suit order_up;
    ASSERT_TRUE(p->make_trump(upcard, true, 2, order_up));
    ASSERT_EQUAL(HEARTS, order_up);

    delete p;

}

TEST(test_simple_player_make_trump_r2_fail) {
    Player * p = Player_factory("Alice", "Simple");
    Suit order;
    ASSERT_FALSE(p->make_trump(Card(NINE, CLUBS), false, 2, order));
    delete p;
}/*
TEST(test_simple_player_add_and_discard) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(NINE, CLUBS));    
    p->add_card(Card(TEN, CLUBS));    
    p->add_card(Card(JACK, CLUBS));    
    p->add_card(Card(QUEEN, CLUBS));    
    p->add_card(Card(KING, CLUBS));    

    Card upcard(NINE, SPADES);
    p->add_and_discard(upcard);

    ASSERT_EQUAL(Card(TEN, CLUBS), p->lead_card(SPADES));
    ASSERT_EQUAL(Card(JACK, CLUBS), p->lead_card(SPADES));
    ASSERT_EQUAL(Card(QUEEN, CLUBS), p->lead_card(SPADES));
    ASSERT_EQUAL(Card(KING, CLUBS), p->lead_card(SPADES));
    ASSERT_EQUAL(Card(NINE, SPADES), p->lead_card(SPADES));

    delete p;
}*/
TEST(test_simple_player_add_and_discard) {
    Player * p = Player_factory("Alice", "Simple");

    p->add_card(Card(NINE, CLUBS));
    p->add_card(Card(JACK, CLUBS));
    p->add_card(Card(ACE, HEARTS));
    Card upcard(TEN, DIAMONDS);
    p->add_and_discard(upcard);

    Card c1 = p->lead_card(HEARTS);
    Card c2 = p->lead_card(HEARTS);
    Card c3 = p->lead_card(HEARTS);

    ASSERT_NOT_EQUAL(Card(NINE, CLUBS), c1);
    ASSERT_NOT_EQUAL(Card(NINE, CLUBS), c2);
    ASSERT_NOT_EQUAL(Card(NINE, CLUBS), c3);
    delete p;
}
TEST(test_simple_player_does_not_follow_with_left_bower) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(JACK, SPADES)); 
    p->add_card(Card(NINE, HEARTS)); 
    
    Card led(KING, CLUBS); 
    Suit trump = SPADES;   
    
    Card play = p->play_card(led, trump);
    ASSERT_EQUAL(Card(NINE, HEARTS), play); 
    delete p;
}
TEST(test_simple_player_lead_highest_trump_only) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(JACK, HEARTS)); 
    
    Card led = p->lead_card(HEARTS);
    ASSERT_EQUAL(Card(JACK, HEARTS), led);
    delete p;
}
TEST(test_simple_player_make_trump_r2_next_suit_bower) {
    Player * p = Player_factory("Bob", "Simple");
    p->add_card(Card(JACK, CLUBS));
    Suit order;
    ASSERT_TRUE(p->make_trump(Card(NINE, SPADES), false, 2, order));
    ASSERT_EQUAL(CLUBS, order);
    delete p;
}
TEST(test_simple_player_play_card_follow_suit_bower) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(JACK, HEARTS));
    p->add_card(Card(NINE, DIAMONDS));
    
    Card led(ACE, DIAMONDS);
    Suit trump = DIAMONDS;
    
    Card play = p->play_card(led, trump);
    ASSERT_EQUAL(Card(JACK, HEARTS), play);
    delete p;
}


TEST_MAIN()
