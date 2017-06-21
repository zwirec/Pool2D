#include "Cue.hpp"

Cue::Cue() {

}

Cue::Cue(Vector2f pos, Vector2f s):position(pos), rotation(180), size(s) {
    t_cue.loadFromFile("/home/daniil/opengl/Box2D_v2.1.2/box/image/cue.png", IntRect(77, 207, 1539, 21));
    t_cue.setSmooth(true);
    shape.setSize(s);
    shape.setOrigin(-15.0f, shape.getSize().y / 2);
    shape.setPosition(position);
    shape.setRotation(rotation);
    shape.setTexture(&t_cue);
}

void Cue::update(Vector2f pos, float rot) {
    rotation = rot;
    shape.setRotation(rot);
    shape.setPosition(pos);
}

void Cue::Draw(RenderWindow &w) {
    w.draw(shape);
}

float Cue::getRotation() {
    return rotation;
}

Vector2f Cue::getPositionOfPower() {
    return shape.getOrigin();
}

void Cue::setPositionOfPower(Vector2f power) {
    shape.setOrigin(power);
}

Vector2f Cue::getPosition() {
    return position;
}

