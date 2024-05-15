#ifndef __BOARD_H_
#define __BOARD_H_

class Player;
class Square;
class Building;

#include "block.h"
#include <string>
#include <fstream>
using std::vector;

class Board {
    std::vector<Square*> boardSquares;
    std::vector<Player*> players;
    Player *currentPlayer;
    int curPindex = 0; 
    
    int availableTimsCups;
    bool testing;

    Block Arts1{"Arts1"};
    Block Arts2{"Arts2"};
    Block Eng{"Eng"};
    Block Health{"Health"};
    Block Env{"Env"};
    Block Sci1{"Sci1"};
    Block Sci2{"Sci2"};
    Block Math{"Math"};

    public:
    Board(bool t);
    ~Board();
    void init();
    int availableCups();
    void givingAwayTimsCup();
    void timsCupsUsed(int c = 1);
    Square* getSquare(int pos);
    Building* getBuilding(std::string name);
    void addPlayer(Player *p);
    Player *getPlayer(std::string name);
    void roll();
    void roll(int v1, int v2);
    void next();
    Player* getCurrentPlayer();
    void purchase(Building *b);
    void trade(std::string name, std::string give, std::string receive);
    void improve(std::string cmd, Academic* a);
    void mortgage(Building *b);
    void unmortgage(Building *b);
    void bankrupt(int charge, Player *owed);
    void auction(Building *b, Player *noBid);
    void assets();
    void all();
    void save(std::ofstream &out);
    void printPlayers(int i);
    void printImprovements(int i);
    void display();
    bool getTesting();
    vector<Player*> getPlayers();
};

#endif
