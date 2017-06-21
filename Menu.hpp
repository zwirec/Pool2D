//
// Created by daniil on 10.12.16.
//

#ifndef POOL2D_MENU_HPP
#define POOL2D_MENU_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "TGUI/TGUI.hpp"
#include <vector>

enum SELECTED_ITEM {
    START_GAME,
    SETTING,
    EXIT
};

using namespace sf;

class Menu {
private:
    SELECTED_ITEM item = EXIT;

    Font font;
    Text capture_main;
    Text capture_settings;

    Texture t_background;
    std::vector<Texture> avatars;

    Sprite s_background;

    tgui::Sprite avatar1;
    tgui::Sprite avatar2;

    tgui::Gui gui;
    tgui::Button::Ptr button_ok;

    tgui::Button::Ptr button_newgame;
    tgui::Button::Ptr button_setting;
    tgui::Button::Ptr button_exit;

    tgui::EditBox::Ptr edit_name_1;
    tgui::EditBox::Ptr edit_name_2;

    tgui::Label::Ptr label_1;
    tgui::Label::Ptr label_2;
    tgui::Label::Ptr label_3;

    tgui::SpinButton::Ptr spinAvatar1;
    tgui::SpinButton::Ptr spinAvatar2;

    String name_1 = "PLAYER1";
    String name_2 = "PLAYER2";

    RenderWindow main_menu_window;
    RenderWindow settings_menu_window;
    RenderWindow pause_menu_window;

    void init_button_ok();
    void init_edit_names();
    void init_labels();
    void init_avatars();
public:
    Menu();
    SELECTED_ITEM start_main_menu();
    std::pair<String, String> getNames();
    void start_settings_menu();
    void init_setting_menu();
    void init_main_menu();
    std::pair<Texture &, Texture &> getAvatars();
};


#endif //POOL2D_MENU_HPP
