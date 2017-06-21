#ifndef POOL2D_GAME_HPP
#define POOL2D_GAME_HPP

static const int width = 1000;

static const int height = 550;

#include "util.hpp"
#include "Player.hpp"
#include "Table.hpp"
#include "MyContactListener.hpp"
#include "Cue.hpp"
#include "InterfaceManager.hpp"
#include "Menu.hpp"

class Game {
private:
    GAMESTATE gamestate = BEFORE_HIT;
    PLAYERSTATE playerstate = PLAYER_1;
    bool needChange = false;

    Player player1;
    Player player2;
    std::unique_ptr<Table> table;

    std::pair<size_t, size_t> game_score;
    PLAYERSTATE whoWin;

    Ball* cueBall;
    std::vector<Ball*> deleted_balls;
    std::unique_ptr<Cue> cue;
    b2World world;

    RenderWindow game_window;
    Menu menu;

    MyContactListener myContactListener;
    float power = MIN_POWER;
    InterfaceManager interfaceManager;

    void init();
    void stop();
    void hitCueBall();

    bool isExistMovingBall();
    bool uncorrectScoredEightBall(PLAYERSTATE ps);
    bool correctScoredEightBall(PLAYERSTATE ps);
    bool isForeignScoredBalls(Player& player);

    void removeScoredBalls();
    void moveBalls();
    void retrievedCueBall();
    void retrieveDeletedBalls(BALLTYPE bt);
    void processBeforeHit();
    void processInHandState();
    void processInGameoverState();
    void choosingMenuItem();

public:
    Game();
    void start();
    void RenderInterface();
    void changeGamestate(GAMESTATE gs);
};

#endif //POOL2D_GAME_HPP
