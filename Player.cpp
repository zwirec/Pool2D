#include "Player.hpp"

Player::Player(BALLTYPE ownballs):own_balls(ownballs) {
}

size_t Player::getNumScoredBalls() {
    return num_scored_balls;
}

BALLTYPE Player::getOwnType() {
    return own_balls;
}

void Player::addNumScoredBalls(size_t n) {
    num_scored_balls += n;
}

void Player::setOwnType(BALLTYPE bt) {
    own_balls = bt;
}

Player::Player(std::string name, BALLTYPE balltype, Texture& texture):name(name), own_balls(balltype), avatar(texture){
}

std::string &Player::getName() {
    return name;
}

void Player::setName(const std::string n) {
    name = n;
}

void Player::reset() {
    num_scored_balls = 0;
    num_fouls = 0;
}

size_t Player::getNumFouls() {
    return num_fouls;
}

void Player::addNumFouls() {
    ++num_fouls;
}

void Player::setAvatar(const Texture & t) {
    avatar = t;
}
