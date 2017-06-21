#include <iostream>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/TGUI.hpp>
#include "InterfaceManager.hpp"
#include "Game.hpp"
#include "SoundManager.h"


void InterfaceManager::init(RenderWindow &window, Game &g) {
    texture_balls.loadFromFile("../image/balls.png");
    texture_background.loadFromFile("../image/blue_background.jpg");
    texture_powerline.loadFromFile("../image/power.png");
    texture_lamp.loadFromFile("../image/lamp.png");
    ball_keeper.loadFromFile("../image/ballskeeper.png", IntRect(6, 46, 186, 108));
    player_board.loadFromFile("../image/board.png");
    board_on_chains.loadFromFile("../image/game_over.png");
    board_pause.loadFromFile("../image/pause.png");
    background.setTexture(texture_background, true);
    background.setScale(0.521, 0.460);
    font.loadFromFile("../font/Instruction.ttf");
    setPowerLine(window);
    setPowerLineBack();
    setPlayerPanel1(Vector2f());
    setPlayerPanel2(Vector2f());
    setLamps();
    sound_on.load("../image/sound_on.png");

    sound_off.load("../image/sound_off.png");

    sound_button = tgui::Button::create();
    sound_button->setSize(30, 30);
    sound_button->setPosition(window.getSize().x - 50, 20);
    sound_button->getRenderer()->setTexture(sound_on);
    sound_button->getRenderer()->setBorderColor(Color::Transparent);
    sound_button->connect("clicked", [this](){
        if (sound_button->getRenderer()->getTexture().getId() == sound_on.getId()) {
            sound_button->getRenderer()->setTexture(sound_off);
            SoundManager::stop();
        }
        else {
            sound_button->getRenderer()->setTexture(sound_on);
            SoundManager::play();
        }
    });

    initPauseWindow(window);

    initGameoverWindow(window, g);

    initGui(window);
}

void InterfaceManager::initGui(RenderWindow &window) {
    gui.setWindow(window);
    gui.setFont(font);
    gui.add(sound_button);
    gui.add(win_pause, "pause");
    gui.add(win_gameover, "gameover");
}

void InterfaceManager::initGameoverWindow(RenderWindow &window, Game &g) {
    win_gameover = tgui::ChildWindow::create();
    win_gameover->setSize(400, 600);
    win_gameover->getRenderer()->setBackgroundColor(Color::Transparent);
    win_gameover->setTitleButtons(tgui::ChildWindow::None);
    win_gameover->getRenderer()->setTitleBarHeight(0);
    win_gameover->getRenderer()->setDistanceToSide(0);
    win_gameover->getRenderer()->setBorders({0, 0});

    win_gameover->getRenderer()->setBorderColor(Color::Transparent);
    win_gameover->setPosition(300, -250);
    win_gameover->showWithEffect(tgui::ShowAnimationType::SlideFromTop, milliseconds(500));
    win_gameover->hide();


    playAgain_button = tgui::Button::copy(resume_button);
    playAgain_button->setText("PLAY AGAIN");
    playAgain_button->setPosition(40, 510);
    playAgain_button->setTextSize(20);
    playAgain_button->connect("mousereleased", [&](){
        win_gameover->hide();
        g.changeGamestate(BALLS_ARE_MOVING);
    });

    exit_button = tgui::Button::copy(playAgain_button);
    exit_button->setText("EXIT");
    exit_button->setPosition(280, 510);
    exit_button->connect("pressed", [&window]() {
        window.close();
    });

    label_gameover = tgui::Label::create("GAME OVER");
    label_gameover->getRenderer()->setTextColor(Color(163, 0, 0));
    label_gameover->setTextSize(34);
    label_gameover->setPosition(112, 422);

    label_whoWin = tgui::Label::create();
    label_whoWin->getRenderer()->setTextColor(Color::Black);
    label_whoWin->setTextSize(24);

    label_score = tgui::Label::copy(label_whoWin);

    boardOnChains_picture = tgui::Picture::create(board_on_chains);
    boardOnChains_picture->setSmooth(true);
    boardOnChains_picture->setSize(400, 600);

    win_gameover->add(boardOnChains_picture);
    win_gameover->add(label_gameover);
    win_gameover->add(label_whoWin);
    win_gameover->add(playAgain_button);
    win_gameover->add(exit_button);
    win_gameover->add(label_score);

    label_pause = tgui::Label::create("PAUSE");
    label_pause->getRenderer()->setTextColor(Color::Black);
    label_pause->setTextSize(42);
    label_pause->setPosition(150, 30);

    win_pause->add(label_pause);
}

