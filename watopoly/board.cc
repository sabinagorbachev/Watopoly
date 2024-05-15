#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "block.h"
#include "state.h"
#include "player.h"
#include "board.h" 
#include "academic.h"
#include "building.h"
#include "gym.h"
#include "residence.h"
#include "adminSquare.h"
#include "dice.h"

using namespace std;

bool isInt(string in) {
	return (!in.empty() && in.find_first_not_of("0123456789") == string::npos);
}

Board::Board(bool t) : availableTimsCups{4}, testing{t} {}

Board::~Board() {
    for (auto s : boardSquares) {
        delete s;
    }

	boardSquares.clear();

	for (auto p : players) {
		delete p; 
	}

    players.clear();
}

void Board::init() {
	Square::setBoard(this);
    //OSAP, parameter: name
	Square* osap = new collectOSAP{};			
	boardSquares.emplace_back(osap);
	//AL, parameter: name, block, purchase cost, purchase improvements, tuition with improvements
	vector<int> AL_tuition = {2, 10, 30, 90, 160, 250};
	Square* AL = new Academic{"AL", 40, 50, AL_tuition, &Arts1};
	Arts1.addToBlock(static_cast<Academic*>(AL));
	boardSquares.emplace_back(AL);
	//SLC1, parameter: name
	Square* slc1 = new SLC{};
	boardSquares.emplace_back(slc1);
	//ML, parameters: name, block, purchase cost, purchase improvements, tuition with improvements
	vector<int> ML_tuition = {4, 20, 60, 180, 320, 450};
	Square* ML = new Academic {"ML",60, 50, ML_tuition, &Arts1};
	Arts1.addToBlock(static_cast<Academic*>(ML));
	boardSquares.emplace_back(ML);
	//TUITION, parameter: name
	Square* tuition = new Tuition{};
	boardSquares.emplace_back(tuition);
	//MKV, parameter: name
	Square* MKV = new Residence{"MKV"};
	boardSquares.emplace_back(MKV);
	//ECH, parameters: name, block, purchase cost, purchase improvements, tuition with improvements
	vector<int> ECH_tuition = {6, 30, 90, 270, 400, 550};
	Square* ECH = new Academic {"ECH", 100, 50, ECH_tuition, &Arts2};
	Arts2.addToBlock(static_cast<Academic*>(ECH));
	boardSquares.emplace_back(ECH);
	//Needles Hall1, parameter: name
	Square* nh1 = new needlesHall{};		
	boardSquares.emplace_back(nh1);
	//PAS, parameters: name, block, purchase cost, purchase improvements, tuition with improvements
	vector<int> PAS_tuition = {6, 30, 90, 270, 400, 550};
	Square* PAS = new Academic {"PAS", 100, 50, PAS_tuition, &Arts2};
	Arts2.addToBlock(static_cast<Academic*>(PAS));
	boardSquares.emplace_back(PAS);
	//HH
	vector<int> HH_tuition = {8, 40, 100, 300, 450, 600};
	Square* HH = new Academic {"HH", 120, 50, HH_tuition, &Arts2};
	Arts2.addToBlock(static_cast<Academic*>(HH));
	boardSquares.emplace_back(HH);
	// DC Time Line
	Square* dctl = new DCTimsLine{}; // need ctor
	boardSquares.emplace_back(dctl);
	//RCH
	vector<int> RCH_tuition = {10, 50, 150, 450, 625, 750};
	Square* RCH = new Academic{"RCH", 140, 100, RCH_tuition, &Eng};
	Eng.addToBlock(static_cast<Academic*>(RCH));
	boardSquares.emplace_back(RCH);
	//PAC
	Square* PAC = new Gym{"PAC"};
	boardSquares.emplace_back(PAC);
	//DWE
	vector<int> DWETuit = {10, 50, 150, 450, 625, 750};
	Square* DWE = new Academic{"DWE", 140, 100, DWETuit, &Eng};
	Eng.addToBlock(static_cast<Academic*>(DWE));
	boardSquares.emplace_back(DWE);
	//CPH
	vector<int> CPH_tuition = {12, 60, 180, 500, 700, 900};
	Square* CPH = new Academic {"CPH", 160, 100, CPH_tuition, &Eng};
	Eng.addToBlock(static_cast<Academic*>(CPH));
	boardSquares.emplace_back(CPH);
	//UWP
	Square* UWP = new Residence{"UWP"};
	boardSquares.emplace_back(UWP);
	//LHI
	vector<int> LHI_tuition = {14, 70, 200, 550, 750,950};
	Square* LHI = new Academic {"LHI", 180, 100,LHI_tuition, &Health};
	Health.addToBlock(static_cast<Academic*>(LHI));
	boardSquares.emplace_back(LHI);
	//SLC2
	Square* slc2 = new SLC{};
	boardSquares.emplace_back(slc2);
	//BMH
	vector<int> BMHTuit = {14, 70, 200, 550, 750, 950};
	Square* BMH = new Academic {"BMH", 180, 100, BMHTuit, &Health};
	Health.addToBlock(static_cast<Academic*>(BMH));
	boardSquares.emplace_back(BMH);
	//OPT
	vector<int> OPT_tuition = {16, 80, 220, 600, 800, 1000};
	Square* OPT = new Academic{"OPT", 200, 100, OPT_tuition, &Health};
	Health.addToBlock(static_cast<Academic*>(OPT));
	boardSquares.emplace_back(OPT);
	//Goose Nesting
	Square* GN = new gooseNesting{};			//need ctor
	boardSquares.emplace_back(GN);
	//EV1
	vector<int> EV1_tuition = {18, 90, 250, 700, 875, 1050};
	Square* EV1 = new Academic{"EV1", 220, 150, EV1_tuition, &Env};
	Env.addToBlock(static_cast<Academic*>(EV1));
	boardSquares.emplace_back(EV1);
	//Needles Hall 2
	Square* nh2 = new needlesHall{};		
	boardSquares.emplace_back(nh2);
	//EV2
	vector<int> EV2_tuition = {18, 90, 250, 700, 875, 1050};
	Square* EV2 = new Academic{"EV2", 220, 150, EV2_tuition, &Env};
	Env.addToBlock(static_cast<Academic*>(EV2));
	boardSquares.emplace_back(EV2);
	//EV3
	vector<int> EV3_tuition = {20, 100, 300, 750, 925, 1100};
	Square* EV3 = new Academic {"EV3", 240, 150, EV3_tuition, &Env};
	Env.addToBlock(static_cast<Academic*>(EV3));
	boardSquares.emplace_back(EV3);
	//V1
	Square* V1 = new Residence {"V1"};
	boardSquares.emplace_back(V1);
	//PHYS
	vector<int> PHYS_tuition = {22, 110, 330, 800, 975, 1150};
	Square* PHYS = new Academic {"PHYS", 260, 150, PHYS_tuition, &Sci1};
	Sci1.addToBlock(static_cast<Academic*>(PHYS));
	boardSquares.emplace_back(PHYS);
	//B1
	vector<int> B1Tuit = {22, 110, 330, 800, 975, 1150};
	Square* B1 = new Academic{"B1", 260, 150, B1Tuit, &Sci1};
	Sci1.addToBlock(static_cast<Academic*>(B1));
	boardSquares.emplace_back(B1);
	//CIF
	Square* CIF = new Gym{"CIF"};
	boardSquares.emplace_back(CIF);
	//B2
	vector<int> B2_tuition = {24, 120, 360, 850, 1025, 1200};
	Square* B2 = new Academic{"B2", 280, 150, B2_tuition, &Sci1};
	Sci1.addToBlock(static_cast<Academic*>(B2));
	boardSquares.emplace_back(B2);
	//Go To Tims
	Square* GT = new goToTims{};		//need ctor
	boardSquares.emplace_back(GT);
	//EIT
	vector<int> EIT_tuition = {26, 130, 390, 900, 1100, 1275};
	Square* EIT = new Academic {"EIT", 300, 200, EIT_tuition, &Sci2};    
	Sci2.addToBlock(static_cast<Academic*>(EIT));          
	boardSquares.emplace_back(EIT);
	//ESC
	vector<int> ESC_tuition = {26, 130, 390, 900, 1100, 1275};
	Square* ESC = new Academic{"ESC", 300, 200, ESC_tuition, &Sci2};
	Sci2.addToBlock(static_cast<Academic*>(ESC));      
	boardSquares.emplace_back(ESC);
	//SLC3
	Square* slc3 = new SLC{};	
	boardSquares.emplace_back(slc3);
	//C2
	vector<int> C2_tuition = {28, 150, 450, 1000, 1200, 1400};
	Square* C2 = new Academic{"C2", 320, 200, C2_tuition, &Sci2};
	Sci2.addToBlock(static_cast<Academic*>(C2));      
	boardSquares.emplace_back(C2);
	//REV
	Square* Rev = new Residence{"REV"};
	boardSquares.emplace_back(Rev);
	//Needles Hall
	Square* nh3 = new needlesHall{};	
	boardSquares.emplace_back(nh3);
	//MC
	vector<int> MC_tuition = {35, 175, 500, 1100, 1300, 1500};
	Square* MC = new Academic {"MC", 350, 200, MC_tuition, &Math};
	Math.addToBlock(static_cast<Academic*>(MC));      
	boardSquares.emplace_back(MC);
	//COOP
	Square* coop = new coopFee{};			
	boardSquares.emplace_back(coop);
	//DC
	vector<int> DC_tuition = {50, 200, 600, 1400, 1700, 2000};
	Square* DC = new Academic{"DC", 400, 200, DC_tuition, &Math};
	Math.addToBlock(static_cast<Academic*>(DC));      
	boardSquares.emplace_back(DC);

	return;
}

