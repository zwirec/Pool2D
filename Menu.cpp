#include "TGUI/TGUI.hpp"
#include "Menu.hpp"
#include "SoundManager.h"


SELECTED_ITEM Menu::start_main_menu() {
    while (main_menu_window.isOpen()) {
        Event e;
        main_menu_window.draw(s_background);
        while (main_menu_window.pollEvent(e)) {
            if (e.type == Event::Closed ||
                (e.type == Event::KeyReleased && e.key.code == Keyboard::Escape)) {
                main_menu_window.close();
            }
            gui.handleEvent(e);
        }
        main_menu_window.draw(capture_main);
        gui.draw();
        main_menu_window.display();
    }
    return item;
}

Menu::Menu() {
    SoundManager();
    init_main_menu();
    t_background.loadFromFile("../image/menu.jpg");
    s_background.setTexture(t_background);
}


void Menu::start_settings_menu() {
    while (settings_menu_window.isOpen()) {
        settings_menu_window.draw(s_background);
        settings_menu_window.draw(capture_settings);
        Event e;
        while (settings_menu_window.pollEvent(e)) {
            if (e.type == Event::Closed ||
                (e.type == Event::KeyReleased && e.key.code == Keyboard::Escape)) {
                settings_menu_window.close();
                gui.remove(edit_name_1);
                gui.remove(edit_name_2);
            }

            if (e.type == Event::MouseButtonReleased && e.mouseButton.button == Mouse::Left) {

            }
            gui.handleEvent(e);
        }
        avatar1.draw(settings_menu_window, RenderStates::Default);
        avatar2.draw(settings_menu_window, RenderStates::Default);
        gui.draw();
        settings_menu_window.display();
    }

}

void Menu::init_main_menu() {
    main_menu_window.create(VideoMode(510, 340), "Pool 2D", Style::None, ContextSettings(0, 0, 8));
    main_menu_window.setPosition(Vector2i(VideoMode::getDesktopMode().width / 2 - 255,
                                          VideoMode::getDesktopMode().height / 2 - 170));
    avatars.resize(10);
    for (size_t i = 0; i < avatars.size(); ++i) {
        avatars[i].loadFromFile("../image/avatars/" + std::to_string(i + 1) + ".png");
    }

    spinAvatar1 = tgui::SpinButton::create(0, 9);
    spinAvatar2 = tgui::SpinButton::create(0, 9);

    font.loadFromFile("../font/Instruction.ttf");
    capture_main.setFont(font);
    capture_main.setCharacterSize(52);
    capture_main.setColor(Color::Black);
    capture_main.setString("POOL 2D");
    capture_main.setOrigin(capture_main.getLocalBounds().width / 2, capture_main.getLocalBounds().height / 2);
    capture_main.setPosition(main_menu_window.getSize().x / 2, 20);

    button_newgame = tgui::Button::create("NEW GAME");
    button_newgame->setSize(100, 50);
    button_newgame->setTextSize(32);
    button_newgame->getRenderer()->setBorderColor(Color::Transparent);
    button_newgame->getRenderer()->setBackgroundColor(Color::Transparent);
    button_newgame->getRenderer()->setBackgroundColorHover(Color::Transparent);
    button_newgame->getRenderer()->setBackgroundColorDown(Color::Transparent);
    button_newgame->getRenderer()->setTextColor(Color::Black);
    button_newgame->getRenderer()->setTextColorHover(Color::Red);
    button_newgame->getRenderer()->setTextStyleHover(Text::Bold);
    button_newgame->setPosition(main_menu_window.getSize().x / 2 - button_newgame->getSize().x / 2, 120);
    button_newgame->connect("pressed", [&]() {
        SoundManager::playMenuClick();
        item = START_GAME;
        main_menu_window.close();
    });
    button_newgame->connect("mouseEntered", [&]() {
        SoundManager::playMenuHover();
    });

    button_setting = tgui::Button::copy(button_newgame);
    button_setting->setText("SETTINGS");
    button_setting->setPosition(main_menu_window.getSize().x / 2 - button_setting->getSize().x / 2, 185);
    button_setting->connect("pressed", [&]() {
        item = SETTING;
        init_setting_menu();
        start_settings_menu();
    });


    gui.removeAllWidgets();
    gui.setWindow(main_menu_window);
    gui.setFont(font);
    gui.add(button_newgame);
    gui.add(button_setting);
}

void Menu::init_setting_menu() {
    settings_menu_window.create(VideoMode(510, 340), "Pool 2D", Style::None, ContextSettings(0, 0, 8));
    settings_menu_window.setPosition(Vector2i(VideoMode::getDesktopMode().width / 2 - 255,
                                              VideoMode::getDesktopMode().height / 2 - 170));
    capture_settings.setFont(font);
    capture_settings.setCharacterSize(52);
    capture_settings.setString("SETTINGS");
    capture_settings.setColor(Color::Black);
    capture_settings.setOrigin(capture_settings.getLocalBounds().width / 2,
                               capture_settings.getLocalBounds().height / 2);
    capture_settings.setPosition(settings_menu_window.getSize().x / 2, 20);
    gui.removeAllWidgets();
    gui.setWindow(settings_menu_window);
    gui.setFont(font);

    init_button_ok();
    init_labels();
    init_edit_names();
    init_avatars();

    gui.add(button_ok);
    gui.add(label_1);
    gui.add(label_2);
    gui.add(edit_name_1);
    gui.add(edit_name_2);
    gui.add(spinAvatar1);
    gui.add(spinAvatar2);
}

