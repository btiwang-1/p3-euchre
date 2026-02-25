#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "Player.hpp"
#include "Card.hpp"
#include "Pack.hpp"


using namespace std;


class Team {
    public:
    Team(Player* p1, Player* p2) : PLRONE(p1), PLRTWO(p2), odup(false), 
        points(0), total(0), tricks_won(0){}

    bool ordered_up() const { return odup; }
    void order_up(bool &yon){ //yon = yes or no; odup = orderup
        odup = yon;
    }
    Player* get_playerone() const { return PLRONE; }
    Player* get_playertwo() const { return PLRTWO; }
    int get_points() const { return points; }
    int get_tricks() const { return tricks_won; }
    void won_trick(){ tricks_won++; }
    void set_points(int pts){ points = pts; }
    void reset(){ 
        total += get_points();
        set_points(0);
        tricks_won = 0; 
        odup = false;
    }
    int total_points() const { return total; };
    int calculate() {
        if (odup){
            if (tricks_won > 3 && tricks_won < 5){
                set_points(1);
                return 1;
            } else if (tricks_won == 5){
                set_points(2);
                return 2;
            }
        } else {
            if (tricks_won > 3){
                set_points(2);
                return 2;
            }
        }
        return 0;
    }

    private:
    Player* PLRONE;
    Player* PLRTWO;
    bool odup;
    int points;
    int total;
    int tricks_won;
};

class Game {
    public:

    Game(vector<Player*> plyrs, Pack pack, bool shuff, int pts_to_win) :
        hand_num(0),                 
        isShuffles(shuff),           
        t1(plyrs[0], plyrs[2]),      
        t2(plyrs[1], plyrs[3]),      
        pts_to_win(pts_to_win),      
        gamepack(pack){
        for (size_t i = 0; i < plyrs.size(); i++){
            players.push_back(plyrs[i]);
        }
        dealerIndex = 0;
    }
    
    void play(int *gameEnd, istream& isme) {
        if (winner == nullptr){
            play_hand(&t1, &t2, gameEnd, isme);
        } else {
            *gameEnd = 1;
            // Player A and Player B win!
            cout << winner->get_playerone()->get_name() << " " <<
                winner->get_playertwo()->get_name() << " win!";
        }
    }

    private:
    int whatNumberOfTrickIndex;
    int hand_num = 0;
    std::vector<Player*> players;
    bool isShuffles = false;
    int NUM_TRICKS = 5;
    Team t1;
    Team t2;
    Team* winner == nullptr; 
    Suit trump;
    Player* currentLeader;
    int dealerIndex;
    int pts_to_win = 1;
    Pack gamepack;
    Card upcard;
    void shuffle(bool &isShuffle){
        // call pack.shuffle()
        if (isShuffle){
            gamepack.shuffle();
        } else {
            gamepack.reset();
        }
    }
    void add_up(int index, int times){
        for (int j = 0; j < times; j++){
            players[index]->add_card(gamepack.deal_one());
        }
    }
    void deal(){
        // call add_card
        // use the 3-2-3-2 sequence followed by the 2-3-2-3 sequence
        
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
        upcard = gamepack.deal_one();
    }
    void print();
    void make_trump(istream& isme){ // use istream?
        int round = 1;
        string decision;
        int starterIndex;
        if (dealerIndex < 3){
            starterIndex = dealerIndex+1;
        } else {
            starterIndex = 0;
        }
        for (size_t i = starterIndex; i < players.size() + starterIndex; i++){
            isme >> decision;
            if (decision != "pass"){
                trump = upcard.get_suit();
                if (i < 3){
                    players[i]->make_trump(upcard, (i==dealerIndex), round, trump);
                    cout << players[i]->get_name() << " orders up " << trump << endl; 
                } else {
                    players[i-starterIndex]->make_trump(upcard, (i==dealerIndex), round, trump);
                    cout << players[i-starterIndex]->get_name() << " orders up " << trump << endl;
                }
                cout << "\n";
                return;
            }
            // Use this somehow player[i-starterIndex]->
        }
        round++;
        // Round 2
        for (size_t i = starterIndex; i < players.size() + starterIndex; i++){
            isme >> decision;
            if (i != dealerIndex){
                if (decision != "pass"){
                    trump = string_to_suit(decision);
                    if (i < 3){
                        players[i]->make_trump(upcard, false, round, trump);
                        cout << players[i]->get_name() << " orders up " << trump << endl;
                    } else {
                        players[i-starterIndex]->make_trump(upcard, false, round, trump);
                        cout << players[i-starterIndex]->get_name() << " orders up " << trump << endl;
                    } 
                    cout << "\n";
                    return;
                }
                if (i < 3){
                    cout << players[i]->get_name() << " passes" << endl;
                } else {
                    cout << players[i-starterIndex]->get_name() << " passes" << endl;
                }
            } else {
                trump = string_to_suit(decision);
                // Screw the dealer!
                players[dealerIndex]->make_trump(upcard, true, round, trump);
            }
        }
    } 
    void print_handwinner(bool odup, Player* p1, Player* p2){
        string winningTeam = p1->get_name() + " and " + p2->get_name();
        cout << winningTeam << " win the hand" << endl;
        if (players[0] == p1){
            if (t1.ordered_up()){
                if(t1.get_points() == 2){
                    cout << "march!" << endl;
                } 
            } else if (t1.get_points() == 2){
                cout << "euchred!" << endl;
            }
        } else {
            if (t2.ordered_up()){
                if(t2.get_points() == 2){
                    cout << "march!" << endl;
                } 
            } else if (t2.get_points() == 2){
                cout << "euchred!" << endl;
            }
        }
    }

