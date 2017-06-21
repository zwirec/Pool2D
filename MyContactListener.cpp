#include "MyContactListener.hpp"
#include "SoundManager.h"

void MyContactListener::BeginContact(b2Contact *contact) {
    uint16 A = contact->GetFixtureA()->GetFilterData().categoryBits;
    uint16 B = contact->GetFixtureB()->GetFilterData().categoryBits;
    if (A == BALL && B == BALL) {
        b2Body* b1 = contact->GetFixtureB()->GetBody();
        b2Body* b2 = contact->GetFixtureA()->GetBody();
        auto power = b1->GetLinearVelocity().Length() + b2->GetLinearVelocity().Length();
        SoundManager::playBallToBall(power);
    }
    if (A == BOUND && B == BALL || A == BALL && B == BOUND) {
        b2Body *b1 = contact->GetFixtureB()->GetBody();
        b2Body *b2 = contact->GetFixtureA()->GetBody();
        auto power = b1->GetLinearVelocity().Length() + b2->GetLinearVelocity().Length();
        SoundManager::playBallToBound(power);
    }

    if (BALL == A && B == POCKET || B == BALL && A == POCKET) {
        SoundManager::playBallToPocket();
        Ball* ball = static_cast<Ball*>(contact->GetFixtureB()->GetBody()->GetUserData());
        ball->setKill(true);
        deletedBalls.push_back(contact->GetFixtureB()->GetBody());
    }
}

MyContactListener::MyContactListener() {
    SoundManager();
}

