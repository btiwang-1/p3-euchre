#include <iostream>
#include <fstream>
#include <string>
#include "Player.hpp"
#include "Card.hpp"
#include "Pack.hpp"


using namespace std;


/*
    How the game will work:
        a. SETUP THE TABLE
            DEALER: 
                In each hand, one player is designated as the dealer 
                (if humans were playing the game, the one who passes out the cards). 
                In our game, player 0 deals during the first hand. Each subsequent hand, 
                the role of dealer moves one player to the left.


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
    Player* get_playerone(){ return PLRONE; }
    Player* get_playertwo(){ return PLRTWO; }
    int get_points(){ return points; }
    int get_tricks() { return tricks_won; }
    void won_trick(){ tricks_won++; }
    void set_points(int &pts){ points = pts; }
    void reset(){ 
        total += get_points();
        set_points(0);
        tricks_won = 0; 
        odup = false;
    }
    int total_points() { return total; };
    int calculate(){
        if (odup){
            if (tricks_won > 3 && tricks_won < 5){
                return 1;
            } else if (tricks_won == 5){
                return 2;
            }
        } else {
            if (tricks_won > 3){
                return 2;
            }
        }
        return 0;
    }

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

    Game(vector<Player*> plyrs, Pack pack, bool shuff, int pts_to_win) :
     pts_to_win(pts_to_win), pack(pack), isShuffles(shuff){
        for (size_t i = 0; i < plyrs.size(); i++){
            players.push_back(plyrs[i]);
        }
        Team t1(players[0], players[1]);
        Team t2(players[2], players[3]);
        dealer = players[0];
    }
    
    void play() {
        int startingIndexOfPlayers;
        if(whatNumberOfTrickIndex == 0) {
            startingIndexOfPlayers = players.find(players.begin(), players.end(), dealer - 1);
            if(startingIndexOfPlayers < 0) { 
                startingIndexOfPlayers = 3;
            }
        } else {
            
        }

        for(size_t i = 1; i < 4; ++i) {

        }
        
    }

    private:
    int whatNumberOfTrickIndex;
    std::vector<Player*> players;
    bool isShuffles = false;
    int NUM_TRICKS = 5;
    Team* winner; 
    Player* dealer;
    int dealerIndex;
    int pts_to_win = 1;
    Pack pack;
    Card upcard;
    void shuffle(bool isShuffle){
        // call pack.shuffle()
        if (isShuffle){
            pack.shuffle();
        } else {
            pack.reset();
        }
    }
    void deal(){
        // call add_card
        // use the 3-2-3-2 sequence followed by the 2-3-2-3 sequence
        void add_up(int index, int times){
            for (int j = 0; j < times; j++){
                players[index].add_card(pack.deal_one());
            }
        }
        for (size_t i = 0; i < players.size(); i++){
            if (i % 2 == 0){
                add_up(i,3); 
            } else {
                add_up(i,2);
            }
        }
        for (size_t i = 0; i < players.size(); i++){
            if (i % 2 == 0){
                add_up(i,2); 
            } else {
                add_up(i,3);
            }
        }
        upcard = pack.deal_one();
    }
    void print();
    void make_trump(){ // use istream?
        int round = 1;
        string decision;
        dealerIndex = find(players.begin(), players.end(), dealer);
        Player* starter;
        if (dealerIndex < 3){
            starter = players[dealerIndex+1];
        } else {
            starter = players[0];
        }
        int starterIndex = find(players.begin(), players.end(), starter);
        for (size_t i = starterIndex; i < players.size() + starterIndex; i++){
            cin >> decision;
            if (decision != "pass"){
                if (i < 3){
                    player[i].make_trump(upcard, (i==dealerIndex), round, upcard.get_suit());
                } else {
                    player[i-starterIndex].make_trump(upcard, (i==dealerIndex), round, upcard.get_suit());
                }
                return;
            }
        }
        round++;
        // Round 2
        for (size_t i = starterIndex; i < players.size() + starterIndex; i++){
            cin >> decision;
            if (i != dealerIndex){
                if (decision != "pass"){
                    if (i < 3){
                        player[i].make_trump(upcard, false, round, string_to_suit(decision));
                    } else {
                        player[i-starterIndex].make_trump(upcard, false, round, string_to_suit(decision));
                    } 
                    return;
                }
            } else {
                // Screw the dealer!
                player[dealerIndex].make_trump(upcard, true, round, string_to_suit(decision));
            }
        }
    } 
    void play_hand(Team &tm1, Team &tm2){
        /*
        Shuffle 
        Deal
        Make_trump
        Play tricks (five times)
        Score
        */

        shuffle(isShuffles);
        deal();
        make_trump();
        // Play each trick. Still not sure if we should have a separate function for playing tricks, 
        // I think we should though and have play be the master function that runs play_hand and prints 
        // everything to the output stream in a while loop in main until a team wins and then we make a break function
        for (size_t i = 0; i < NUM_TRICKS; i++){
            play_trick();
        }
        if (tm1.calculate() > tm2.calculate()){
            // print that the players in team 1 won that round
            // if team 1 meets or exceeds points to win then end game
            if (tm1.total_points() >= pts_to_win){
                // end game
            }
        } else {
            // print that the players in team 2 won that round
            // if team 2 meets or exceeds points to win then end game
            if (tm2.total_points() >= pts_to_win){
                // end game
            }
        }
        if (dealerIndex < 3){
            dealerIndex++;
        } else {
            dealerIndex = 0;
        }
    } // REMEMBER: each hand, rotate the dealer 1 to the left; Eldest Hand leads the first trick
    void play_trick();
}


int main(int argc, char *argv[]) {
    bool game_end = false;
    int NUM_ARGS = 12
    int PLR_ARG_START = 4
    vector<Player*> plrs; 
    // check if there are four arguments in the command line
    // and print a helpful message
    if (argc != NUM_ARGS) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }
    // initialize Players vector
    for (int i = PLR_ARG_START; i < NUM_ARGS - 1; i+2){
        if (argv[i+1] == "Simple"){
            plrs.push_back(SimplePlayer(argv[i]));
        } else {
            plrs.push_back(HumanPlayer(argv[i]));
        }
    }

    // create a file stream for the input file
    ifstream isn(argv[1]);
    if (!isn.is_open()) {
        cout << "Error opening file " << argv[1] << endl;
        return 1;
    }
    // Create Game
    Game euchreGame(plrs, Pack(isn), (argv[2] == "shuffle"), static_cast<int>(argv[3]));
    while (!game_end){
        euchreGame.play();
    }
}