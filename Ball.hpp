#ifndef BOX_BALL_HPP
#define BOX_BALL_HPP
#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "util.hpp"

using namespace sf;
//const float SCALE = 30.f;
class Ball {
private:
    Sprite s;
    BALLNUMBER number;
    BALLTYPE type;
    CircleShape cshape;
    b2Vec2 startPos;
    b2Vec2 currPos;
    float rad;
    b2Body* Body;
    b2BodyDef bDef;
    b2FixtureDef fdef;
    b2CircleShape shape;
    static Texture texture;
    bool kill = false;
public:
    bool isKill();
    void setKill(bool b);
    BALLTYPE getType();
    b2Body* getBody();
    Shape& getShape();
    Ball(RenderWindow& win, b2World& w, b2Vec2 pos, float rad, BALLNUMBER num, BALLTYPE type);
    Ball(){};
    ~Ball(){};
    void Draw(RenderWindow& w);
    static void setTexture();
    BALLNUMBER getNumber();

    void RegisterInPhysicWorld(b2World &world, float rad);
};



#endif //BOX_BALL_HPP