int Board::availableCups() { return availableTimsCups; }

void Board::givingAwayTimsCup() {
	--availableTimsCups;
	return;
}

void Board::timsCupsUsed(int c) {
	availableTimsCups += c;
	return;
}


Square* Board::getSquare(int pos) {
	return boardSquares[pos%40];
}

Building* Board::getBuilding(string name) {
	for (auto s : boardSquares) {
		if (s->getType() != SquareType::Admin && s->getName() == name) {
			return static_cast<Building*>(s);
		}
	}

	return nullptr;
}

void Board::addPlayer(Player *p) {
	if (players.empty()) currentPlayer = p;
	players.emplace_back(p);
	return;
}

Player* Board::getPlayer(string name) {
	for (auto p : players) {
		if (p->getName() == name) return p;
	}
	return nullptr;
}

// create init dice object *dice, call roll on dice object, letting player know how much to move, check if player can roll, canRoll(), testing mode

void Board::roll() {	
	//roll dice 
	int rolls = 0;
	Dice d = Dice{};
	//update player state, attach, detach from squares
	while (rolls < 3 && currentPlayer->canRoll()) {
		d.roll();
		++rolls;
		cout << "You rolled a " << d.getVal1() << " and a " << d.getVal2() << "." << endl;
		currentPlayer->setPosition(currentPlayer->getState().position + d.getValue());
		
		if (!d.isDouble()) break; 
		if (rolls < 3 && currentPlayer->canRoll()) {
			cout << "You rolled a double, you get to roll again!" << endl;
		}
	}

	
	currentPlayer->roll();

	if (currentPlayer->isInTims()) {
		next();
		return;
	}

	if (rolls == 3) {
		cout << "You rolled 3 doubles, go to DC Tims Line." << endl;
		currentPlayer->setPosition(10);
	}
	return;
}

