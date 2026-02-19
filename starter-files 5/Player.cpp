#include "Player.hpp"
#include <vector>
#include <string>
#include <cassert>
#include <iostream>

class SimplePlayer : public Player {
  public:
    SimplePlayer(const std::string &name_in) : name{name_in} {}
    const std::string & SimplePlayer::get_name() const override {
      return name;
    }

    void SimplePlayer::add_card(const Card &c) override {
      assert(MAX_HAND_SIZE > allCards.size());
      allCards.push_back(c);
    }

    bool SimplePlayer::make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override {
      return false;
    }

    void SimplePlayer::add_and_discard(const Card &upcard) override {}

    Card SimplePlayer::lead_card(Suit trump) override {
     
      return allCards[0];
    }

    Card SimplePlayer::play_card(const Card &led_card, Suit trump) override {
       
      return allCards[0];
    }

  private:
    std::string name;
    std::vector<Card> allCards;
};

class HumanPlayer : public Player {
  public:
    HumanPlayer(const std::string &name_in) : name(name_in) {}

    const std::string & get_name() const override {
        return name;
    }

    void add_card(const Card &c) override {
      assert(MAX_HAND_SIZE > allCards.size());
      allCards.push_back(c);
    }

    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override {
      return false;
    }

    void add_and_discard(const Card &upcard) override {}

    Card lead_card(Suit trump) override {
      return allCards[0];
    }

    Card play_card(const Card &led_card, Suit trump) override {
      return allCards[0];
    }

  private:
    std::string name;
    std::vector<Card> allCards;
};


//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
Player * Player_factory(const std::string &name, const std::string &strategy) {
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
  } else if(strategy == "Human") {
    return new HumanPlayer(name);
  }
  assert(false);
  return nullptr;
}

//EFFECTS: Prints player's name to os
std::ostream & operator<<(std::ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}