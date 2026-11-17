#ifndef DEALERNOMATCH_H
#define DEALERNOMATCH_H

#include "GameResult.h"
#include <vector>
#include <random>

class DealerNoMatch
{
    private:
        std::vector<int> dealer_nos;
        std::mt19937 generator;
        std::uniform_int_distribution<int> distribution;

    public:
        DealerNoMatch();

        GameResult play();
};

#endif
