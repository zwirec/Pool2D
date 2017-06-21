#ifndef BOX_UTIL_HPP
#define BOX_UTIL_HPP

#include <Box2D/Box2D.h>
#include "TGUI/TGUI.hpp"

const float PPM = 30;

const float RADTODEG = 180 / b2_pi;
const float DEGTORAD = b2_pi / 180;

const float STEP_POWER = 11.f;
const float MIN_POWER = 25.f;
const float MAX_POWER = 300.f;

enum BODYTYPE {
    BALL = 0x0001,
    POCKET = 0x0002,
    BOUND = 0x0004
};
enum BALLTYPE {
    SOLID,
    STRIP,
    CUEBALL
};

enum BALLNUMBER {
    BALL_1, BALL_2, BALL_3, BALL_4, BALL_5,
    BALL_6, BALL_7, BALL_8, BALL_9, BALL_10,
    BALL_11, BALL_12, BALL_13, BALL_14, BALL_15,
    BALL_16
};

enum GAMESTATE{
    BALLS_ARE_MOVING,
    BEFORE_HIT,
    CUEBALL_IN_HAND,
    PAUSE,
    GAME_OVER
};

enum PLAYERSTATE{
    PLAYER_1,
    PLAYER_2
};

#endif //BOX_UTIL_HPP
