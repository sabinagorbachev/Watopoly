#include "dice.h"

Dice::Dice() : val1{0}, val2{0}, value{0}, doubles{false} {}

Dice::~Dice() {}

void Dice::roll() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng{seed};
    std::uniform_int_distribution<int> dist(1,6);
    val1 = dist(rng);
    val2 = dist(rng);

    if (val1 == val2) doubles = true;
    else doubles = false;

    value = val1+val2;

    return;
}

void Dice::roll(int v1, int v2) {
    val1 = v1;
    val2 = v2;
    if (val1 == val2) doubles = true;
    else doubles = false;

    value = val1+val2;
    return;
}

int Dice::getValue() {return value;}

bool Dice::isDouble() {return doubles;}

int Dice::getVal1() {return val1;}

int Dice::getVal2() {return val2;}
