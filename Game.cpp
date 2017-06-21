#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include "Game.hpp"
#include "SFGUI/SFGUI.hpp"
#include "SoundManager.h"

Game::Game():
        world(b2Vec2(0.0, 0.0), true){
    world.SetContactListener(&myContactListener);
    SoundManager::playMainTheme();
}

void Game::start(){
    choosingMenuItem();
    while (game_window.isOpen()) {
        Event e;
        while (game_window.pollEvent(e)) {
            interfaceManager.getGui().handleEvent(e);
            if (e.type == Event::Closed) {
                stop();
                game_window.close();
            }
            //если нажата кнопка мыши и шар не "в руке"
            if (e.type == Event::MouseButtonReleased && gamestate == BEFORE_HIT &&
                    !interfaceManager.getGui().handleEvent(e) && gamestate != PAUSE)
                if (e.mouseButton.button == Mouse::Left) {
                    needChange = true;
                    gamestate = BALLS_ARE_MOVING;
                    SoundManager::playBallToCue(power);
                    hitCueBall();
                }
            //если нажата кнопка мыши и шар "в руке"
            if (e.type == Event::MouseButtonReleased && gamestate == CUEBALL_IN_HAND) {
                auto count = std::count_if(table->getBalls().begin(),
                                           table->getBalls().end(),
                                           [this](Ball* b) {
                                               return cueBall->getShape().getGlobalBounds().intersects
                                                       (b->getShape().getGlobalBounds());
                                           });
                if (count == 1) {
                    gamestate = BEFORE_HIT;
                    cueBall->getBody()->SetActive(true);
                }
            }

            //крутим колесико мыши для увелечения силы удара
            if (e.type == Event::MouseWheelScrolled && gamestate == BEFORE_HIT) {
                if (e.mouseWheelScroll.delta >= 0 && power < MAX_POWER) {
                    power += STEP_POWER;
                } else if (e.mouseWheelScroll.delta < 0 && power > MIN_POWER) {
                    power -= STEP_POWER;
                }
                cue->setPositionOfPower(Vector2f(-10.0f - power/5, cue->getPositionOfPower().y));
            }
            if (e.type == Event::KeyReleased && e.key.code == Keyboard::Key::Escape) {
                auto win_pause = interfaceManager.getGui().get("pause");
                interfaceManager.setWhoWin("PLAYER1");
                interfaceManager.setGameScore(player1.getName(), player2.getName(), 0, 0);
                win_pause->show();
                while (win_pause->isVisible()) {
                    game_window.waitEvent(e);
                    sleep(microseconds(1000));
                    interfaceManager.getGui().handleEvent(e);
                    interfaceManager.getGui().draw();
                    game_window.display();
                }
            }
        }
        RenderInterface();
        table->Draw(game_window);

        world.Step(1/60.f, 8, 3);

        if (isExistMovingBall())
            gamestate = BALLS_ARE_MOVING;
        else if (gamestate == BALLS_ARE_MOVING)
            gamestate = BEFORE_HIT;

        if (gamestate == CUEBALL_IN_HAND) {
            processInHandState();
        }

        removeScoredBalls();
        moveBalls();

        if (gamestate == BEFORE_HIT) {
            processBeforeHit();
             if (gamestate == GAME_OVER) {
                 processInGameoverState();
             }
        }
        interfaceManager.getGui().draw();
        game_window.display();
    }
}

void Game::RenderInterface() {
    interfaceManager.drawBackground(game_window);
    interfaceManager.drawPowerLine(game_window);
    interfaceManager.drawPowerLineBack(game_window, power);
    interfaceManager.drawPlayerPanels(game_window);
    interfaceManager.drawScoredBalls(game_window);
    interfaceManager.drawPlayersInfo(game_window, player1.getName(), player2.getName(), player1.getNumScoredBalls(),
                                    player2.getNumScoredBalls(), player1.getNumFouls(), player2.getNumFouls());
    interfaceManager.drawPlayerState(playerstate, game_window);
    interfaceManager.drawLamps(game_window);
}

