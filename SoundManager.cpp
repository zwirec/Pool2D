#include "MyContactListener.hpp"
#include "SoundManager.h"
SoundBuffer SoundManager::bufferBallToBall;
SoundBuffer SoundManager::bufferBallToBound;
SoundBuffer SoundManager::bufferBallToPocket;
SoundBuffer SoundManager::bufferMenuHover;
SoundBuffer SoundManager::bufferMenuClick;
SoundBuffer SoundManager::bufferCueToBall;
SoundBuffer SoundManager::bufferMainTheme;

Sound SoundManager::soundBallToBall;
Sound SoundManager::soundBallToBound;
Sound SoundManager::soundBallToPocket;
Sound SoundManager::soundCueToBall;
Sound SoundManager::soundMenuHover;
Sound SoundManager::soundMenuClick;
Sound SoundManager::soundMainTheme;

bool SoundManager::sound = true;

void SoundManager::playBallToBall(float32 power) {
    if (sound) {
        soundBallToBall.setVolume(power*10);
        soundBallToBall.play();
    }
}

void SoundManager::playBallToPocket() {
    if (sound) {
        SoundManager::soundBallToPocket.setPlayingOffset(milliseconds(300));
        SoundManager::soundBallToPocket.setVolume(100);
        SoundManager::soundBallToPocket.play();
    }
}

void SoundManager::playBallToBound(float32 power) {
    if (sound) {
        SoundManager::soundBallToBound.setVolume(power*20);
        SoundManager::soundBallToBound.play();
    }
}

SoundManager::SoundManager() {
    SoundManager::bufferBallToBall.loadFromFile("../sound/ball02s.wav");
    SoundManager::soundBallToBall.setBuffer(SoundManager::bufferBallToBall);
    SoundManager::bufferBallToPocket.loadFromFile("../sound/balltopocket.wav");
    SoundManager::soundBallToPocket.setBuffer(SoundManager::bufferBallToPocket);
    SoundManager::bufferBallToBound.loadFromFile("../sound/board02.wav");
    SoundManager::soundBallToBound.setBuffer(SoundManager::bufferBallToBound);
    SoundManager::bufferMenuHover.loadFromFile("../sound/menu.wav");
    SoundManager::soundMenuHover.setBuffer(SoundManager::bufferMenuHover);
    SoundManager::bufferMenuClick.loadFromFile("../sound/menuClick.wav");
    SoundManager::soundMenuClick.setBuffer(SoundManager::bufferMenuClick);
    SoundManager::bufferCueToBall.loadFromFile("../sound/cue02.wav");
    SoundManager::soundCueToBall.setBuffer(SoundManager::bufferCueToBall);
    SoundManager::bufferMainTheme.loadFromFile("../sound/ZigZag.wav");
    SoundManager::soundMainTheme.setBuffer(bufferMainTheme);
    SoundManager::soundMainTheme.setLoop(true);
}

void SoundManager::playMenuHover() {
    if (sound) {
        SoundManager::soundMenuHover.play();
    }
}

void SoundManager::playMenuClick() {
    if (sound) {
        SoundManager::soundMenuClick.play();
    }
}

void SoundManager::playBallToCue(float32 power) {
    if (sound) {
        SoundManager::soundCueToBall.setVolume(power*10);
        SoundManager::soundCueToBall.play();
    }
}

void SoundManager::playMainTheme() {
    if (sound) {
        SoundManager::soundMainTheme.setVolume(10);
        SoundManager::soundMainTheme.play();
    }
}

void SoundManager::stop() {
    SoundManager::soundMainTheme.setVolume(0);
    sound = false;
}

void SoundManager::play() {
    SoundManager::soundMainTheme.setVolume(10);
    sound = true;
}