void InterfaceManager::initPauseWindow(RenderWindow &window) {
    win_pause = tgui::ChildWindow::create();

    board_picture = tgui::Picture::create(board_pause);
    board_picture->setSmooth(true);
    board_picture->setSize(400, 180);

    win_pause->getRenderer()->setBackgroundColor(Color::Transparent);
    win_pause->setTitleButtons(tgui::ChildWindow::None);
    win_pause->getRenderer()->setTitleBarHeight(0);
    win_pause->getRenderer()->setBorderColor(Color::Transparent);
    win_pause->hide();
    win_pause->setSize(400, 230);
    win_pause->setPosition(VideoMode::getDesktopMode().width / 2 - 380,
                           VideoMode::getDesktopMode().height / 2 - 230);
    win_pause->add(board_picture);

    resume_button = tgui::Button::create("RESUME");
    resume_button->getRenderer()->setBorderColor(Color::Transparent);
    resume_button->getRenderer()->setBackgroundColor(Color::Transparent);
    resume_button->getRenderer()->setBackgroundColorHover(Color::Transparent);
    resume_button->getRenderer()->setBackgroundColorDown(Color::Transparent);
    resume_button->getRenderer()->setTextColor(Color::Black);
    resume_button->getRenderer()->setTextColorHover(Color::Red);
    resume_button->getRenderer()->setTextStyleHover(Text::Bold);
    resume_button->setSize(100, 50);
    resume_button->setTextSize(30);
    resume_button->connect("pressed", [&](){ win_pause->hide();});
    resume_button->setPosition(win_pause->getSize().x / 15, win_pause->getSize().y - 130);

    exit_button = tgui::Button::copy(resume_button);
    exit_button->setText("EXIT");
    exit_button->setPosition(win_pause->getSize().x * 11 / 15, win_pause->getSize().y - 130);
    exit_button->connect("pressed", [&](){window.close();});

    win_pause->add(resume_button);
    win_pause->add(exit_button);
}

void InterfaceManager::setPowerLine(RenderWindow &window) {
    power_line.setSize(Vector2f(30, 300));
    power_line.setOrigin(power_line.getSize().x/2, 0);
    power_line.setPosition(window.getSize().x - 120, window.getSize().y/2 - 175);
    power_line.setTexture(&texture_powerline);
}

void InterfaceManager::setPowerLineBack() {
    power_line_back.setSize(Vector2f(30, 300 - MIN_POWER));
    power_line_back.setOrigin(power_line_back.getSize().x/2, 0);
    power_line_back.setPosition(power_line.getPosition());
    power_line_back.setFillColor(Color::Black);
}

void InterfaceManager::drawPowerLineBack(RenderWindow &window, float power) {
    power_line_back.setSize(Vector2f(30, 300 - power));
    window.draw(power_line_back, BlendAlpha);
}

void InterfaceManager::drawPowerLine(RenderWindow &window) {
    window.draw(power_line);
}