void Game::hitCueBall() {
    b2Body* b = cueBall->getBody();
    b2Vec2 pos = b2Vec2(-(Mouse::getPosition(game_window).x - b->GetPosition().x * PPM),
                        -(Mouse::getPosition(game_window).y - b->GetPosition().y * PPM));
    pos.Normalize();
    b->ApplyLinearImpulse(b2Vec2(pos.x * power/2, pos.y * power/2), b->GetWorldCenter());
}

bool Game::isExistMovingBall() {
    auto count = std::count_if(table->getBalls().begin(), table->getBalls().end(),
                               [](Ball* b){
                                   return b->getBody()->GetLinearVelocity().x != 0 &&
                                          b->getBody()->GetLinearVelocity().y != 0;});
    return count != 0;
}

void Game::removeScoredBalls() {
    std::vector<b2Body*> bodies = std::move(myContactListener.getDeletedBodies());

    for(int i = 0; i < bodies.size(); i++) {
        Ball* b = (Ball*)bodies[i]->GetUserData();
        if (b->getType() != CUEBALL)
            deleted_balls.push_back((Ball*)bodies[i]->GetUserData());
        table->getBalls().erase(std::remove(table->getBalls().begin(), table->getBalls().end(), b),
                                table->getBalls().end());
        world.DestroyBody(b->getBody());
    }
}

void Game::moveBalls() {
    for (auto &it: table->getBalls()) {
        b2Vec2 vel = it->getBody()->GetLinearVelocity();
        vel.Normalize();
        float velChangeX = -vel.x;
        float velChangeY = -vel.y;
        it->getBody()->ApplyForce(b2Vec2(velChangeX * 0.02f * it->getBody()->GetMass() * 9.81f,
                                         velChangeY * 0.02f * it->getBody()->GetMass() * 9.81f),
                                  it->getBody()->GetWorldCenter());
        float velAng = it->getBody()->GetAngularVelocity();
        if (fabs(velAng) > 0.2)
            it->getBody()->ApplyTorque(-(velAng/10));
        else it->getBody()->SetAngularVelocity(0);
        if (!it->isKill())
            it->Draw(game_window);
    }
}

void Game::retrieveDeletedBalls(BALLTYPE bt) {
    for (size_t i = 0; i < deleted_balls.size(); ++i) {
        Ball *b = deleted_balls[i];
        if (b->getType() != bt) {
            deleted_balls.erase(std::remove(deleted_balls.begin(), deleted_balls.end(), b), deleted_balls.end());
            b = new Ball(game_window, world, b2Vec2(table->getPos().x - 100, table->getPos().y), 10.0,
                         b->getNumber(), b->getType());
            b->setKill(false);
            table->getBalls().push_back(b);
        }
        else
            interfaceManager.addScoredBall(b->getNumber());
    }
}

void Game::retrievedCueBall() {
    cueBall = new Ball(game_window, world, b2Vec2(table->getPos().x - 100, table->getPos().y), 10.0, BALL_16,
                       CUEBALL);
    cueBall->setKill(false);
    table->getBalls().push_back(cueBall);
    cueBall->getBody()->SetActive(false);
    gamestate = CUEBALL_IN_HAND;
}

bool Game::isForeignScoredBalls(Player& player) {
    auto count = std::count_if(deleted_balls.begin(), deleted_balls.end(),
                               [&player](Ball *b) {
                                   return b->getType() != player.getOwnType();
                               });
    return count != 0;
}