std::pair<String, String> Menu::getNames() {
    if (edit_name_1 == nullptr)
        return std::pair<String, String>(String(), String());
    return std::pair<String, String>(edit_name_1->getText(), edit_name_2->getText());
}

std::pair<Texture &, Texture &> Menu::getAvatars() {
    return std::pair<Texture &, Texture &>(avatars[spinAvatar1->getValue()], avatars[spinAvatar2->getValue()]);
}

void Menu::init_button_ok() {
    button_ok = tgui::Button::create("OK");
    button_ok->getRenderer()->setBorderColor(Color::Transparent);
    button_ok->getRenderer()->setBackgroundColor(Color::Transparent);
    button_ok->getRenderer()->setBackgroundColorHover(Color::Transparent);
    button_ok->getRenderer()->setBackgroundColorDown(Color::Transparent);
    button_ok->getRenderer()->setTextColor(Color::Black);
    button_ok->getRenderer()->setTextColorHover(Color::Red);
    button_ok->getRenderer()->setTextStyleHover(Text::Bold);
    button_ok->setSize(75, 75);
    button_ok->setPosition(settings_menu_window.getSize().x / 2 - button_ok->getSize().x / 2,
                           settings_menu_window.getSize().y - 100);
    button_ok->connect("pressed", [&]() {
        SoundManager::playMenuClick();
        name_1 = edit_name_1->getText();
        name_2 = edit_name_2->getText();
        gui.remove(edit_name_1);
        gui.remove(edit_name_2);
        settings_menu_window.close();
        item = START_GAME;
    });
}

void Menu::init_edit_names() {
    edit_name_1 = tgui::EditBox::create();
    edit_name_1->setPosition(180, 65);
    edit_name_1->setDefaultText(name_1);
    edit_name_1->setSize(200, 40);
    edit_name_1->setTextSize(30);
    edit_name_1->setMaximumCharacters(10);
    edit_name_1->setInputValidator("^[a-zA-Z0-9]+$");
    edit_name_1->getRenderer()->setTextColor(Color::Black);
    edit_name_1->getRenderer()->setDefaultTextColor(Color(255, 255, 255, 128));
    edit_name_1->getRenderer()->setDefaultTextStyle(Text::Regular);
    edit_name_1->getRenderer()->setBackgroundColor(Color::Transparent);
    edit_name_1->getRenderer()->setBackgroundColorHover(Color::Transparent);
    edit_name_1->getRenderer()->setBorderColor(Color::Transparent);
    edit_name_1->getRenderer()->setBorderColorHover(Color::Transparent);

    edit_name_2 = tgui::EditBox::copy(edit_name_1);
    edit_name_2->setPosition(180, 145);
    edit_name_2->setDefaultText(name_2);
    edit_name_2->setRenderer(edit_name_1->getRenderer()->clone());
}

void Menu::init_labels() {
    label_1 = tgui::Label::create("PLAYER 1");
    label_1->getRenderer()->setTextColor(Color::Black);
    label_1->setTextSize(30);
    label_1->setPosition(30, 70);

    label_2 = tgui::Label::create("PLAYER 2");
    label_2->getRenderer()->setTextColor(Color::Black);
    label_2->setTextSize(30);
    label_2->setPosition(30, 150);
}

void Menu::init_avatars() {
    spinAvatar1->setSize(160, 30);
    spinAvatar1->setPosition(350, 75);
    spinAvatar1->setVerticalScroll(false);
    spinAvatar1->getRenderer()->setSpaceBetweenArrows(100);
    spinAvatar1->getRenderer()->setBackgroundColor(Color::Transparent);
    spinAvatar1->getRenderer()->setBorderColor(Color::Transparent);
    spinAvatar1->getRenderer()->setBackgroundColorHover(Color(0, 0, 0, 128));
    spinAvatar1->connect("valuechanged",
                         [&]() {
                             SoundManager::playMenuClick();
                             avatar1.setTexture(avatars[spinAvatar1->getValue()]);
                         });

    spinAvatar2 = tgui::SpinButton::copy(spinAvatar1);
    spinAvatar2->setPosition(350, 155);
    spinAvatar2->connect("valuechanged",
                         [&]() {
                             SoundManager::playMenuClick();
                             avatar2.setTexture(avatars[spinAvatar2->getValue()]);
                         });


    avatar1.setTexture(avatars[spinAvatar1->getValue()]);
    avatar2.setTexture(avatars[spinAvatar2->getValue()]);

    avatar1.setOrigin(avatar1.getSize().x / 2, avatar1.getSize().y / 2);
    avatar1.setPosition(430, 90);
    avatar1.setScale(0.5, 0.5);

    avatar2.setOrigin(avatar2.getSize().x / 2, avatar2.getSize().y / 2);
    avatar2.setPosition(430, 170);
    avatar2.setScale(0.5, 0.5);
}