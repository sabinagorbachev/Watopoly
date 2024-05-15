#ifndef GAME_H
#define GAME_H

#include <string>
#include "board.h"

class Player;

using namespace std;

class Game {
    bool testing; 
    bool loaded;
    Board board;
    Player *currentPlayer;

    private:
        void loadProperty(string s); // assigns property to have correct owner and improvements
        void loadPlayer(string playerDetails, bool first); // loads in players fields

    public:
        Game(bool t = false, bool l = false); // Constructor
        void load(ifstream &ss); // loads in previously existing game
        void save(); // saves current game
        void play(); // executes game
        void readPlayers(); // reads in player names and chosen pieces for new game
};

#endif
