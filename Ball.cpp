#include "Ball.hpp"
#include "util.hpp"

bool load = false;

Texture Ball::texture = Texture();

Ball::Ball(sf::RenderWindow& window, b2World& world, b2Vec2 p, float rad, BALLNUMBER num, BALLTYPE t):
        number(num), startPos(p), currPos(p), type(t) {
    
    bDef.position = b2Vec2(startPos.x / PPM, startPos.y / PPM);
    bDef.type = b2_dynamicBody;
    bDef.allowSleep = true;
    Body = world.CreateBody(&bDef);
    Body->SetUserData(this);

    b2CircleShape Shape;
    Shape.m_radius = rad/PPM;
    b2FixtureDef FixtureDef;
    FixtureDef.density = 20;
    FixtureDef.friction = 1.0f;
    FixtureDef.restitution = 0.5f;
    Body->SetBullet(true);
    FixtureDef.shape = &Shape;
    FixtureDef.filter.categoryBits = BALL;
    Body->CreateFixture(&FixtureDef);
    cshape.setRadius(rad);
    cshape.setPointCount(1000);
    cshape.setOrigin(cshape.getRadius(), cshape.getRadius());
    cshape.setTexture(&Ball::texture);
    if (num <= 3)
        cshape.setTextureRect(IntRect(165 + 596*num, 176, 530, 530));
    if (num > 3 && num <= 7)
        cshape.setTextureRect(IntRect(165 + 596*(num % 4), 836, 530, 530));
    if (num > 7 && num <=11)
        cshape.setTextureRect(IntRect(165 + 596*(num % 4), 1500, 530, 530));
    if (num > 11 && num <=15)
        cshape.setTextureRect(IntRect(165 + 596*(num % 4), 2160, 530, 530));
    Draw(window);
}

void Ball::RegisterInPhysicWorld(b2World &world, float rad) {

}

void Ball::Draw(RenderWindow& w) {
    cshape.setPosition(PPM * Body->GetPosition().x, PPM * Body->GetPosition().y);
    cshape.setRotation(Body->GetAngle() * 180 / b2_pi);
    w.draw(cshape);
}

b2Body *Ball::getBody() {
    return Body;
}

bool Ball::isKill() {
    return kill;
}

void Ball::setKill(bool b) {
    kill = b;
}

BALLTYPE Ball::getType() {
    return type;
}

BALLNUMBER Ball::getNumber() {
    return number;
}

Shape &Ball::getShape() {
    return cshape;
}

void Ball::setTexture() {
    Ball::texture.loadFromFile("../image/balls.png");
    Ball::texture.setSmooth(true);
}

