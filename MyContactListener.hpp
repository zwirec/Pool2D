#ifndef BOX_MYCONTACTLISTENER_HPP
#define BOX_MYCONTACTLISTENER_HPP

#include <Box2D/Box2D.h>
#include "MyContactListener.hpp"
#include "util.hpp"
#include "Ball.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class MyContactListener: public b2ContactListener, public b2Fixture {
private:
    std::vector<b2Body*> deletedBalls;
public:
    void BeginContact(b2Contact *contact) override;
    std::vector<b2Body*>& getDeletedBodies() {return deletedBalls;}
    MyContactListener();
};


#endif //BOX_MYCONTACTLISTENER_HPP