void InterfaceManager::setPlayerPanel1(Vector2f pos) {
    playerPanel1.setSize(Vector2f(200, 110));
    playerPanel1.setOrigin(playerPanel1.getSize().x/2, playerPanel1.getSize().y/2);
    playerPanel1.setPosition(pos);
    playerPanel1.setTexture(&player_board);
    ballPanel1.setSize(Vector2f(186, 108));
    ballPanel1.setOrigin(ballPanel1.getSize().x/2, ballPanel1.getSize().y/2);
    ballPanel1.setPosition(playerPanel1.getPosition().x + playerPanel1.getSize().x/2 + ballPanel1.getSize().x/2 + 5,
                            playerPanel1.getPosition().y + playerPanel1.getSize().y/2 - ballPanel1.getSize().y/2 - 5);
    ballPanel1.setTexture(&ball_keeper);
}

void InterfaceManager::setPlayerPanel2(Vector2f pos) {
    playerPanel2.setSize(Vector2f(200, 110));
    playerPanel2.setOrigin(playerPanel2.getSize().x/2, playerPanel2.getSize().y/2);
    playerPanel2.setPosition(pos);
    playerPanel2.setTexture(&player_board);
    playerPanel2.setScale(-1, 1);
    ballPanel2.setSize(Vector2f(186, 108));
    ballPanel2.setOrigin(ballPanel2.getSize().x/2, ballPanel2.getSize().y/2);
    ballPanel2.setPosition(playerPanel2.getPosition().x - playerPanel2.getSize().x/2 - ballPanel2.getSize().x/2 - 5,
                           playerPanel2.getPosition().y + playerPanel2.getSize().y/2 - ballPanel2.getSize().y/2 - 5);
    ballPanel2.setTexture(&ball_keeper);
    ballPanel2.setScale(-1, 1);
}

void InterfaceManager::drawPlayerPanels(RenderWindow &window) {
    window.draw(playerPanel1);
    window.draw(ballPanel1);
    window.draw(playerPanel2);
    window.draw(ballPanel2);
}

void InterfaceManager::addScoredBall(BALLNUMBER bn) {
    scoredBalls.resize(15);
    scoredBalls[bn] = CircleShape(10.0, 100);
    scoredBalls[bn].setTexture(&texture_balls);
    if (bn <= 3)
        scoredBalls[bn].setTextureRect(IntRect(165 + 596*bn, 176, 530, 530));
    if (bn > 3 && bn <= 7)
        scoredBalls[bn].setTextureRect(IntRect(165 + 596*(bn % 4), 836, 530, 530));
    if (bn > 7 && bn <= 11)
        scoredBalls[bn].setTextureRect(IntRect(165 + 596*(bn % 4), 1500, 530, 530));
    if (bn > 11 && bn <= 14)
        scoredBalls[bn].setTextureRect(IntRect(165 + 596*(bn % 4), 2160, 530, 530));
    scoredBalls[bn].setOrigin(scoredBalls[bn].getRadius(), scoredBalls[bn].getRadius());
}

void InterfaceManager::drawScoredBalls(RenderWindow &window) {
    float step = (ballPanel1.getSize().x - 146.f)/8;
    for(size_t bn = 0; bn < scoredBalls.size(); ++bn) {
        if (bn < 7) {
            scoredBalls[bn].setPosition(ballPanel1.getPosition().x - ballPanel1.getSize().x/2 + 3
                                        + step*(bn+1) + scoredBalls[bn].getRadius() + 2*bn*scoredBalls[bn].getRadius(),
                                        ballPanel1.getPosition().y + 17);
            window.draw(scoredBalls[bn]);
        }
        else {
            scoredBalls[bn].setPosition(ballPanel2.getPosition().x - ballPanel2.getSize().x/2 + 3
                                        + step*(bn-7) + scoredBalls[bn].getRadius() + 2*(bn-8)*scoredBalls[bn].getRadius(),
                                        ballPanel2.getPosition().y + 17);
            window.draw(scoredBalls[bn]);
        }
    }
}