void Board::roll(int v1, int v2) {
	// same as above, except no random generation
	//roll dice 
	int rolls = 0;
	Dice d = Dice{};
	//update player state, attach, detach from squares
	while (rolls < 3 && currentPlayer->canRoll()) {
		d.roll(v1, v2);
		++rolls;
		cout << "You rolled a " << d.getVal1() << " and a " << d.getVal2() << "." << endl;
		currentPlayer->setPosition(currentPlayer->getState().position + d.getValue());
	
		if (!d.isDouble()) break; 
		if (rolls < 3 && currentPlayer->canRoll()) {
			cout << "You rolled a double, you get to roll again!" << endl;
			cout << "Testing mode, enter dice rolls: ";
			cin >> v1;
			cin >> v2;
		}
	}

	currentPlayer->roll();

	if (currentPlayer->isInTims()) {
		next();
		return;
	}
 
	if (rolls == 3) {
		cout << "You rolled 3 doubles, go to DC Tims Line." << endl;
		currentPlayer->setPosition(10);
	}
	return;
}
//increment init player, set canRoll() back to true
void Board::next() {
	// clear bool of current player, set next player in vector to current player, return currentPlayer to game
	if (currentPlayer->done()) {
		players.erase(remove(players.begin(), players.end(), currentPlayer), players.end());
		currentPlayer = players[curPindex];
	
	} else {
		currentPlayer->next();
		curPindex = (++curPindex)%players.size();
		currentPlayer = players[curPindex];
	}

	return;
}

//takes in pointer for init building to be bought, assigns building to currentplayer, check for funds, auction if player declines
void Board::purchase(Building *b) {
	cout << "You can buy " << b->getName() << " for $" << b->getCost() << ". [y|n]: ";
	char response;
	while (cin >> response) {
		if (response == 'y' || response == 'n') {
			break;
		} else {
			cout << "Please enter 'y' for YES and 'n' for NO: ";
		}
	}

	if (response == 'n') {
		cout << b->getName() << " will be auctioned." << endl;
		auction(b, currentPlayer);
	} else {
		int price = b->getCost(); 
		cout << "You owe must pay $" << price << " to the Bank." << endl;
		cout << "Your current balance is " << currentPlayer->getState().balance << "." << endl;
		if (price > currentPlayer->getState().balance) {
			cout << "You cannot afford to purchase " << b->getName() << endl;
			cout << b->getName() << " will be auctioned." << endl;
			auction(b, currentPlayer);	
		} else {
			currentPlayer->addBuilding(b);
			currentPlayer->changeFunds(-price);
			cout << "Your current balance is $" << currentPlayer->getState().balance << " and you now own " << b->getName() << "." << endl;
		}
    }
}

bool Board::getTesting() { return testing; }