    void play_hand(Team *tm1, Team *tm2, int *endgame, istream& isme){
        /*
        Shuffle 
        Deal
        Make_trump
        Play tricks (five times)
        Score
        */
        
        shuffle(isShuffles);
        deal();
        cout << "Hand " << hand_num << "\n";
        cout << players[dealerIndex] << "deals\n";
        cout << upcard << "turned up" << endl;
        make_trump(isme);
        // Play each trick. Still not sure if we should have a separate function for playing tricks, 
        // I think we should though and have play be the master function that runs play_hand and prints 
        // everything to the output stream in a while loop in main until a team wins and then we make a break function
        for (size_t i = 0; i < NUM_TRICKS; i++){
            play_trick();
        }
        if (tm1->calculate() > tm2->calculate()){
            // print that the players in team 1 won that round
            print_handwinner(tm1->ordered_up(), tm1->get_playerone(), tm1->get_playertwo());
            // if team 1 meets or exceeds points to win then end game
            if (tm1->total_points() >= pts_to_win){
                winner = &t1;
                *endgame = 1;
            }
        } else {
            // print that the players in team 2 won that round
            print_handwinner(tm2->ordered_up(), tm2->get_playerone(), tm2->get_playertwo());
            // if team 2 meets or exceeds points to win then end game
            if (tm2->total_points() >= pts_to_win){
                winner = &t2;
                *endgame = 1;
            }
        }
        if (dealerIndex < 3){
            dealerIndex++;
        } else {
            dealerIndex = 0;
        }
        t1.reset();
        t2.reset();
        hand_num++;
    } // REMEMBER: each hand, rotate the dealer 1 to the left; Eldest Hand leads the first trick
    void play_trick(){
        int startingIndexOfPlayers; // eldest hand
        if(whatNumberOfTrickIndex == 0) {
            
            if(dealerIndex + 1 < 4) {
                startingIndexOfPlayers = dealerIndex + 1;
            } else {
                startingIndexOfPlayers = 0;
            }
            currentLeader = players[startingIndexOfPlayers];
        } else {
            startingIndexOfPlayers = find(players.begin(), players.end(), currentLeader) - players.begin();
        }
        int currentHighestIndex = startingIndexOfPlayers;
        Card led_card = currentLeader->lead_card(trump);
        Card highest_Card = led_card;

        if(startingIndexOfPlayers == 3) {
            startingIndexOfPlayers = 0;
        } else {
            startingIndexOfPlayers++;
        }
        // playing cards
        for(size_t i = 1; i < 4; ++i) {
            if(startingIndexOfPlayers != 0) {
                // MAKE CARD VAR = TO CURRENTLEADER.LEADCARD() *STORES THE LED_CARD
                Card playedCard = players[startingIndexOfPlayers]->play_card(led_card, trump);
                if(Card_less(highest_Card, playedCard, led_card, trump)) {
                    highest_Card = playedCard;
                    currentHighestIndex = startingIndexOfPlayers;
                }
                startingIndexOfPlayers++;
                if(startingIndexOfPlayers == 4) {
                    startingIndexOfPlayers = 0;
                }
            } else {
                Card playedCard = players[startingIndexOfPlayers]->play_card(led_card, trump);
                if(Card_less(highest_Card, playedCard, led_card, trump)) {
                    currentHighestIndex = startingIndexOfPlayers;
                    highest_Card = playedCard;
                }
                startingIndexOfPlayers++;
            }
        }
        currentLeader = players[currentHighestIndex];
        if (currentHighestIndex % 2 == 0){
            t1.won_trick();
        } else {
            t2.won_trick();
        }
        whatNumberOfTrickIndex++; 
    }
};


int main(int argc, char *argv[]) {
    int gend = -1;
    int *game_end = &gend;
    int NUM_ARGS = 12;
    int PLR_ARG_START = 4;
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
    for (int i = PLR_ARG_START; i < NUM_ARGS - 1; i+=2){
        plrs.push_back(Player_factory(argv[i],argv[i+1]));
    }

    // create a file stream for the input file
    ifstream isn(argv[1]);
    if (!isn.is_open()) {
        cout << "Error opening file " << argv[1] << endl;
        return 1;
    }
    // Create Game
    Game euchreGame(plrs, Pack(isn), (string(argv[2]) == "shuffle"), stoi(argv[3]));
    while (gend < 0){
        euchreGame.play(game_end, isn);
    }
}
