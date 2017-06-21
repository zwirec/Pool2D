#ifndef POOL2D_SOUNDMANAGER_H
#define POOL2D_SOUNDMANAGER_H


#include <Box2D/Box2D.h>
#include <SFML/Audio.hpp>

class SoundManager {
private:
    static SoundBuffer bufferBallToBall;
    static SoundBuffer bufferBallToBound;
    static SoundBuffer bufferBallToPocket;
    static SoundBuffer bufferCueToBall;
    static SoundBuffer bufferMenuHover;
    static SoundBuffer bufferMenuClick;
    static SoundBuffer bufferMainTheme;

    static Sound soundBallToBall;
    static Sound soundBallToBound;
    static Sound soundBallToPocket;
    static Sound soundCueToBall;
    static Sound soundMenuHover;
    static Sound soundMenuClick;
    static Sound soundMainTheme;

    static bool sound;
public:
    static void playBallToBall(float32 power);
    static void playBallToBound(float32 power);
    static void playBallToPocket();
    static void playBallToCue(float32 power);
    static void playMenuHover();
    static void playMenuClick();
    static void playMainTheme();
    static void stop();
    static void play();
    SoundManager();
};


#endif //POOL2D_SOUNDMANAGER_H