Player* Board::getCurrentPlayer() {
	return currentPlayer;
}
//check that trade can occur, not money for money, buildings do not within a monopoly do not have improvements, check for funds
void Board::trade(string name, string give, string receive) {
	//valid player passed
	Player *trader = getPlayer(name);
	if (!trader) {
		cout << "The player " << name << " does not exist" << endl;
		return;
	}
	// not money for money
	if (isInt(give) && isInt(receive)) {
		cout << "You cannot trade money for money" << endl;
		return;
	}

	// building ptrs/ints for give and receive
	Building *mine = nullptr;
	int funds_for = 0; //money for building

	Building *theirs = nullptr;
	int for_funds = 0; //building for money

	//check for ownership / proper funds
	// check building block has no improvements
	// check enough funds for the trade
	if (!isInt(give)) {
		mine = getBuilding(give);
		if (!mine) {
			cout << "Invalid input, the building " << give << " does not exist" << endl;
			return;
		}

		Player *give_owner = mine->getOwner();
		if (currentPlayer != give_owner) {
			cout << "You do not own building " << mine->getName() << endl;
			return;
		}
		
		Academic *a = dynamic_cast<Academic*>(mine);
		if (a && a->monopolyHasImprovements()) {
			cout << currentPlayer->getName() << " owns the monopoly for " << a->getName() << " which has improvements" << endl;
			return;
		}
	} else {
		funds_for = stoi(give);
		if (funds_for <= 0) {
			cout << "Money given must be greater than $0" << endl;
			return;
		} else if (funds_for > currentPlayer->getState().balance) {
			cout << "You do not have enough funds to make this trade." << endl;
			return;
		}
	}

	if (!isInt(receive)) {
		theirs = getBuilding(receive);

		if (!theirs) {
			cout << "Invalid input, the building " << receive << " does not exist" << endl;
			return;
		}

		Player *receive_owner = theirs->getOwner();
		if (trader != receive_owner) {
			cout << trader->getName() << " does not own building " << theirs->getName() << endl;
			return;
		}

		Academic *a = dynamic_cast<Academic*>(theirs);
		if (a && a->monopolyHasImprovements()) {
			cout << trader->getName() << " owns the monopoly for " << a->getName() << " which has improvements" << endl;
			return;
		}
	} else {
		for_funds = stoi(receive);
		if (for_funds <= 0) {
			cout << "Money received must be greater than $0" << endl;
			return;
		} else if (for_funds > trader->getState().balance) {
			cout << trader->getName() << " does not have enough funds to make this trade." << endl;
			return;
		}
	}

    string approval;
    cout << trader->getName() << ", do you approve this trade [accept|reject]: ";
    
    while (cin >> approval) {
        if (approval == "accept" || approval == "reject") break;
        cout << "Please enter accept or reject: ";
    }

    if (approval == "reject") {
	    cout << trader->getName() << " rejected your proposed trade" << endl;
	    return;
    }

	if (mine && theirs) {
		currentPlayer->trade(mine, trader, theirs);
	} else if (mine) {
		currentPlayer->trade(mine, trader, for_funds);
	} else if (theirs) {
		currentPlayer->trade(funds_for, trader, theirs);
	}

	return;
}

void Board::improve(std::string cmd, Academic *a) {
	if (cmd == "buy") {
		if (currentPlayer->getState().balance < a->getImprovementCost()) {
			cout << "Sorry, you cannot afford to improve " << a->getName() << endl;
			return;
		}

		if (a->upgrade()) {
			currentPlayer->changeFunds(-1*(a->getImprovementCost()));
		}
	} else {
		if (a->sellUpgrade()) {
			currentPlayer->changeFunds(a->getImprovementCost()/2);
		}
	}
}

//check for ownership
void Board::mortgage(Building *b) {
	if (b->getOwner() != currentPlayer) {
		cout << "Sorry " << currentPlayer->getName() << ", you do not own this property so you cannot mortgage it." << endl;
		return;	
	} 
	Academic *a = dynamic_cast<Academic *>(b); 
	if (a && a->monopolyHasImprovements()) {
		cout << "Sorry you cannot mortgage until you sell off all your improvements in the monopoly" << endl;
		return;
	}
	b->mortgage();
	cout << b->getName() << " has been mortgaged!" << endl;
	currentPlayer->changeFunds(b->getCost()*0.5); 
	display();
	return;
}

//check for funds
void Board::unmortgage(Building *b) {
	// Check if owed player has enough to pay half the cost of the property
	if ( (b->getCost()*0.6) > currentPlayer->getState().balance) {
		cout << "Sorry " << currentPlayer->getName() << ", you do not have enough funds to unmortgage this property." << endl;
	} else if (b->getOwner() != currentPlayer) {
		cout << "Sorry " << currentPlayer->getName() << " you do not own this building." << endl;
	} else {
		// Charge owed player and unmortgage
		currentPlayer->changeFunds(-b->getCost() * 0.6);
		cout << b->getName() << " has been unmortgaged." << endl;
		cout << currentPlayer->getName() << ", " << (b->getCost()*0.6) << " has been deduced from your balance." << endl;
		cout << "Your current balance is $" << currentPlayer->getState().balance << "." << endl;
		b->unmortgage();
		cout << b->getName() << " has been unmortgaged!" << endl;
		display();
	}
	return;
}


