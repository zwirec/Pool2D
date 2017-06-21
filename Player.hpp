//
// Created by daniil on 30.11.16.
//

#ifndef BOX_PLAYER_HPP
#define BOX_PLAYER_HPP


#include <cstddef>
#include "util.hpp"
using namespace sf;

class Player {
private:
    Texture avatar;
    size_t num_scored_balls = 0;
    size_t num_fouls = 0;
    BALLTYPE own_balls;
    std::string name;
public:
    Player(){};
    Player(std::string name, BALLTYPE balltype, Texture& t);
    Player(BALLTYPE ownballs);
    size_t getNumScoredBalls();
    size_t getNumFouls();
    void addNumFouls();
    void addNumScoredBalls(size_t num = 1);
    void reset();
    BALLTYPE getOwnType();
    void setOwnType(BALLTYPE bt);
    void setName(const std::string n);
    void setAvatar(const Texture&);
    std::string& getName();
};


#endif //BOX_PLAYER_HPP
