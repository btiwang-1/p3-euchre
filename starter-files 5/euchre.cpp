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
    Team(Player* p1, Player* p2) : 
        PLRONE(p1), PLRTWO(p2), odup(false), 
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
        if (odup) {
            if (tricks_won == 5) { points = 2; }
            else if (tricks_won >= 3) { points = 1; }
            else { points = 0; }
        } else {
            if (tricks_won >= 3) { points = 2; }
            else { points = 0; }
        }
        return points;
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

    Game(vector<Player*> plyrs, Pack pack, 
        bool shuff, int pts_to_win) :
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
            cout << winner->get_playerone()->get_name() << 
                " and " << winner->get_playertwo()->get_name() 
                << " win!";
        }
    }

    private:
    int whatNumberOfTrickIndex;
    int hand_num = 0;
    std::vector<Player*> players;
    bool isShuffles = false;
    const int NUM_TRICKS = 5;
    Team t1;
    Team t2;
    Team* winner = nullptr; 
    Suit trump;
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
        int startingDealerIndex = (dealerIndex + 1) % 4;
        for (size_t i = 0; i < players.size(); i++){
            int currentPlayer = (startingDealerIndex + i) % 4;
            if (i % 2 == 0){
                add_up(currentPlayer,3); 
            } else {
                add_up(currentPlayer,2);
            }
        }
        for (size_t i = 0; i < players.size(); i++){
            int currentPlayer = (startingDealerIndex + i) % 4;
            if (i % 2 == 0){
                add_up(currentPlayer,2); 
            } else {
                add_up(currentPlayer,3);
            }
        }
        upcard = gamepack.deal_one();
    }
    void print();
    void make_trump(istream& isme){ // use istream?
        int starterIndex = (dealerIndex + 1) % 4;
        for (int round = 1; round <= 2; ++round) {
            for (int i = 0; i < 4; ++i) {
                int currentPlayer = (starterIndex + i) % 4;
                bool deald = (round == 2 && i == 3);

                if (deald || players[currentPlayer]->make_trump(upcard, 
                        (currentPlayer == dealerIndex), round, trump)) {
                    cout << players[currentPlayer]->get_name() << " orders up " << trump << "\n";
                    bool yes = true;
                    if (currentPlayer % 2 == 0) { t1.order_up(yes); } 
                    else { t2.order_up(yes); }
                    if (round == 1) { players[dealerIndex]->add_and_discard(upcard); }
                    return;
                }
                cout << players[currentPlayer]->get_name() << " passes" << endl;
            }
        }
        cout << endl;
    } 
    void print_handwinner(bool odup, Player* p1, Player* p2) {
        cout << p1->get_name() << " and " << p2->get_name() << " win the hand" << endl;
        
        Team* winning_team;
        if (players[0] == p1 || players[2] == p1) { winning_team = &t1; } 
        else { winning_team = &t2; }

        if (odup && winning_team->get_tricks() == 5) {
         cout << "march!" << endl;
        } else if (!odup && winning_team->get_tricks() >= 3) {
            cout << "euchred!" << endl;
        }
    }

    void play_hand(Team *tm1, Team *tm2, int *endgame, istream& isme){
        whatNumberOfTrickIndex = 0;
        shuffle(isShuffles); // Shuffle
        deal(); // Deal
        cout << "Hand " << hand_num << "\n" << players[dealerIndex]->get_name() << " deals\n"
         << upcard << " turned up" << endl; // Print the hand
        make_trump(isme); // Make Trump
        Player* trick_leader = players[(dealerIndex + 1) % 4];
        for (size_t i = 0; i < NUM_TRICKS; i++){
            trick_leader = play_trick(trick_leader);
        }
        t1.calculate();
        t2.calculate();
        if (tm1->get_tricks() > tm2->get_tricks()) {
            print_handwinner(tm1->ordered_up(), tm1->get_playerone(), tm1->get_playertwo());
        } else {
            print_handwinner(tm2->ordered_up(), tm2->get_playerone(), tm2->get_playertwo());
        }
        cout << t1.get_playerone()->get_name() << " and " << t1.get_playertwo()->get_name() 
         << " have " << t1.total_points() << " points\n";
        cout << t2.get_playerone()->get_name() << " and " << t2.get_playertwo()->get_name() 
         << " have " << t2.total_points() << " points\n" << endl;
        if (t1.total_points() >= pts_to_win) {
            winner = &t1;
            *endgame = 1;
            cout << t1.get_playerone()->get_name() << " and " 
            << t1.get_playertwo()->get_name() << " win!" << endl;
        } else if (t2.total_points() >= pts_to_win) {
            winner = &t2;
            *endgame = 1;
            cout << t2.get_playerone()->get_name() << " and " 
             << t2.get_playertwo()->get_name() << " win!" << endl;
        }
        t1.reset();
        t2.reset();
        dealerIndex = (dealerIndex + 1) % 4;
        hand_num++;
    } // REMEMBER: each hand, rotate the dealer 1 to the left; Eldest Hand leads the first trick
    Player* play_trick(Player* currentLeader){
        int startingIndexOfPlayers; // eldest hand
        if(whatNumberOfTrickIndex == 0) {
            if(dealerIndex + 1 < 4) { startingIndexOfPlayers = dealerIndex + 1; } 
            else { startingIndexOfPlayers = 0; }
            currentLeader = players[startingIndexOfPlayers];
        } else { startingIndexOfPlayers = find(players.begin(), players.end(), currentLeader) - players.begin(); }
        int currentHighestIndex = startingIndexOfPlayers;
        Card led_card = currentLeader->lead_card(trump);
        cout << led_card << " led by " << currentLeader->get_name() << endl;
        Card highest_Card = led_card;
        if(startingIndexOfPlayers == 3) { startingIndexOfPlayers = 0; } 
        else { startingIndexOfPlayers++; }
        for(size_t i = 1; i < 4; ++i) {
            if(startingIndexOfPlayers != 0) {
                Card playedCard = players[startingIndexOfPlayers]->play_card(led_card, trump);
                cout << playedCard << " played by " << players[startingIndexOfPlayers]->get_name() << endl;
                if(Card_less(highest_Card, playedCard, led_card, trump)) {
                    highest_Card = playedCard;
                    currentHighestIndex = startingIndexOfPlayers;
                }
                startingIndexOfPlayers++;
                if(startingIndexOfPlayers == 4) { startingIndexOfPlayers = 0; }
            } else {
                Card playedCard = players[startingIndexOfPlayers]->play_card(led_card, trump);
                cout << playedCard << " played by " << players[startingIndexOfPlayers]->get_name() << endl;
                if(Card_less(highest_Card, playedCard, led_card, trump)) {
                    currentHighestIndex = startingIndexOfPlayers;
                    highest_Card = playedCard;
                }
                startingIndexOfPlayers++;
            }
        }
        currentLeader = players[currentHighestIndex];
        cout << currentLeader->get_name() << " takes the trick" << endl << endl;
        if (currentHighestIndex % 2 == 0){ t1.won_trick(); } 
        else { t2.won_trick(); }
        whatNumberOfTrickIndex++; 
        return currentLeader;
    }
};


int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) { cout << argv[i] << " "; }
    cout << endl;
    int gend = -1;
    int NUM_ARGS = 12;
    int PLR_ARG_START = 4;
    vector<Player*> plrs; 
    if (argc != NUM_ARGS) { 
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (stoi(argv[3]) < 1 || stoi(argv[3]) > 100) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 2;
    }
    if (string(argv[2]) != "shuffle" && string(argv[2]) != "noshuffle") {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 3;
    }
    for(int i = 5; i < NUM_ARGS; i+=2) {
        if(string(argv[i]) != "Simple" && string(argv[i]) != "Human"){
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
            return 4;
        }
    }
    for (int i = PLR_ARG_START; i < NUM_ARGS - 1; i+=2){ 
        plrs.push_back(Player_factory(argv[i],argv[i+1]));
    }
    ifstream isn(argv[1]);
    if (!isn.is_open()) {
        cout << "Error opening " << argv[1] << endl;
        return 5;
    }
    Game euchreGame(plrs, Pack(isn), (string(argv[2]) == "shuffle"), stoi(argv[3]));  // Create Game
    while (gend < 0){ euchreGame.play(&gend, cin); }
    for (size_t i = 0; i < plrs.size(); ++i) { delete plrs[i]; }
    return 0;
}
