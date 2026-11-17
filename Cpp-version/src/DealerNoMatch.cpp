#include "DealerNoMatch.h"
#include <algorithm>
#include <iostream>


DealerNoMatch::DealerNoMatch():
    dealer_nos(16),
    generator(std::random_device{}()),
    distribution(1, 100)
{}

GameResult DealerNoMatch::play(){
    // Fill vector with random numbers
    std::generate(dealer_nos.begin(), dealer_nos.end(), [this](){   // For every position in dealer_nos, generate a random number using distribution(generator).
        return distribution(generator);
    });

    for (int number : dealer_nos){
        std::cout << number << " ";
    }

std::cout << "\n";

    for (std::size_t i = 0; i < dealer_nos.size() ; i++ ){
        for (std::size_t j = i+ 1; j < dealer_nos.size(); j++){
            if (dealer_nos[i] == dealer_nos[j]){
                return GameResult::Lose;
            }
        }
    }
    return GameResult::Win;
}