// Check value of player assets
void Board::bankrupt(int charge, Player *owed) {

	bool bankrupt = false;

	// Make player choose if they would like to declare bankruptcy or try to raise money
	cout << "You currently do not have the funds to pay your tuition" << endl;
	cout << "You have two ways to proceed:" << endl;
	cout << "1) Declaring bankruptcy" << endl;
	cout << "2) Raising money to try pay your tuition" << endl;
	cout << "Enter 'bankrupt' for declaring bankruptcy or 'raise' to try to raise funds: ";
	string cmd; 
	while (cin >> cmd) {
		
		if (cmd == "bankrupt") {
			bankrupt = true;
			break;
		} else if (cmd == "raise") {

			// If player has ability to raise enough funds
			if (charge < currentPlayer->getCash())  {
				cout << "Your raisable cash is: " << currentPlayer->getCash() << " . And you owe: " << charge << "." << endl;
				cout << "Therefore you have the ability to raise enough funds, and you cannot declare bankruptcy." << endl;
				cout << "You must now raise your cash by trading, selling improvements or mortgaging:" << endl;
				string cmd;
				cout << currentPlayer->getName() << ", enter a command, or enter 'c' for list of commands: "; 
				while (cin >> cmd) {
					if (cmd == "c") {
						cout << "Available commands:" << endl; 
						cout << "'trade' <name of player trading with> <give> <receive> - attempt a trade" << endl;
						cout << "'improve' <property> <buy/sell> - improve or sell improvements of property" << endl;
						cout << "'mortgage' <property> - attempt to mortage property" << endl;
						cout << "'assets' - print your assets" << endl;
						cout << "'all' - print all players' assets" << endl;

					} else if (cmd == "trade") {
						string name; cin >> name;
						string give; cin >> give;
						string receive; cin >> receive;
						trade(name, give, receive);
						display();
					} else if (cmd == "improve") {
						string property; 
						cin >> property;
						string buyOrSell;
						cin >> buyOrSell;
						Building *b = getBuilding(property);
						if (!b) { 
							cout << "Sorry, that building does not exist." << endl; 
						} else {
							if (b->getType() != SquareType::Academic) cout << "Sorry, you can only improve academic buildings" << endl;
							else {
								Academic *a = static_cast<Academic*>(b);
								if (a->getOwner() != currentPlayer) cout << "Sorry, you do not own " << a->getName() << endl;
								else {
									if (buyOrSell == "buy" || buyOrSell == "sell") {
										improve(buyOrSell, a);
										display();
									} else cout << "Invalid command, please enter \"improve <property> buy/sell\"" << endl;
								}
							}
						}
						
					} else if (cmd == "mortgage") {
						string property; 
						cin >> property;   
						Building *b = getBuilding(property);
						if (!b) { 
							cout << "Sorry that building does not exist." << endl; 
						} else {
							if (!b->isMortgaged()) mortgage(b);
							else cout << "Sorry you cannot mortgage a building that is already mortgaged" << endl;
						} 
					} else if (cmd == "assets") {
						cout << currentPlayer->assets() << endl;

					} else if (cmd == "all") {
						all();
					} else cout << "Invalid entry, please try again." << endl;
					
					if (currentPlayer->getState().balance > charge) {
						cout << currentPlayer->getName() << ", you have raised enough funds" << endl;
						currentPlayer->changeFunds(-charge);
						if (owed) owed->changeFunds(charge);
						cout << currentPlayer->getName() << ", " << charge << " has been deduced from your balance." << endl;
						cout << "Your current balance is $" << currentPlayer->getState().balance << "." << endl;
						break;
					}

        			cout << currentPlayer->getName() << ", enter a command, or enter 'c' for list of commands: "; 
				}

			// If player doesn't have the ability to raise enough funds
			} else {
				cout << "Your raisable cash is: " << currentPlayer->getCash() << " . And you owe: " << charge << "." << endl;
				cout << "Therefore you do not have the ability to raise enough funds, your only option now is either to attempt to trade or declare bankruptcy:" << endl;
				string cmd;
				cout << currentPlayer->getName() << ", enter a command, or enter 'c' for list of commands: "; 
				while (cin >> cmd) {
					if (cmd == "c") {
						cout << "Available commands:" << endl; 
						cout << "'trade' <name of player trading with> <give> <receive> - attempt a trade" << endl;
						cout << "'bankrupt' - print all players' assets" << endl;

					} else if (cmd == "trade") {
						string name; cin >> name;
						string give; cin >> give;
						string receive; cin >> receive;
						trade(name, give, receive);
						display();		
					} else if (cmd == "bankrupt") {
						bankrupt = true; 
						break;

					} else cout << "Invalid entry, please try again." << endl;
					
					display();
					if (currentPlayer->getState().balance > charge) {
						cout << currentPlayer->getName() << ", you have raised enough funds" << endl;
						currentPlayer->changeFunds(-charge);
						cout << currentPlayer->getName() << ", " << charge << " has been deduced from your balance." << endl;
						cout << "Your current balance is $" << currentPlayer->getState().balance << "." << endl;
						break;
					}
			
					cout << currentPlayer->getName() << ", enter a command, or enter 'c' for list of commands: "; 
				}
			}
			break;
		} else {
			cout << "Sorry please enter a valid command:" << endl;
			cout << "Enter 'bankrupt' for declaring bankruptcy or 'raise' to try to raise funds: ";
		}
	}
	if (bankrupt) {
		int bankruptTimsCups = currentPlayer->getTimsCups();
		// If assets go to player owed
		if (owed) { 
			
			int owedTimsCups = owed->getTimsCups();
			owed->setTimsCups(owedTimsCups+bankruptTimsCups);
			for (auto b: currentPlayer->getProperties()) {
				
				if (b->isMortgaged()) {
					cout << "Player: " << owed->getName() << ", you are receiving a mortgaged property: " << b->getName() << "." << endl;
					cout << "This means you must immediately pay 10%: (" << 0.1*b->getCost() << ") of the original price of property to the bank." << endl;
					
					// Check if owed player has enough to pay 10% of original price of property
					if ( 0.1*b->getCost() > owed->getState().balance) {
						auction(b, owed);
						cout << owed->getName() << ", you don't have the funds to pay the 10%, so you can't receive this property, it has to be auctioned." << endl;
					} else {
						owed->addBuilding(b); 
						owed->changeFunds(-0.1*b->getCost());
						cout << owed->getName() << ", " << 0.1*b->getCost() << " has been deduced from your balance." << endl;
						cout << "Your current balance is $" << owed->getState().balance << "." << endl;
					}

					cout << "" << endl;
					cout << "You now have two options:" << endl;
					cout << "1) You can unmortgage the property by paying " << b->getCost()/2 << ". (Half the cost of the property)" << endl;
					cout << "2) You can leave the property mortgaged and later pay " << (b->getCost()*0.6) << ". (Half the cost of the property + 10%)" << endl;
					cout << "Please enter '1' for option 1 or '2' for option 2: "; 
					string cmd;
					while (cin >> cmd) {
						
						if (cmd == "1") {
							if ( (b->getCost()/2) > owed->getState().balance) {
									cout << "Sorry " << owed->getName() << ", you do not have enough funds to unmortgage this property right now." << endl;
									cout << "You will have to leave the property mortgaged and later pay " << (b->getCost()*0.6) << ", when you choose to unmortage." << endl;
								} else {
									// Charge owed player and unmortgage
									b->unmortgage();
									owed->changeFunds(-(b->getCost()/2));
									cout << b->getName() << " has been unmortgaged." << endl;
									cout << owed->getName() << ", " << (b->getCost()/2) << " has been deduced from your balance." << endl;
									cout << "Your current balance is $" << owed->getState().balance << "." << endl;
								}
							break;
						} else if (cmd == "2") {
							cout << "You have chosen to leave the property mortgaged and later pay " << (b->getCost()*0.6) << ", when you choose to unmortage." << endl;
							break;
						} else {
							cout << "Sorry please enter a valid command:" << endl;
							cout << "Please enter '1' for option 1 or '2' for option 2: "; 
						}
					}
				}
			}

		// If assets go to bank
		} else { 
			timsCupsUsed(bankruptTimsCups);
			vector<Building*> buildingsAuctioning = currentPlayer->getProperties();
			for (auto *b : buildingsAuctioning) {
				auction(b, currentPlayer); 
			}
		}
		currentPlayer->bankrupt();
		getSquare(currentPlayer->getState().position)->detach(currentPlayer);
		next();
	}
}

