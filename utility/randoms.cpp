/*
ὄνομα: randoms
αἰών: 4/8/2026
βούλημα: game
*/

#include "randoms.h"

#include <random>

int randint(int min , int max ) {
    static std::random_device rd;
    static std::default_random_engine ran{rd()};
    return std::uniform_int_distribution<>{min, max}(ran);
}
