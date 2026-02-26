#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include "Pack.hpp"
#include "Player.hpp"

using namespace std;

class Game {
public:
    Game(const string &pack_filename,
         bool shuffle_in,
         int points_to_win_in,
         const vector<Player*> &players_in)
        : players(players_in),
          shuffle_flag(shuffle_in),
          points_to_win(points_to_win_in),
          dealer(0),
          hand_number(0)
    {
        ifstream fin(pack_filename);
        if (!fin) {
            cout << "Error opening " << pack_filename << endl;
            exit(1);
        }
        pack = Pack(fin);
        score[0] = 0;
        score[1] = 0;
    }

    void play() {
        while (score[0] < points_to_win && score[1] < points_to_win) {
            play_hand();
            dealer = (dealer + 1) % 4;
            hand_number++;
        }

        if (score[0] >= points_to_win) {
            cout << players[0]->get_name() << " and "
                 << players[2]->get_name() << " win!" << endl;
        } else {
            cout << players[1]->get_name() << " and "
                 << players[3]->get_name() << " win!" << endl;
        }
    }

private:
    Pack pack;
    vector<Player*> players;
    bool shuffle_flag;
    int points_to_win;
    int dealer;
    int hand_number;
    int score[2];
    Suit trump;
    int maker_team;

    void play_hand() {
        cout << "Hand " << hand_number << endl;
        cout << players[dealer]->get_name() << " deals" << endl;

        if (shuffle_flag) {
            pack.shuffle();
        } else {
            pack.reset();
        }

        deal_cards();

        Card upcard = pack.deal_one();
        cout << upcard << " turned up" << endl;

        make_trump(upcard);

        int leader = (dealer + 1) % 4;
        int tricks[2] = {0, 0};

        for (int i = 0; i < 5; i++) {
            int winner = play_trick(leader);
            tricks[winner % 2]++;
            leader = winner;
            cout << endl;
        }

        score_hand(tricks);
        print_score();
        cout << endl;
    }

    void deal_cards() {
        int order[4];
        for (int i = 0; i < 4; i++)
            order[i] = (dealer + 1 + i) % 4;

        int pattern[8] = {3,2,3,2,2,3,2,3};
        int idx = 0;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < pattern[i]; j++) {
                players[order[idx]]->add_card(pack.deal_one());
            }
            idx = (idx + 1) % 4;
        }
    }

    void make_trump(const Card &upcard) {
        maker_team = -1;

        // Round 1
        for (int i = 1; i <= 4; i++) {
            int p = (dealer + i) % 4;
            Suit order_suit;
            if (players[p]->make_trump(upcard, p == dealer, 1, order_suit)) {
                trump = order_suit;
                maker_team = p % 2;
                cout << players[p]->get_name()
                     << " orders up " << trump << endl;
                if (p != dealer) {
                    players[dealer]->add_and_discard(upcard);
                } else {
                    players[dealer]->add_and_discard(upcard);
                }
                cout << endl;
                return;
            } else {
                cout << players[p]->get_name() << " passes" << endl;
            }
        }

        // Round 2
        for (int i = 1; i <= 4; i++) {
            int p = (dealer + i) % 4;
            Suit order_suit;
            if (players[p]->make_trump(upcard, p == dealer, 2, order_suit)) {
                trump = order_suit;
                maker_team = p % 2;
                cout << players[p]->get_name()
                     << " orders up " << trump << endl;
                cout << endl;
                return;
            } else {
                cout << players[p]->get_name() << " passes" << endl;
            }
        }

        assert(false); // should never reach
    }

    int play_trick(int leader) {
        vector<Card> trick(4);
        int order[4];
        for (int i = 0; i < 4; i++)
            order[i] = (leader + i) % 4;

        trick[0] = players[order[0]]->lead_card(trump);
        cout << trick[0] << " led by "
             << players[order[0]]->get_name() << endl;

        for (int i = 1; i < 4; i++) {
            trick[i] = players[order[i]]->play_card(trick[0], trump);
            cout << trick[i] << " played by "
                 << players[order[i]]->get_name() << endl;
        }

        int winner = 0;
        for (int i = 1; i < 4; i++) {
            if (Card_less(trick[winner], trick[i], trick[0], trump)) {
                winner = i;
            }
        }

        cout << players[order[winner]]->get_name()
             << " takes the trick" << endl;

        return order[winner];
    }

    void score_hand(int tricks[]) {
        int maker_tricks = tricks[maker_team];
        int other_team = 1 - maker_team;

        if (maker_tricks == 5) {
            score[maker_team] += 2;
            cout << players[maker_team]->get_name()
                 << " and "
                 << players[maker_team + 2]->get_name()
                 << " win the hand" << endl;
            cout << "march!" << endl;
        }
        else if (maker_tricks >= 3) {
            score[maker_team] += 1;
            cout << players[maker_team]->get_name()
                 << " and "
                 << players[maker_team + 2]->get_name()
                 << " win the hand" << endl;
        }
        else {
            score[other_team] += 2;
            cout << players[other_team]->get_name()
                 << " and "
                 << players[other_team + 2]->get_name()
                 << " win the hand" << endl;
            cout << "euchred!" << endl;
        }
    }

    void print_score() {
        cout << players[0]->get_name()
             << " and " << players[2]->get_name()
             << " have " << score[0] << " points" << endl;

        cout << players[1]->get_name()
             << " and " << players[3]->get_name()
             << " have " << score[1] << " points" << endl;
    }
};

int main(int argc, char **argv) {

    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4"
             << endl;
        return 1;
    }

    string pack_filename = argv[1];
    string shuffle_arg = argv[2];
    int points = stoi(argv[3]);

    if (points < 1 || points > 100 ||
        (shuffle_arg != "shuffle" && shuffle_arg != "noshuffle")) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4"
             << endl;
        return 1;
    }

    vector<Player*> players;
    for (int i = 0; i < 4; i++) {
        string name = argv[4 + i*2];
        string type = argv[5 + i*2];

        if (type != "Simple" && type != "Human") {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                 << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4"
                 << endl;
            return 1;
        }

        players.push_back(Player_factory(name, type));
    }

    cout << "./euchre.exe ";
    for (int i = 1; i < argc; i++)
        cout << argv[i] << " ";
    cout << endl;

    Game game(pack_filename,
              shuffle_arg == "shuffle",
              points,
              players);

    game.play();

    for (Player* p : players)
        delete p;

    return 0;
}