//actual auction
void Board::auction(Building *b, Player *noBid) {
	vector<Player*> playersCopy = players;
	playersCopy.erase(remove(playersCopy.begin(), playersCopy.end(), noBid), playersCopy.end());
	int bid = b->getCost();

	// Get participants for auction
	cout << "Auction for property: " << b->getName() << ". Bids begin at: " << bid << "." << endl;
	for (auto p : playersCopy) {
		cout << p->getName() << ": Would you like to participate? Enter 'Y' or 'N': "; 
		string ans;
		while (cin >> ans) {
			
			if (ans == "Y") {
				break;
			} else if (ans == "N") {
				playersCopy.erase(remove(playersCopy.begin(), playersCopy.end(), p), playersCopy.end());
				break;
			} else {
				cout << "Sorry please enter a valid command:" << endl;
				cout << p->getName() << ": Would you like to participate? Enter 'Y' or 'N': "; 
			}

		}
	}

	// If no auction participants, set owner to bank (leave on market)
	if (playersCopy.size() == 0) {
		b->setOwner(nullptr); 

	// Start auction
	} else {
		int i = 0;
		cout << "Welcome to the auction of " << b->getName() << ". Starting bid is " << bid << "." <<  endl;
		while (playersCopy.size() > 1) {
			cout << playersCopy[i]->getName() << ": Enter a bid larger than " << bid <<  " or 'd' for dropout: ";
			string cmd; 
			cin >> cmd;
			if (isInt(cmd)) {
				if (stoi(cmd) > bid) {
					if (playersCopy[i]->getState().balance >= bid) {
						bid = stoi(cmd);
						i++;
					} else {
						cout << "Sorry you do not have enough balance for that bid, you must dropout of the auction." << endl;
						playersCopy.erase(remove(playersCopy.begin(), playersCopy.end(), playersCopy[i]), playersCopy.end());
						cout << playersCopy[i]->getName() << " has been removed from the auction." << endl;
					}
				} 
				
			} else if (cmd == "d") {
				playersCopy.erase(remove(playersCopy.begin(), playersCopy.end(), playersCopy[i]), playersCopy.end());
				cout << playersCopy[i]->getName() << " has been removed from the auction." << endl;
			} else {
				cout << "Sorry please enter a valid command:" << endl;
			}
	
		}
								
		// Should be one player left in auction, they win the auction, transfer ownership to them, charge them bid amount
		playersCopy[0]->changeFunds(-bid);
		playersCopy[0]->addBuilding(b);
		cout << "Congratulations " << playersCopy[0]->getName() << "! You have won the auction." << endl;
		cout << "Ownership of " << b->getName() << " has been transferred to you, and " << bid << " has been deducted from your balance." << endl;
		cout << "Your current balance is $" << playersCopy[0]->getState().balance << "." << endl;
	}

}

