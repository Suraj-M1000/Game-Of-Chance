
#include "PickANumber.h"

PickANumber::PickANumber()
: generator(std::random_device{}()),    // Provides a source of random seed data.
    distribution(1, 10)   // Produces integers from 1 through 10, with each integer having an equal probability.
{}   // when the object get created the generaotr which is seeded and distribution get get configured

GameResult PickANumber::play(int guess){
    if (guess < 1 || guess > 10){
        return GameResult::InvalidInput;
    }

    int creatorNumber = distribution(generator);   // This asks the distribution to generate a number using the random engine.

    if (guess == creatorNumber){
        return GameResult::Win;
    }
    return GameResult::Lose;
}
