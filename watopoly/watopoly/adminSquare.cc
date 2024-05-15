#include "adminSquare.h"
#include "player.h"
#include "state.h"
#include "dice.h"
#include "board.h"

#include <iostream>
#include <random>
#include <chrono>
#include <sstream>

adminSquare::adminSquare(std::string name) : Square{name, SquareType::Admin} {}

collectOSAP::collectOSAP() : adminSquare{"Collect OSAP"} {}

void collectOSAP::landedOn(Player *p) {
    p->changeFunds(200);
    return;
}

goToTims::goToTims() : adminSquare{"Go To Tims"} {}

void goToTims::landedOn(Player *p) {
    p->setPosition(10);
    return;
}

DCTimsLine::DCTimsLine() : adminSquare{"DC Tims Line"} {}

void DCTimsLine::landedOn(Player *p) {
    //DCTims line procedure
    if (p->isInTims()) {
        int rounds = p->getTimsRounds();
        if (rounds == -1) {
            std::cout << "Welcome to Tims! Your turn is now over." << std::endl;
            p->setInTims(true, ++rounds);
            return;
        }

        Dice d = Dice();
        std::cout << p->getName() << ", this is your "; 
        if (rounds == 0) {
            std::cout << "1st ";
        } else if (rounds == 1) {
            std::cout << "2nd ";
        } else {
            std::cout << "3rd ";
        }

        std::cout << "turn in the DC Tims Line. You may leave by rolling doubles [1], paying $50 [2], or using a roll up the rim cup [3]: ";
        std::string res;
        while (std::cin >> res) {
            if (res == "3" && p->getTimsCups() > 0) break;
            else if (res == "3" && !p->getTimsCups()) {
                std::cout << "You do not have a Tims Cup! Please try a valid option: ";
                continue;
            } else if (res == "1" || res == "2") break;
            
            std::cout << "Please enter 1, 2, or 3 for the corresponding options: ";
        }

        if (res == "1") {
            if (board->getTesting()) {
                std::cout << "You have selected to roll doubles." << std::endl; 
                std::cout << "Testing mode, enter two positive integers: "; 
                int dice1; 
                int dice2;
                std::string dice;
                while (getline(std::cin, dice) ) {
                    if (dice.length() == 0) {
                        d.roll();
                    } else {
                        std::stringstream ss{dice};
                        if ((ss >> dice1) && (dice1 > 0) &&
                            (ss >> dice2) && (dice2 > 0)) {
                            d.roll(dice1, dice2);
                            break;
                        } else {
                            std::cout << "Sorry you need to enter two non-negative integers for your dice: " << std::endl;
                        }
                    }
                }
            } else {
                d.roll();
            }
            if (d.isDouble()) {
                std::cout << "You rolled doubles! You have moved forward " << d.getValue() << " spaces." << std::endl;
                p->setInTims(false, -1);
                p->setPosition(p->getState().position + d.getValue());
                board->display();
                return;
            } else {
                std::cout << "You rolled a " << d.getVal1() << " and a " << d.getVal2() << ". You have to stay in DC Tims Line" << std::endl;
            }
       } else if (res == "2") {
            if (50 > p->getState().balance) {
                board->bankrupt(50, nullptr);
            } else {
                p->changeFunds(-50);
                p->setInTims(false, -1);
                return;
            }
        } else if (res == "3") {
            int cups = p->getTimsCups();
            --cups;
            p->setTimsCups(cups);
            board->timsCupsUsed();
            p->setInTims(false, -1);
            return;
        }
        p->setInTims(true, ++rounds);
        if (rounds == 3) {
            std::cout << p->getName() << ", you have spent 3 turns in line! You must leave by paying $50 [1] or using a roll up the rim cup [2]: ";
            std::cout << "Please enter 1 or 2 for the corresponding options: ";
            std::string res;
            
            while (std::cin >> res) {
                if (res == "2" && p->getTimsCups() > 0) break;
                else if (res == "2" && !p->getTimsCups()) {
                    std::cout << "You do not have a Tims Cup! Please try a valid option: ";
                    continue; 
                } else if (res == "1") break;
                
                std::cout << "Please enter 1 or 2 for the corresponding options: ";
            }
            if (res == "1") {
                if (50 > p->getState().balance) {
                    board->bankrupt(50, nullptr);
                } else {
                    p->changeFunds(-50);
                    p->setInTims(false, -1);
                    return;
                }
            } else if (res == "2") {
                int cups = p->getTimsCups();
                --cups;
                p->setTimsCups(cups);
                board->timsCupsUsed();
                p->setInTims(false, -1);
                return;
            }
            p->setPosition(p->getState().position + d.getValue());
            p->setInTims(false, 0);
        }
    }
    return;
}

gooseNesting::gooseNesting() : adminSquare{"Goose Nesting"} {}

void gooseNesting::landedOn(Player *p) {
    return;
}

Tuition::Tuition() : adminSquare{"Tuition"} {}

