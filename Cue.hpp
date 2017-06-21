#ifndef POOL2D_CUE_HPP
#define POOL2D_CUE_HPP

#include "util.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
using namespace sf;

class Cue {
private:
    Texture t_cue;
    RectangleShape shape;
    Vector2f size;
    Vector2f position;
    float rotation;
public:
    Cue();
    Cue(Vector2f pos, Vector2f s);
    void update(Vector2f pos, float rot);
    void Draw(RenderWindow& w);
    float getRotation();
    Vector2f getPosition();
    Vector2f getPositionOfPower();
    void setPositionOfPower(Vector2f power);
};


#endif //POOL2D_CUE_HPP
