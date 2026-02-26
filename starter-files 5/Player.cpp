#include "Player.hpp"
#include <vector>
#include <string>
#include <cassert>
#include <iostream>

class SimplePlayer : public Player {
  public:
    SimplePlayer(const std::string &name_in) : name{name_in} {}
    const std::string & get_name() const {
      return name;
    }

    void add_card(const Card &c) {
      assert(MAX_HAND_SIZE > allCards.size());
      allCards.push_back(c);
      std::sort(allCards.begin(), allCards.end());
    }

    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const {
      Suit trumpSuit = upcard.get_suit(); 
      if(round == 1) {
        int faceCheckCounter = 0;
        for(size_t i = 0; i < allCards.size(); ++i) {
          if(allCards[i].get_suit() == trumpSuit && allCards[i].is_face_or_ace()) {
            faceCheckCounter++;
          }
        }
        if(faceCheckCounter >= 2) {
          order_up_suit = trumpSuit;
          return true;
        } else {
          return false;
        }
      } else {
        int faceOrAceCheckCounterTwo = 0;
        Suit sameColor = Suit_next(trumpSuit); 
        for(size_t i = 0; i < allCards.size(); ++i) {
          if(allCards[i].get_suit() == sameColor && allCards[i].is_face_or_ace()) {
            faceOrAceCheckCounterTwo++;
          }
        } 
        if(faceOrAceCheckCounterTwo >= 1) {
          order_up_suit = sameColor;
          return true;
        } else if(is_dealer == true) {
          order_up_suit = sameColor;
          return true;
        } else {return false;}
        
      }
    }

    void add_and_discard(const Card &upcard) {
      assert(allCards.size() >= 1);
      allCards.push_back(upcard);
      int worstCardIndex = 0;
      Suit trumpSuit = upcard.get_suit();
      for(size_t i = 1; i < allCards.size(); ++i) {
        if(Card_less(allCards[i], allCards[worstCardIndex], trumpSuit)) {
          worstCardIndex = i;
        }
      }
      allCards.erase(allCards.begin() + worstCardIndex);
      std::sort(allCards.begin(), allCards.end());
    }

    Card lead_card(Suit trump) {
        int ind = -1;
        for (size_t i = 0; i < allCards.size(); i++) {
            if (!allCards[i].is_trump(trump)) {
                if (ind == -1 || Card_less(allCards[ind], allCards[i], trump)) {
                    ind = i;
                }
            }
        }
        if (ind == -1) {
            ind = 0;
            for (size_t i = 1; i < allCards.size(); ++i) {
                if (Card_less(allCards[ind], allCards[i], trump)) {
                    ind = i;
                }
            }
        }
        
        Card led = allCards[ind];
        allCards.erase(allCards.begin() + ind);
        return led;
    }

    Card play_card(const Card &led_card, Suit trump) {
        int ind = -1;
        Suit led_suit = led_card.get_suit(trump);

        for (size_t i = 0; i < allCards.size(); i++){
            if (allCards[i].get_suit() == led_suit){
                if (ind == -1 || Card_less(allCards[ind],allCards[i],led_card,trump)){
                    ind = i;
                }
            }
        }
        
        if (ind == -1){
            ind = 0;
            for (size_t i = 1; i < allCards.size(); i++){
                if (Card_less(allCards[i],allCards[ind],led_card,trump)){
                    ind = i;
                }
            }
        }
        
    
        Card play = allCards[ind];
        allCards.erase(allCards.begin() + ind);
        return play;
    }
    
  private:
    std::string name;
    std::vector<Card> allCards;
};

class HumanPlayer : public Player {
  public:
    HumanPlayer(const std::string &name_in) : name(name_in) {}

    const std::string & get_name() const {
        return name;
    }

    void add_card(const Card &c) {
      assert(MAX_HAND_SIZE > allCards.size());
      allCards.push_back(c);
    }

    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const {
      print_hand();
      std::cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
      std::string decision;
      std::cin >> decision;
      if(decision != "pass") {
        order_up_suit = string_to_suit(decision);
        return true;
      } else {
        return false;
      }
    }

    void add_and_discard(const Card &upcard)  {
      allCards.push_back(upcard);
      std::sort(allCards.begin(), allCards.end());
      print_hand();
      std::cout << "Discard upcard: [-1]\n";
      std::cout << "Human player " << name << ", please select a card to discard:\n";
      int chosenIndex;
      std::cin >> chosenIndex;
      if(chosenIndex == -1) {
        allCards.pop_back();
      } else {
        allCards.erase(allCards.begin() + chosenIndex);
      }

    }

    Card lead_card(Suit trump)  {
        print_hand();
        std::cout << "Human player " << name << ", please select a card:\n";
        int crd;
        std::cin >> crd;
        Card chosenCrd = allCards[crd];
        allCards.erase(allCards.begin() + crd);
        return chosenCrd;
    }

    Card play_card(const Card &led_card, Suit trump)  {
        print_hand();
        std::cout << "Human player " << name << ", please select a card:\n";
        int crd;
        std::cin >> crd;
        Card chosenCrd = allCards[crd];
        allCards.erase(allCards.begin() + crd);
        return chosenCrd;
    }
    
  private:
    std::string name;
    std::vector<Card> allCards;
    void print_hand() const {
        for (size_t i=0; i < allCards.size(); ++i){
            std::cout << "Human player " << name << "'s hand: "
                << "[" << i << "] " << allCards[i] << "\n";
        }
    }
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