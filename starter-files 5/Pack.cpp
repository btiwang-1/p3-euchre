#include <iostream>
#include <array>
#include <string>
#include "Pack.hpp"
#include "Card.hpp"
using namespace std;

  // EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on. 
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack(){
    for (int i = 0; i <= DIAMONDS; i++){
        for (int j = 7; j <= ACE; j++){
            Card newCard(static_cast<Rank>(j),static_cast<Suit>(i));
            cards[(j-7)*i+(j-7)] = newCard;
        }
    }
  }

  // REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack(std::istream& pack_input){
    string o; 
    string newlin;
    for (int i = 0; i < PACK_SIZE; i++){
        int rank;
        int suit; 
        pack_input >> rank >> o >> suit >> newlin;
        cards[i] = Card(static_cast<Rank>(rank), static_cast<Suit>(suit));
    }
  }
  
  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
  Card Pack::deal_one(){
    return cards[next++]; 
  }

  // EFFECTS: Resets next index to first card in the Pack
  void Pack::reset(){
    next = 0;
  }

  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
  void Pack::shuffle() {
    std::array<Card, PACK_SIZE> shuffledCards;
    for(size_t i = 0; i < 7; ++i) {
      for(size_t j = 0; j < PACK_SIZE / 2; j++) {
        shuffledCards[2* j] = cards[j + PACK_SIZE / 2];
        shuffledCards[2*j + 1] = cards[j];

      }
      cards=shuffledCards;
    }
    reset();
  }

  // EFFECTS: returns true if there are no more cards left in the pack
  bool Pack::empty() const{
    return (next == PACK_SIZE);
  }