void Game::processBeforeHit() {
    //если была неправильно забита восьмерка
    if (uncorrectScoredEightBall(playerstate)) {
        gamestate = GAME_OVER;
        playerstate = (PLAYERSTATE) !playerstate;
        needChange = false;
        return;
    }
    //если была правильно забита восьмерка
    if (correctScoredEightBall(playerstate)) {
        gamestate = GAME_OVER;
        needChange = false;
        return;
    }
    //если был забит биток
    if (cueBall->isKill()) {
        retrievedCueBall();
        if (playerstate == PLAYER_1) {
            retrieveDeletedBalls(player1.getOwnType());
            player1.addNumScoredBalls(deleted_balls.size());
            player1.addNumFouls();
            playerstate = (PLAYERSTATE) !playerstate;
            needChange = false;
        }
        else {
            retrieveDeletedBalls(player2.getOwnType());
            player2.addNumFouls();
            player2.addNumScoredBalls(deleted_balls.size());
            playerstate = (PLAYERSTATE) !playerstate;
            needChange = false;
        }
    }
        //если не было забито шаров
    else if (deleted_balls.empty() && needChange) {
        playerstate = (PLAYERSTATE) !playerstate;
        needChange = false;
        return;
    }
    //если бил первый игрок и были забитые шары
    else if (playerstate == PLAYER_1 && !deleted_balls.empty()) {

        if (!isForeignScoredBalls(player1)) {
            player1.addNumScoredBalls(deleted_balls.size());
            retrieveDeletedBalls(player1.getOwnType());
            needChange = false;
        }
        else {
            retrieveDeletedBalls(player1.getOwnType());
            player1.addNumScoredBalls(deleted_balls.size());
            player1.addNumFouls();
            playerstate = PLAYER_2;
            needChange = false;
        }
    }
    //если бил второй игрок и были забитые шары
    else if (playerstate == PLAYER_2 && !deleted_balls.empty()) {

        if (!isForeignScoredBalls(player2)) {
            player2.addNumScoredBalls(deleted_balls.size());
            retrieveDeletedBalls(player2.getOwnType());
            needChange = false;
        }
        else {
            retrieveDeletedBalls(player2.getOwnType());
            player2.addNumScoredBalls(deleted_balls.size());
            player2.addNumFouls();
            playerstate = PLAYER_1;
            needChange = false;
        }
    }

    deleted_balls.clear();

    cue->update(Vector2f(cueBall->getBody()->GetPosition().x * PPM, cueBall->getBody()->GetPosition().y * PPM),
               atan2f((Mouse::getPosition(game_window).y - cueBall->getBody()->GetPosition().y * PPM),
                      (Mouse::getPosition(game_window).x - cueBall->getBody()->GetPosition().x * PPM))
               * RADTODEG);

    float rayLength = power * 2;
    b2Vec2 p1(cueBall->getBody()->GetPosition().x, cueBall->getBody()->GetPosition().y);
    b2Vec2 p2 = p1 + rayLength / PPM * b2Vec2(sinf(-(90 - cue->getRotation()) * DEGTORAD),
                                              cosf((90 + cue->getRotation()) * DEGTORAD));
    cue->Draw(game_window);
    DrawRay(game_window, world, p1, p2);

}

bool Game::uncorrectScoredEightBall(PLAYERSTATE ps) {
    auto eightBall = std::find_if(deleted_balls.begin(), deleted_balls.end(),
                             [](Ball* b) {
                                 return b->getNumber() == BALL_8;
                             });
    std::vector<Ball*>::iterator ball;
    if (ps == PLAYER_1) {
        ball = std::find_if(table->getBalls().begin(), table->getBalls().end(),
                            [this](Ball* b) {
                                return b->getType() == player1.getOwnType();
        });
    }
    else {
        ball = std::find_if(table->getBalls().begin(), table->getBalls().end(),
                            [this](Ball* b) {
                                return b->getType() == player2.getOwnType();
                            });
    }

    return eightBall != deleted_balls.end() && ball != table->getBalls().end();
}

bool Game::correctScoredEightBall(PLAYERSTATE ps) {
    auto eightBall = std::find_if(deleted_balls.begin(), deleted_balls.end(),
                                  [](Ball* b) {
                                      return b->getNumber() == BALL_8;
                                  });
    std::vector<Ball*>::iterator ball;
    if (ps == PLAYER_1) {
        ball = std::find_if(table->getBalls().begin(), table->getBalls().end(),
                            [this](Ball* b) {
                                return b->getType() == player1.getOwnType();
                            });
    }
    else {
        ball = std::find_if(table->getBalls().begin(), table->getBalls().end(),
                            [this](Ball* b) {
                                return b->getType() == player2.getOwnType();
                            });
    }

    return eightBall != deleted_balls.end() && ball == table->getBalls().end();
}


