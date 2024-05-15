#include "game.h"
#include "building.h"
#include "academic.h"
#include "player.h"
#include "board.h"
#include "state.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;  

// Constructor
Game::Game(bool t, bool l): testing{t}, loaded{l}, board{t}, currentPlayer{nullptr} {
    board.init();
}

void Game::loadProperty(string s) {

    string word;
    int improvements;
    istringstream ss{s};
    

    // Get property object
    ss >> word; 
    Building *property = board.getBuilding(word);
    // Set ownership
    ss >> word;
    Player *p = board.getPlayer(word);
    if (p) {        
        // Add to player's buildingsOwned
        p->addBuilding(property);

        // Set improvements
        ss >> improvements;
        property->setImprovements(improvements);
    }
}
    


void Game::loadPlayer(string playerDetails, bool first) { 
    
    string word;
    int cups; 
    istringstream ss{playerDetails};

    // Get player object, add to board
    string name;
    ss >> name; 
    

    // Set players piece char
    ss >> word; 
    char piece = word[0];
    

    Player *p = new Player {&board, name, piece};

    // Set players roll-up-the-rim cups
    ss >> word; 
    stringstream ss2; 
    ss2 << word;
    ss2 >> cups; 
    p->setTimsCups(cups);


    // Set player balance
    ss >> word; 
    stringstream ss3; 
    int money;
    ss3 << word;
    ss3 >> money; 
    State newState; 
    newState.balance = money;
    newState.canRoll = true;


    // Set player position
    ss >> word;
    stringstream ss4;
    int pos; 
    ss4 << word;
    ss4 >> pos;
    newState.position = pos;
    board.getSquare(pos)->attach(p);
    if (pos == 10) { 
        ss >> word; 
        stringstream ss5;
        int tims;
        ss5 << word;
        ss5 >> tims; 
        if (tims == 0) p->setInTims(false, 0);
        else {
            ss >> word; 
            stringstream ss6;
            int rounds;
            ss6 << word;
            ss6 >> rounds; 
            p->setInTims(true, rounds);

        }
    }
    
    p->setState(newState);
    if (first) currentPlayer = p; 
    board.addPlayer(p);
}
    

// Loads in previously existing game
void Game::load(ifstream &ss) {

    string s; 
    int numPlayers;
    getline(ss, s);
    stringstream ss2{s};
    ss2 >> numPlayers;

    for (int i = 0; i < numPlayers; i++) {
        getline(ss, s);
        if (i == 0) loadPlayer(s, true);
        else loadPlayer(s, false);
    }
    while (getline(ss, s)) {
        loadProperty(s);
    }

}

// Saves current game
void Game::save() {
    string file;
    while (cin >> file) {
        
        ofstream savedGame; 
        savedGame.open(file);

        if (savedGame.is_open()) {
            board.save(savedGame);
            break;
        } else {
            cout << "File could not be opened, please enter valid file name: ";
        }
    }
    return;
}

