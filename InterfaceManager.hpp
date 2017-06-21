#ifndef POOL2D_DRAWERINTERFACE_HPP
#define POOL2D_DRAWERINTERFACE_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include "util.hpp"


using namespace sf;
class Game;

class InterfaceManager {
private:
    std::vector<CircleShape> scoredBalls;
    Texture texture_balls;
    Texture texture_background;
    Texture texture_powerline;
    Texture ball_keeper;
    Texture player_board;
    Texture board_on_chains;
    Texture board_pause;
    Texture texture_lamp;
    tgui::Texture sound_on;
    tgui::Texture sound_off;


    tgui::Gui gui;

    tgui::Picture::Ptr boardOnChains_picture;
    tgui::Picture::Ptr board_picture;
    Sprite background;

    Font font;
    tgui::Text info_1;
    tgui::Text info_2;

    tgui::ChildWindow::Ptr win_pause;
    tgui::ChildWindow::Ptr win_gameover;
    tgui::Button::Ptr resume_button;
    tgui::Button::Ptr exit_button;

    tgui::Button::Ptr playAgain_button;
    tgui::Button::Ptr sound_button;

    tgui::Sprite avatar1;
    tgui::Sprite avatar2;

    tgui::Label::Ptr label_pause;
    tgui::Label::Ptr label_gameover;
    tgui::Label::Ptr label_score;
    tgui::Label::Ptr label_whoWin;

    RectangleShape power_line;
    RectangleShape power_line_back;
    RectangleShape playerPanel1;
    RectangleShape lamp1;
    RectangleShape lamp2;
    RectangleShape ballPanel1;
    RectangleShape playerPanel2;
    RectangleShape ballPanel2;
public:
    InterfaceManager(){};
    void init(RenderWindow &window, Game& g);
    void setPowerLine(RenderWindow &window);
    void setPowerLineBack();
    void setPlayerPanel1(Vector2f pos);
    void setPlayerPanel2(Vector2f pos);
    void setAvatars(const Texture& t1, const Texture& t2);
    void setLamps();
    void drawPowerLineBack(RenderWindow &window, float power);
    void drawPowerLine(RenderWindow &window);
    void drawLamps(RenderWindow &window);
    void drawPlayerPanels(RenderWindow &window);
    void drawPlayersInfo(RenderWindow &window, std::string name1, std::string name2,
                         size_t count1, size_t count2,
                         size_t foul1, size_t foul2);
    void drawPlayerState(PLAYERSTATE ps, RenderWindow &window);
    void setGameScore(const String& name1, const String& name2, const size_t score1, const size_t score2);
    void setWhoWin(const String& winner);
    void addScoredBall(BALLNUMBER bn);
    void drawScoredBalls(RenderWindow &window);
    void drawBackground(RenderWindow &window);
    void deleteScoredBalls();
    tgui::Gui& getGui();

    void initPauseWindow(RenderWindow &window);

    void initGameoverWindow(RenderWindow &window, Game &g);

    void initGui(RenderWindow &window);
};


#endif //POOL2D_DRAWERINTERFACE_HPP
