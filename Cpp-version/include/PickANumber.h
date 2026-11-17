#ifndef PICKANUMBER_H
#define PICKANUMBER_H

#include "GameResult.h"
#include <random>


class PickANumber
{
    private:
        std::mt19937 generator;     // mt19937 - pseudorandom number generator (Mersenne Twister). It generates a sequence of pseudorandom numbers from a seed
        std::uniform_int_distribution<int> distribution;  // Produces integers from 1 through 10, with each integer having an equal probability.

    public:
        PickANumber();
        GameResult play(int guess);
};

#endif