// Executes game
void Game::play() {
    string cmd; 
    if (!loaded) readPlayers(); 
    
    
    cout << currentPlayer->getName() << ", enter a command, or enter 'c' for list of commands: "; 
    while (cin >> cmd) {
        if (cmd == "c") {
            cout << "Available commands:" << endl; 
            cout << "'roll' - roll 2 dice, if in testing mode input 2 values for dice" << endl;
            cout << "'next' - give control to next player" << endl;
            cout << "'trade' <name of player trading with> <give> <receive> - attempt a trade" << endl;
            cout << "'improve' <property> <buy/sell> - improve or sell improvements of property" << endl;
            cout << "'mortgage' <property> - attempt to mortage property" << endl;
            cout << "'unmortgage' <property> - attempt to unmortage property " << endl;
            cout << "'assets' - print your assets" << endl;
            cout << "'all' - print all players' assets" << endl;
            cout << "'save' - save and quit current game" << endl;
            cout << "'quit' - quit current game" << endl;
            continue;
        } else if (cmd == "roll") { // Player rolls two dice OR if -testing, takes input for dice
            if (currentPlayer->canRoll()) {
                if (testing) { 
                    int dice1; 
                    int dice2;
                    string dice;
                    getline(cin, dice);

                    if (dice.length() == 0) {
                        board.roll();
                        board.display();
                    } else {
                        stringstream ss{dice};
                        if ((ss >> dice1) && (dice1 > 0) &&
                            (ss >> dice2) && (dice2 > 0)) {
                            board.roll(dice1, dice2);
                            board.display();
                        } else {
                           cout << "Sorry you need to enter two non-negative integers for your dice." << endl;
                        }
                    }
                } else {
                    board.roll();
                    board.display();
                }
                
            } else cout << "Sorry you cannot roll" << endl;
        } else if (cmd == "next") { // Gives control to next payer
            if (!currentPlayer->canRoll()) {
                board.next(); 
                currentPlayer = board.getCurrentPlayer();

                if (currentPlayer->isInTims()) {
                    board.getSquare(10)->landedOn(currentPlayer);
                }
            } else cout << "Sorry, " << currentPlayer->getName() << ", you still can roll so you cannot give control to the next player." << endl;
        } else if (cmd == "trade") { // Executes sa trade between current player and input player
            string name; cin >> name;
            string give; cin >> give;
            string receive; cin >> receive;
            board.trade(name, give, receive);
            board.display();
        } else if (cmd == "improve") { // Attemps to improve property
            string property; 
            cin >> property;
            string buyOrSell;
            cin >> buyOrSell;
            Building *b = board.getBuilding(property);
            if (!b) { 
                cout << "Sorry, that building does not exist." << endl; 
            } else {
                if (b->getType() != SquareType::Academic) cout << "Sorry, you can only improve academic buildings" << endl;
                else {
                    Academic *a = static_cast<Academic*>(b);
                    if (a->getOwner() != currentPlayer) cout << "Sorry, you do not own " << a->getName() << endl;
                    else {
                        if (buyOrSell == "buy" || buyOrSell == "sell") {
                            board.improve(buyOrSell, a);
                            board.display();
                        } else cout << "Invalid command, please enter \"improve <property> buy/sell\"" << endl;
                    }
                }
            }
            
        } else if (cmd == "mortgage") { // Attemps to mortgage property
            string property; 
            cin >> property;   
            Building *b = board.getBuilding(property);
            if (!b) { 
                cout << "Sorry that building does not exist." << endl; 
            } else {
                if (!b->isMortgaged()) board.mortgage(b);
                else cout << "Sorry you cannot mortgage a building that is already mortgaged" << endl;
            } 

        } else if (cmd == "unmortgage") { // Attempts to unmortgage property
            string property; 
            cin >> property;   
            Building *b = board.getBuilding(property);
            if (!b) { 
                cout << "Sorry that building does not exist." << endl; 
            } else {
                if (b->isMortgaged()) board.unmortgage(b);
                else cout << "Sorry you cannot unmortgage a building that is already mortgaged" << endl;
            } 
        } else if (cmd == "assets") { 
            cout << currentPlayer->assets() << endl;
        } else if (cmd == "all") { // Display assets of every player
            board.all();
        } else if (cmd == "save") {
            save();
            cout << "Game saved." << endl;
        } else if (cmd == "quit") {
            cout << "Would you like to save your game?" << endl;
            cout << "Enter 'yes' to save, any key to quit without saving: ";
            string ans; 
            cin >> ans; 
            if (ans == "yes") {
                save();
                cout << "Game saved." << endl;
            } 
            cout << "Game quit." << endl;
            cout << "Thanks for Playing!" << endl;
            break;
        } else  {
            cout << "Invalid entry, please try again." << endl;
        }
        
        if (board.getPlayers().size() == 1) {
            std::vector<Player*> lastPlayer = board.getPlayers();
            cout << "Congratulations " << lastPlayer[0]->getName() << "!. You are the last player left, you won!" << endl;
            cout << "Would you like to save your game?" << endl;
            cout << "Enter 'yes' to save, any key to quit without saving: ";
            string ans; 
            cin >> ans; 
            if (ans == "yes") {
                save();
                cout << "Game saved." << endl;
            } 
            cout << "Game quit." << endl;
            cout << "Thanks for Playing!" << endl;
            break;
        }
        currentPlayer = board.getCurrentPlayer();
        cout << currentPlayer->getName() << ", enter a command, or enter 'c' for list of commands: "; 
    }
    return;

}

// reads in player names and chosen pieces for new game
void Game::readPlayers() {
    int numPlayers; 
    string name;
    string stringP;
    char p;
    string word; 
    cout << "Please enter number of players: "; 
    
    // Get numPlayers
    while (cin >> word) {
        
        stringstream ss; 
        ss << word;
        ss >> numPlayers;

        if (numPlayers < 2 || numPlayers > 7) {
            cout << "Invalid number of players, please enter value [2-7]: ";
        } else {
            break;
        }
    }

    vector<char> pieces; // vector for keeping track of taken pieces

    // Add each player to game
    for (int i = 0; i < numPlayers; i++) {
        
        // Reading in player's name
        cout << "Player " << i+1 << ", please choose a player from the following options: " << endl;
        cout << "           Goose    GRT-Bus     Doughnut    Professor    Student    Money    Laptop    Pink-tie" << endl;
        cout << "            'G'       'B'         'D'          'P'         'S'       '$'      'L'        'T'   " << endl;
        cout << "Please enter a char corresponding to your player choice: ";
        
        // Reading in player's piece choice
        while (cin >> stringP ) {
            bool taken = false;
            
            if (stringP.length() > 1) cout << "Please enter only 1 character from 'G', 'B', 'D', 'P', 'S', '$', 'L', 'T': ";
            else {
                p = stringP[0]; // convert string to char

                // Check p is valid
                if (p == 'G' || p == 'B'|| p == 'D' || p == 'P' || p =='S' || p == '$'|| p == 'L' || p == 'T') {
                    for (char pc : pieces) { if (pc == p) taken = true; }
                    if (taken) cout << "That piece is already taken, please try again: ";
                    else { pieces.push_back(p); break;}
                } else cout << "Invalid entry. Please enter only a character from 'G', 'B', 'D', 'P', 'S', '$', 'L', 'T': ";
            }
        }

        if (p == 'G') name = "Goose";
        if (p == 'B') name = "GRT-Bus";
        if (p == 'D') name = "Doughnut";
        if (p == 'P') name = "Professor";
        if (p == 'S') name = "Student";
        if (p == '$') name = "Money";
        if (p == 'L') name = "Laptop";
        if (p == 'T') name = "Pink-tie";

        cout << "Player " << i+1 << ", you chose to be the " << name << "." << endl;
        cout << "" << endl;

        Player *newPlayer = new Player{&board, name, p}; 
        board.addPlayer(newPlayer);
        board.getSquare(0)->attach(newPlayer);
        
        // If first player, set currentPlayer to newPlayer
        if (i == 0) {
            currentPlayer = newPlayer; 
        }
    }
    
}
