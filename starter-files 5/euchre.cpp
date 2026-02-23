#include <iostream>
#include <string>
#include <sstring>
#include "Player.hpp"
#include "Card.hpp"
#include "Pack.hpp"


using namespace std;


/*
    How the game will work:
        a. SETUP THE TABLE
            TEAMS:
                Players 0 and 2 are TEAM ONE
                Players 1 and 3 are TEAM TWO
            i. Shuffle
                - Call Pack::shuffle()
                * If shuffling is disabled, call Pack::reset() anytime shuffling would be called
            ii. Deal
                - Call Player::add_card(Pack::dealOne()) for every card you want to give a player, *It returns a Card*
                - Remember 3-2-3-2 and then 2-3-2-3; Each player gets 5 cards in the end.

                - Four cards remain in the deck after the deal. 
                    The NEXT card in the pack is called the UPCARD 
                    (it is turned face up, while the other cards are all face down)
        b. MAKING TRUMP
            i. Round One
                (Yo who is the dealer)
                - Ordering Up! Starts with the Player to the left of the dealer
                    and goes around the circle to see if anyone wants to order up or pass
                - Should someone order up, the suit of the UPCARD becomes the TRUMP SUIT (Use Player::make_trump())
                - The dealer has the opportunity to replace one of their cards with the UPCARD
            ii. Round Two
                - If all players pass the first round, We do another making
                - The UPCARD's suit is rejected and cannot be ordered up. Instead, 
                    the players may order up any suit other than the UPCARD's suit.
                    (Using the same rotation as round one starting at the eldest hand)
                - If making reaches the dealer during the second round, a variant called 
                    screw the dealer is invoked: the dealer must order up a suit other than 
                    the rejected suit.

        c. TRICK TAKING
            - Once the trump is determined, we go through FIVE tricks: 
            Trick: Players take turns laying down cards, highest card wins the trick (Use Card::Card_less)
            - Person who plays first becomes LEADER
            * For the 1st Trick, the ELDEST HAND leads

            TRICKS:
                1. Leader leads any of their cards (the suit off that card becomes the led_suit)
                2. Each player must follow suit if they can but must put something down regardless
                3. Play moves to the left around the table, with each player playing one card.
            - The winner of the trick leads the next trick.
        d. SCORING
            i. If the team that ordered up the trump suit takes 3 or 4 tricks, they get 1 point.
            ii. If the team that ordered up the trump suit takes all 5 tricks, they get 2 points. This is called a MARCH.
            iii. If the team that did not order up takes 3, 4, or 5 tricks, they receive 2 points. This is called EUCHRED.
*/

class Team(){
    public:
    Team(Player* p1, Player* p2) : PLRONE(p1), PLRTWO(p2){}

    bool ordered_up(){ return odup; }
    void order_up(bool &yon){ //yon = yes or no; odup = orderup
        odup = yon;
    }
    int get_points();
    void won_trick(){ tricks_won++; }
    void set_points(int pts);
    void reset();
    int total_points();
    int calculate();

    private:
    Player* PLRONE;
    Player* PLRTWO;
    bool odup = false;
    int points;
    int total;
    int tricks_won;
};
class Game(){
    public:
    Game();

    

    Game(Player p1, Player p2, Player p3, Player p4, Pack pack, int pts_to_win) :
     pts_to_win(pts_to_win), players.push_back(p1), players.push_back(p2), 
     players.push_back(p3), players.push_back(p4), pack(pack){

    }
    
    void play();

    private:
    std::vector<Player*> players;
    int pts_to_win = 1;
    Pack pack;
    void shuffle();
    void deal();
    void make_trump();
    void play_hand();
}


int main(int argc, char *argv[]) {
    // check if there are four arguments in the command line
    // and print a helpful message
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }

    // create a file stream for the input file
    ifstream isn(argv[1]);
    if (!isn.is_open()) {
        cout << "Error opening file " << argv[1] << endl;
        return 1;
    }
}