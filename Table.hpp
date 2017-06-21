
#ifndef BOX_TABLE_HPP
#define BOX_TABLE_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Ball.hpp"

using namespace sf;

const float SCALE = 30.f;
const float OFFSET_X = 57.f,
            OFFSET_Y = 57.f;

class Table {
private:
    Texture t_table;
    RectangleShape sh_table;
    b2Vec2 position;
    b2Vec2 size;
    std::vector<Ball*> balls;
    void CreateBound(b2World& World, float X, float Y, float XLen, float YLen);
    void CreatePocket(b2World& World, float X, float Y, float rad);
    void CreateBalls(RenderWindow& window, b2World& World, float x, float y, float rad);
public:
    b2Vec2 getPos();
    b2Vec2 getSize();
    std::vector<Ball*>& getBalls();
    Table(){};
    Table(RenderWindow& win, b2World& w, b2Vec2 pos, b2Vec2 size);
    void Draw(RenderWindow& w);
    ~Table();

    void RegisterInPhysicWorld(RenderWindow &win, b2World &w, const b2Vec2 &pos, const b2Vec2 &s);
};

void DrawRay(RenderWindow& w, b2World& world, b2Vec2 p1, b2Vec2 p2);

#endif //BOX_TABLE_HPP
