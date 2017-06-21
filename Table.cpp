#include "Table.hpp"
#include "util.hpp"
const float widthL = 20;
void Table::CreateBound(b2World &World, float X, float Y, float XLen, float YLen) {
    b2BodyDef BodyDef;
    BodyDef.position.Set(X/PPM, Y/PPM);
    BodyDef.type = b2_staticBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsEdge(b2Vec2(-XLen/2/PPM, -YLen/2/PPM), b2Vec2(XLen/2/PPM, YLen/2/PPM));
    b2FixtureDef FixtureDef;
    FixtureDef.friction = sqrtf(0.5);
    FixtureDef.density = 100.f;
    FixtureDef.restitution = 0.3f;
    FixtureDef.filter.categoryBits = BOUND;
    FixtureDef.shape = &Shape;

    Body->CreateFixture(&FixtureDef);
}

void Table::CreatePocket(b2World &World, float X, float Y, float rad) {
    b2BodyDef BodyDef;
    BodyDef.position.Set(X / PPM, Y / PPM);
    BodyDef.type = b2_staticBody;
    b2Body *Body = World.CreateBody(&BodyDef);
    Body->SetUserData(this);

    b2CircleShape sh_pocket;
    sh_pocket.m_radius = rad/PPM;

    b2FixtureDef fdef;
    fdef.shape = &sh_pocket;
    fdef.isSensor = true;
    fdef.filter.categoryBits = POCKET;
    fdef.filter.maskBits = BALL;
    Body->CreateFixture(&fdef);
}

Table::Table(RenderWindow &win, b2World &w, b2Vec2 pos, b2Vec2 s) {
    position = pos;
    size = s;
    RegisterInPhysicWorld(win, w, pos, s);

    t_table.loadFromFile("../image/table1.png", IntRect(10, 51, 434, 238));
    t_table.setSmooth(true);
    sh_table.setSize(Vector2f(s.x + OFFSET_X, s.y + OFFSET_Y));
    sh_table.setOrigin(sh_table.getSize().x/2, sh_table.getSize().y/2);
    sh_table.setPosition(getPos().x, getPos().y);
    sh_table.setTexture(&t_table, true);
}

void Table::RegisterInPhysicWorld(RenderWindow &win, b2World &w, const b2Vec2 &pos, const b2Vec2 &s) {
    CreateBound(w, pos.x - s.x / 2 - 2, pos.y, 0, s.y - widthL * 2);
    CreateBound(w, pos.x - s.x/4, pos.y + s.y/2-2, s.x/2 - widthL - 26, 0);
    CreateBound(w, pos.x + s.x/4, pos.y + s.y/2-2, s.x/2 - widthL - 26, 0);
    CreateBound(w, pos.x + s.x/2, pos.y, 0, s.y - widthL*2);
    CreateBound(w, pos.x + s.x/4, pos.y - s.y/2-2, s.x/2 - widthL - 26, 0);
    CreateBound(w, pos.x - s.x/4, pos.y - s.y/2-2, s.x/2 - widthL - 26, 0);

    CreatePocket(w, getPos().x - size.x / 2 - 10, getPos().y - size.y / 2 - 10, 12);
    CreatePocket(w, getPos().x - size.x / 2 - 8, getPos().y + size.y / 2 + 8, 12);
    CreatePocket(w, getPos().x - 2, getPos().y + size.y / 2 + 12, 12);
    CreatePocket(w, getPos().x - 2, getPos().y - size.y / 2 - 12, 12);
    CreatePocket(w, getPos().x + size.x / 2 + 5, getPos().y + size.y / 2 + 8, 12);
    CreatePocket(w, getPos().x + size.x / 2 + 5, getPos().y - size.y / 2 - 10, 12);
    CreateBalls(win, w, getPos().x - 100, getPos().y, 10.0);
}

void Table::Draw(RenderWindow& w) {
    w.draw(sh_table);
}

b2Vec2 Table::getPos() {
    return position;
}

b2Vec2 Table::getSize() {
    return size;
}

void DrawRay(RenderWindow &w, b2World &world, b2Vec2 p1, b2Vec2 p2) {
    b2RayCastInput input;
    input.p1 = b2Vec2(p1.x, p1.y);
    input.p2 = b2Vec2(p2.x, p2.y);
    input.maxFraction = 1;

    float closestFraction = 1;
    b2Vec2 intersectionNormal(0, 0);
    for (b2Body *b = world.GetBodyList(); b; b = b->GetNext()) {
        for (b2Fixture *f = b->GetFixtureList(); f; f = f->GetNext()) {
            b2RayCastOutput output;
            if (!f->RayCast(&output, input))
                continue;
            if (output.fraction < closestFraction) {
                closestFraction = output.fraction;
                intersectionNormal = output.normal;
            }
        }
    }

    b2Vec2 intersectionPoint = p1 + closestFraction * (p2 - p1);
    CircleShape point;
    point.setRadius(3.0);
    point.setFillColor(Color(0, 0, 0, 0));
    point.setOutlineThickness(1.0);
    point.setOutlineColor(Color::White);
    point.setOrigin(point.getRadius(), point.getRadius());
    point.setPosition(intersectionPoint.x * PPM , intersectionPoint.y * PPM );
    point.setFillColor(Color(255, 255, 240, 160));


    Vertex ray[]{
            Vertex(Vector2f(p1.x * PPM, p1.y * PPM), Color::Transparent),
            Vertex(Vector2f(intersectionPoint.x * PPM , intersectionPoint.y * PPM ), Color::Green),

    };
    w.draw(ray, 2, Lines);
    w.draw(point);
}

void Table::CreateBalls(RenderWindow &window, b2World &World, float x, float y, float rad) {
    Ball::setTexture();
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_16, CUEBALL));
    x += 185;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_1, SOLID));
    x += 2 * rad - 1;
    y -= rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_11, STRIP));
    y += 2 * rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_3, SOLID));
    x += 2 * rad - 1;
    y += rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_14, STRIP));
    y -= 2 * rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_8, SOLID));
    y -= 2 * rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_6, SOLID));
    x += 2 * rad;
    y -= rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_13, STRIP));
    y += 2 * rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_15, STRIP));
    y += 2 * rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_4, SOLID));
    y += 2 * rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_9, STRIP));
    x += 2 * rad;
    y += rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_12, STRIP));
    y -= 2 * rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_5, SOLID));
    y -= 2 * rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_10, STRIP));
    y -= 2 * rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_2, SOLID));
    y -= 2 * rad;
    balls.push_back(new Ball(window, World, b2Vec2(x, y), rad, BALL_7, SOLID));
}

std::vector<Ball*> &Table::getBalls() {
    return balls;
}

Table::~Table() {
    for (auto i = 0; i < balls.size(); ++i) {
        delete balls[i];
    }
}