void Tuition::landedOn(Player *p) {
    //Tuition line procedure
    std::cout << "You must pay either $300 [1] or 10% of your net worth [2]: ";
    std::string res;
    while (std::cin >> res) {
        if (res == "1" || res == "2") break;
        else {
            std::cout << "Please enter 1 or 2: ";
        }
    }

    int fee = 0;
    if (res == "1") {
        fee = 300;
    } else {
        fee = 0.1*(p->getWorth());
    }

    if (fee > p->getState().balance) {
        board->bankrupt(fee, nullptr);
    } else {
        p->changeFunds(-fee);
        std::cout << "You paid $" << fee << std::endl;
        std::cout << "Your current balance is $" << p->getState().balance << "." << std::endl;
    }
    return;
}

coopFee::coopFee() : adminSquare{"Co-op Fee"} {}

void coopFee::landedOn(Player *p) {
    std::cout << "You owe $" << 150 << " to the Bank." << std::endl;
    if (150 > p->getState().balance) {
        board->bankrupt(150, nullptr);
    } else {
        p->changeFunds(-150);
        std::cout << "Your current balance is $" << p->getState().balance << "." << std::endl;
    }
    return;
}

SLC::SLC() : adminSquare{"SLC"} {}

void SLC::landedOn(Player *p) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    if (board->availableCups()) {
        std::default_random_engine rng{seed};
        std::uniform_int_distribution<int> dist(1,100);

        if (dist(rng) == 1) {
            board->givingAwayTimsCup();
            p->setTimsCups(p->getTimsCups() + 1);
            return;
        }
    } 

    std::default_random_engine rng{seed};
    std::uniform_int_distribution<int> dist(0,23);

    int randInt = dist(rng);
    int curPos = p->getState().position;
    if (randInt < 3) { //0, 1, 2 (3/24 = 1/8)
        std::cout << p->getName() << ", you are being moved back 3 squares." << std::endl;
        p->setPosition((curPos-3));
    } else if (randInt < 7) { //3, 4, 5, 6 (4/24 = 1/6)
        std::cout << p->getName() << ", you are being moved back 2 squares." << std::endl;
        p->setPosition((curPos-2));
    } else if (randInt < 12) { //7, 8, 9, 10
        std::cout << p->getName() << ", you are being moved back 1 square." << std::endl;
        p->setPosition((curPos-1));
    } else if (randInt < 14) { //11, 12, 13
        std::cout << p->getName() << ", you are being moved forward 1 square." << std::endl;
        p->setPosition((curPos+1));
    } else if (randInt < 18) { //14, 15, 16, 17
        std::cout << p->getName() << ", you are being moved forward 2 squares." << std::endl;
        p->setPosition((curPos+2));
    } else if (randInt < 22) { //18, 19, 20, 21
    std::cout << p->getName() << ", you are being moved forward 3 squares." << std::endl;
        p->setPosition((curPos+3));
    } else if (randInt < 23) {//22
        std::cout << p->getName() << ", you are being moved to DC Tims Line." << std::endl;
        p->setPosition(30);
    } else {
        std::cout << p->getName() << ", you are being moved to Collect OSAP." << std::endl;
        p->setPosition(0);
    }

}

needlesHall::needlesHall() : adminSquare{"Needles Hall"} {}

void needlesHall::landedOn(Player *p) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    if (board->availableCups()) {
        std::default_random_engine rng{seed};
        std::uniform_int_distribution<int> dist(1,100);

        if (dist(rng) == 1) {
            board->givingAwayTimsCup();
            p->setTimsCups(p->getTimsCups() + 1);
            return;
        }
    } 

    std::default_random_engine rng{seed};
    std::uniform_int_distribution<int> dist(0,17);

    int randInt = dist(rng);
    int curBal = p->getState().balance;
    if (randInt < 1) {
        std::cout << p->getName() << ", you lose $200." << std::endl;
        if (curBal < 200) board->bankrupt(200, nullptr);
        else p->changeFunds(-200);
    } else if (randInt < 3) { 
        std::cout << p->getName() << ", you lose $100." << std::endl;
        if (curBal < 100) board->bankrupt(200, nullptr);
        else p->changeFunds(-100);
    } else if (randInt < 6) { 
        std::cout << p->getName() << ", you lose $50." << std::endl;
        if (curBal < 50) board->bankrupt(200, nullptr);
        else p->changeFunds(-50);
    } else if (randInt < 12) { 
        std::cout << p->getName() << ", you gain $25." << std::endl;
        p->changeFunds(25);
    } else if (randInt < 15) { 
        std::cout << p->getName() << ", you gain $50." << std::endl;
        p->changeFunds(50);
    } else if (randInt < 17) { 
        std::cout << p->getName() << ", you gain $100." << std::endl;
        p->changeFunds(100);
    } else {
        std::cout << p->getName() << ", you gain $200." << std::endl;
        p->changeFunds(200);
    }
}