//print current players assests
void Board::assets() {
	cout << currentPlayer->assets() << flush;
}

//prints every players assests
void Board::all() {
	for (auto p : players) {
		cout << p->assets() << flush;
	}
}

void Board::save(ofstream& out) {
	out << players.size() << endl;
	
	for (auto p : players)
		out << p->print() << flush;

	for (auto s : boardSquares) {
		auto b = dynamic_cast<Building*>(s);
		if (b) out << b->print() << endl;
	}
}


void Board::printPlayers(int i) {
	Square *s = boardSquares[i];
	vector<Player*> playersOnSquare = s->getPlayersOnSquare();
	
	const std::string GREEN = "\033[1m\033[32m" ;
	const std::string RED = "\033[31m";
	const std::string RESET =  "\033[0m";
	std::string colour; 
	
	for (auto p : playersOnSquare) {
		colour = (p->isInTims()) ? RED : GREEN;	
		cout << colour << p->getPiece() << RESET;
	}
	
	cout << string(7-playersOnSquare.size(), ' ');

	cout << "|";
}


void Board::printImprovements(int i) {
    int numberOfImprovements;
    Academic *a = dynamic_cast<Academic*>(boardSquares[i]); 
    numberOfImprovements = a->getImprovementValue();

    const std::string CYAN = "\033[36m"; 
    const std::string RESET =  "\033[0m";

    if (numberOfImprovements > 0) {
        for (int j = 0; j < numberOfImprovements; ++j){
                cout << CYAN << "I" << RESET;
        }
		cout << string(7-numberOfImprovements, ' ');
    } else {
        cout << string(7, ' ');
    }

    cout << "|";
}