void InterfaceManager::drawPlayersInfo(RenderWindow &window, std::string name1, std::string name2, size_t score1, size_t score2, size_t foul1,
                                      size_t foul2) {
    info_1.setFont(font);
    info_1.setString(name1 + "\n" +
                     "SCORE " + std::to_string(score1) + "\n" +
                     "FOULS " + std::to_string(foul1));
    info_1.setCharacterSize(18);
    info_1.setPosition(playerPanel1.getPosition().x + playerPanel1.getSize().x/2 - info_1.getSize().x - 15,
                       playerPanel1.getPosition().y - playerPanel1.getSize().y/2 + 10);
    info_1.setScale(1, 0.8);
    info_1.draw(window, RenderStates::Default);

    info_2 = info_1;
    info_2.setString(name2 + "\n" +
                     "SCORE " + std::to_string(score2) + "\n" +
                     "FOULS " + std::to_string(foul2));
    info_2.setPosition(playerPanel2.getPosition().x - playerPanel2.getSize().x/2 + 20,
                       playerPanel2.getPosition().y - playerPanel1.getSize().y/2 + 10);
    info_2.draw(window, RenderStates::Default);
    avatar1.draw(window, RenderStates::Default);
    avatar2.draw(window, RenderStates::Default);
}

void InterfaceManager::drawPlayerState(PLAYERSTATE ps, RenderWindow &window) {
    CircleShape shape(10.0, 100);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setTexture(&texture_balls);
    if (ps == PLAYER_1) {
        shape.setPosition(info_1.getPosition().x + info_1.getSize().x/2 - 5,
                          info_1.getPosition().y + info_1.getSize().y + 5);
        shape.setTextureRect(IntRect(165, 176, 530, 530));
        window.draw(shape);
    }
    else {
        shape.setPosition(info_2.getPosition().x + info_2.getSize().x/2 - 5,
                          info_2.getPosition().y + info_2.getSize().y + 5);
        shape.setTextureRect(IntRect(165, 1500, 530, 530));
        window.draw(shape);
    }
}

void InterfaceManager::drawBackground(RenderWindow &window) {
    window.draw(background);
    gui.draw();
}

void InterfaceManager::deleteScoredBalls() {
    scoredBalls.clear();
}

tgui::Gui& InterfaceManager::getGui() {
    return gui;
}

void InterfaceManager::setAvatars(const Texture &t1, const Texture &t2) {
    avatar1.setTexture(t1);
    avatar2.setTexture(t2);
    avatar1.setPosition(playerPanel1.getPosition().x - playerPanel1.getSize().x/2,
                        playerPanel1.getPosition().y - playerPanel1.getSize().y/2 + 5);
    avatar2.setPosition(playerPanel2.getPosition().x + playerPanel2.getSize().x/2 - avatar2.getSize().x/2 - 30,
                        playerPanel2.getPosition().y - playerPanel2.getSize().y/2 + 5);

    avatar1.setScale(0.75, 0.75);
    avatar2.setScale(0.75, 0.75);
}


void InterfaceManager::setWhoWin(const String &winner) {
    label_whoWin->setText(winner + " WIN!");
    label_whoWin->setPosition(win_gameover->getSize().x/2 - label_whoWin->getSize().x/2 + 5, 460);
}

void
InterfaceManager::setGameScore(const String &name1, const String &name2, const size_t score1, const size_t score2) {
    label_score->setText(name1 + "  " + std::to_string(score1) + "-" + std::to_string(score2) + "  " + name2);
    label_score->setPosition(win_gameover->getSize().x/2 - label_score->getSize().x/2 + 5, 490);
}

void InterfaceManager::setLamps() {
    lamp1.setSize(Vector2f(300, 200));
    lamp1.setOrigin(lamp1.getSize().x/2, lamp1.getSize().y/2);
    lamp1.setPosition(300, 30);
    lamp1.setTexture(&texture_lamp);

    lamp2.setSize(Vector2f(300, 200));
    lamp2.setOrigin(lamp1.getSize().x/2, lamp1.getSize().y/2);
    lamp2.setPosition(700, 30);
    lamp2.setTexture(&texture_lamp);
}

void InterfaceManager::drawLamps(RenderWindow &window) {
    window.draw(lamp1);
    window.draw(lamp2);
}