void Game::processInHandState() {
    b2Body *b = cueBall->getBody();
    auto leftBound = (table->getPos().x - table->getSize().x/2);
    auto rightBound = (table->getPos().x + table->getSize().x/2);
    auto topBound = (table->getPos().y - table->getSize().y/2);
    auto downBound = (table->getPos().y + table->getSize().y/2);

    auto mx = Mouse::getPosition(game_window).x;
    auto my = Mouse::getPosition(game_window).y;

    if (mx - 10 < leftBound)
        Mouse::setPosition(Vector2i((int) leftBound + 10, my), game_window);

    if (mx + 10 > rightBound)
        Mouse::setPosition(Vector2i((int) rightBound - 10, my), game_window);

    if (my - 10 < topBound)
        Mouse::setPosition(Vector2i(mx, (int) (topBound + 10)), game_window);

    if (my + 10 > downBound)
        Mouse::setPosition(Vector2i(mx, (int) (downBound - 10)), game_window);

    if (mx - 10 < leftBound && my - 10 < topBound)
        Mouse::setPosition(Vector2i((int) leftBound + 10, (int) (topBound + 10)), game_window);

    if (mx - 10 < leftBound && my + 10 > downBound)
        Mouse::setPosition(Vector2i((int) leftBound + 10, (int) (downBound - 10)), game_window);

    if (mx + 10 > rightBound && my + 10 > downBound)
        Mouse::setPosition(Vector2i((int) rightBound - 10, (int) (downBound - 10)), game_window);

    if (mx + 10 > rightBound && my - 10 < topBound)
        Mouse::setPosition(Vector2i((int) rightBound - 10, (int) (topBound + 10)), game_window);

    auto _mx = Mouse::getPosition(game_window).x;
    auto _my = Mouse::getPosition(game_window).y;

    b->SetTransform(b2Vec2(_mx / PPM, _my / PPM), 0);
}

void Game::init() {
    game_window.setFramerateLimit(60);
    table = std::unique_ptr<Table>(new Table(game_window, world,
                                             b2Vec2(game_window.getSize().x / 2, game_window.getSize().y / 2 - 20),
                                             b2Vec2(500, 250)));

    gamestate = BEFORE_HIT;

    auto names = menu.getNames();
    auto avatars = menu.getAvatars();

    if (names.first.isEmpty())
        player1.setName("PLAYER1");
    else
        player1.setName(menu.getNames().first);

    if (names.second.isEmpty())
        player2.setName("PLAYER2");
    else
        player2.setName(menu.getNames().second);

    player1.setAvatar(avatars.first);
    player2.setAvatar(avatars.second);

    player1.setOwnType(SOLID);
    player2.setOwnType(STRIP);

    cueBall = table->getBalls()[0];
    cue = std::unique_ptr<Cue>(new Cue(Vector2f(cueBall->getBody()->GetPosition().x * PPM, cueBall->getBody()->GetPosition().y * PPM),
                  Vector2f(300, 4.5)));
    interfaceManager.setPlayerPanel1(Vector2f(110, game_window.getSize().y - 60));
    interfaceManager.setPlayerPanel2(Vector2f(game_window.getSize().x - 110, game_window.getSize().y - 60));
    interfaceManager.setAvatars(avatars.first, avatars.second);
}

void Game::stop() {
    for(b2Body* b = world.GetBodyList(); b != 0; b = b->GetNext())
        world.DestroyBody(b);
    deleted_balls.clear();
    table->getBalls().clear();
    interfaceManager.deleteScoredBalls();
    player1.reset();
    player2.reset();
}

void Game::changeGamestate(GAMESTATE gs) {
    gamestate = gs;
}

void Game::processInGameoverState() {
    gamestate = PAUSE;
    stop();
    if (playerstate == PLAYER_1) {
        ++game_score.first;
        interfaceManager.setWhoWin(player1.getName());
    }
    else {
        ++game_score.second;
        interfaceManager.setWhoWin(player2.getName());
    }
    interfaceManager.setGameScore(player1.getName(), player2.getName(), game_score.first, game_score.second);
    auto win_pause = interfaceManager.getGui().get("gameover");
    win_pause->show();
    init();
}

void Game::choosingMenuItem() {
    auto choose = menu.start_main_menu();
    if (choose == EXIT) {
        game_window.close();
        return;
    }
    if (choose == START_GAME) {
        game_window.create(VideoMode(width, height), "Pool 2D", Style::Default, ContextSettings(0, 0, 8));
        interfaceManager.init(game_window, *this);
        init();
        game_window.setPosition(Vector2i(VideoMode::getDesktopMode().width/2 - 500,
                                         VideoMode::getDesktopMode().height/2 - 225));
    }
}