void Board::display() {

	//1
	cout << string(89, '_') << endl;
	//2
	cout << "|Goose  |";
	printImprovements(21);
	cout << "NEEDLES|";
	printImprovements(23);
	printImprovements(24);
	cout << "V1     |";
	printImprovements(26);
	printImprovements(27);
	cout << "CIF    |";
	printImprovements(29);
	cout << "GO TO  |" << endl;
	//3
	cout <<
	"|Nesting|-------|HALL   |-------|-------|       |-------|-------|       |-------|TIMS   |"
	<< endl;
	//4
	cout << "|       |EV1    |       |EV2    |EV3    |       |PHYS   |B1     |       |B2     |       |" << endl;
	//5
	cout <<	"|";
	for (int i = 20; i < 31; i++) {
		printPlayers(i);
	}
	cout << endl;

	//6
	cout << "|_______|_______|_______|_______|_______|_______|_______|_______|_______|_______|_______|"
	<< endl;
	//7
	cout << "|";
	printImprovements(19);                                                           
	cout <<
	"                                                                       |";
	printImprovements(31); 
	cout << endl;
	//8
	cout << 
	"|-------|                                                                       |-------|"
	<< endl;
	//9
	cout <<
	"|OPT    |                                                                       |EIT    |"
	<< endl;
	cout << "|";
	printPlayers(19);   
	cout <<
	"                                                                       |";
	printPlayers(31); 
	cout << endl;                                                                     
	//10
	cout << 
	"|_______|                                                                       |_______|"
	<< endl;
	//11
	cout <<
	"|";
	printImprovements(18);   
	cout <<
	"                                                                       |";
	printImprovements(32); 
	cout << endl;
	//12
	cout << 
	"|-------|                                                                       |-------|"
	<< endl;
	//13
	cout <<
	"|BMH    |                                                                       |ESC    |"
	<< endl;
	//14
	cout << "|";
	printPlayers(18);   
	cout <<
	"                                                                       |";
	printPlayers(32); 
	cout << endl; 

	//15
	cout <<
	"|_______|                                                                       |_______|"
	<< endl;
	//16
	cout <<
	"|SLC    |                                                                       |SLC    |"
	<< endl;
	//17
	cout <<
	"|       |                                                                       |       |"
	<< endl;
	//18
	cout <<
	"|       |                                                                       |       |"
	<< endl;
	//19
	cout << "|";
	printPlayers(17);   
	cout <<
	"                                                                       |";
	printPlayers(33); 
	cout << endl; 
	//20
	cout <<
	"|_______|                                                                       |_______|"
	<< endl;
	//21
	cout <<
	"|";
	printImprovements(16);   
	cout <<
	"                                                                       |";
	printImprovements(34); 
	cout << endl;
	cout << 
	"|-------|                                                                       |-------|"
	<< endl;
	//22
	cout << 
	"|LHI    |                                                                       |C2     |"
	<< endl;
	//23
	cout << "|";
	printPlayers(16);   
	cout <<
	"             _____________________________________________             |";
	printPlayers(34); 
	cout << endl; 
	// 24
	cout <<
	"|_______|            |                                             |            |_______|"
	<< endl;
	//25
	cout <<
	"|UWP    |            | #   #  ##  ###### ###  ###   ###  #   #   # |            |REV    |"
	<< endl;
	//26
	cout <<
	"|       |            | #   # #  #   #   #   # #  # #   # #   #   # |            |       |"
	<< endl;
	//27
	cout <<
	"|       |            | # # # ####   #   #   # ###  #   # #    # #  |            |       |"
		<< endl;
	//28
	cout << "|";
	printPlayers(15);   
	cout <<
			 "            | # # # #  #   #   #   # #    #   # #     #   |            |";
	printPlayers(35); 
	cout << endl;
	//29
	cout <<
	"|_______|            | ##### #  #   #    ###  #     ###  ## #  #   |            |_______|"
	<< endl;
	//30
	cout << "|";
	printImprovements(14); 
	cout <<
	"            |_____________________________________________|            |NEEDLES|"
	<< endl;
	//31
	cout <<
	"|-------|                                                                       |HALL   |"
	<< endl;
	//32
	cout <<
	"|CPH    |                                                                       |       |"
	<< endl;
	//33
	cout << "|";
	printPlayers(14);   
	cout <<
	"                                                                       |";
	printPlayers(35); 
	cout << endl; 
	//34
	cout <<
	"|_______|                                                                       |_______|"
	<< endl;
	//35
	cout <<
	"|";
	printImprovements(13);   
	cout <<
	"                                                                       |";
	printImprovements(37); 
	cout << endl;
	//36
	cout <<
	"|-------|                                                                       |-------|"
	<< endl;
	//37
	cout <<
	"|DWE    |                                                                       |MC     |"
	<< endl;
	//38
	cout << "|";
	printPlayers(13);   
	cout <<
	"                                                                       |";
	printPlayers(37); 
	cout << endl; 
	cout <<
	"|_______|                                                                       |_______|"
	<< endl;
	//39
	cout <<
	"|PAC    |                                                                       |COOP   |"
	<< endl;
	//40
	cout << 
	"|       |                                                                       |FEE    |"
	<< endl;
	//41
	cout <<
	"|       |                                                                       |       |"
	<< endl;
	//42
	cout << "|";
	printPlayers(12);   
	cout <<
	"                                                                       |";
	printPlayers(38); 
	cout << endl; 
	//43
	cout <<
	"|_______|                                                                       |_______|"
	<< endl;
	//44
	cout <<
	"|";
	printImprovements(11);   
	cout <<
	"                                                                       |";
	printImprovements(39); 
	cout << endl;
	//45
	cout <<
	"|-------|                                                                       |-------|"
	<< endl;
	//46
	cout <<
	"|RCH    |                                                                       |DC     |"
	<< endl;
	//47 
	cout << "|";
	printPlayers(11);   
	cout <<
	"                                                                       |";
	printPlayers(39); 
	cout << endl; 
	//48
	cout <<
	"|_______|_______________________________________________________________________|_______|"
	<< endl;
	//49
	cout << "|DC Tims|";
	printImprovements(9);
	printImprovements(8);
	cout << "NEEDLES|";
	printImprovements(6);
	cout << "MKV    |";
	cout << "TUITION|";
	printImprovements(3);
	cout << "SLC    |";
	printImprovements(1);
	cout << "COLLECT|" << endl;
	//50
	cout <<
	"|Line   |-------|-------|HALL   |-------|       |       |-------|       |-------|OSAP   |"
	<< endl;
	//51
	cout << 
	"|       |HH     |PAS    |       |ECH    |       |       |ML     |       |AL     |       |"
	<< endl;
	//52
	cout <<
	"|";
	for (int i = 10; i > -1; i--){
		printPlayers(i);
	}
	cout << endl;
	//53
	cout <<    "|_______|_______|_______|_______|_______|_______|_______|_______|_______|_______|_______|"
	<< endl;
}

vector<Player*> Board::getPlayers() { return players; }
