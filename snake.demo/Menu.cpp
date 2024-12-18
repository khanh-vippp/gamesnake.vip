#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

Menu::Menu(sf::RenderWindow& window)
    : window(window), playSelected(false), exitSelected(false), settingsSelected(false), helpSelected(false), soundOn(true) {
    // Tải hình ảnh cho background và các nút
    if (!backgroundTexture.loadFromFile("assets/menu.png") ||
        !playButtonTexture.loadFromFile("assets/button_play.png") ||
        !exitButtonTexture.loadFromFile("assets/exit.png") ||
        !settingsButtonTexture.loadFromFile("assets/set.png") ||
        !helpButtonTexture.loadFromFile("assets/help.png") ||
        !soundButtonTextureOn.loadFromFile("assets/on.png") ||
        !soundButtonTextureOff.loadFromFile("assets/off.png")) {
        throw std::runtime_error("Error loading images!");
    }

    // Thiết lập background
    backgroundSprite.setTexture(backgroundTexture);

    // Thiết lập nút Play
    float centerX = window.getSize().x / 2.0f;
    playButtonSprite.setTexture(playButtonTexture);
    playButtonSprite.setPosition(centerX - playButtonSprite.getGlobalBounds().width / 2,
        window.getSize().y / 3 + 100);

    // Thiết lập các nút khác (Help, Settings, Exit)
    helpButtonSprite.setTexture(helpButtonTexture);
    helpButtonSprite.setPosition(playButtonSprite.getPosition().x + playButtonSprite.getGlobalBounds().width + 20,
        playButtonSprite.getPosition().y - 50);

    settingsButtonSprite.setTexture(settingsButtonTexture);
    settingsButtonSprite.setPosition(20, window.getSize().y - settingsButtonSprite.getGlobalBounds().height - 20);

    exitButtonSprite.setTexture(exitButtonTexture);
    exitButtonSprite.setPosition(centerX - exitButtonSprite.getGlobalBounds().width / 2 + 8,
        playButtonSprite.getPosition().y + 90);

    // Thiết lập nút âm thanh (ở góc trên bên trái)
    soundButtonSprite.setTexture(soundButtonTextureOn);  // Mặc định là bật âm thanh
    soundButtonSprite.setPosition(10, 10);

    // Tải nhạc nền
    if (!backgroundMusic.openFromFile("assets/music.mp3")) {
        throw std::runtime_error("Error loading background music!");
    }
    backgroundMusic.setLoop(true);
    if (soundOn) {
        backgroundMusic.play();  // Bắt đầu phát nhạc nền nếu âm thanh bật
    }

    // Tải âm thanh click
    if (!clickBuffer.loadFromFile("assets/click_sound.wav")) {
        throw std::runtime_error("Error loading click sound!");
    }
    clickSound.setBuffer(clickBuffer);
}

void Menu::drawMenu() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(playButtonSprite);
    window.draw(settingsButtonSprite);
    window.draw(helpButtonSprite);
    window.draw(exitButtonSprite);
    window.draw(soundButtonSprite);  // Vẽ nút âm thanh
    window.display();
}

void Menu::displayMenu() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Kiểm tra các nút được nhấn
                if (playButtonSprite.getGlobalBounds().contains(mousePos)) {
                    playSelected = true;
                    clickSound.play();  // Phát âm thanh click
                    return;  // Ngừng vòng lặp và chuyển đến nơi gọi displayMenu, bắt đầu game ở đó
                }
                if (settingsButtonSprite.getGlobalBounds().contains(mousePos)) {
                    settingsSelected = true;
                    showSettingsScreen(); // Hiển thị màn hình cài đặt
                    settingsSelected = false;
                }
                if (helpButtonSprite.getGlobalBounds().contains(mousePos)) {
                    helpSelected = true;
                    showHelpScreen(); // Hiển thị màn hình trợ giúp
                    helpSelected = false;
                }
                if (exitButtonSprite.getGlobalBounds().contains(mousePos)) {
                    exitSelected = true;
                    clickSound.play();  // Phát âm thanh click
                    return; // Đóng game khi chọn Exit
                }
                // Kiểm tra nếu nút âm thanh được nhấn
                if (soundButtonSprite.getGlobalBounds().contains(mousePos)) {
                    toggleSound();  // Thực hiện bật/tắt âm thanh
                    clickSound.play();  // Phát âm thanh click
                }
            }
        }

        // Hiệu ứng hover
        handleHoverEffect();

        drawMenu();
    }
}

void Menu::handleHoverEffect() {
    auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Kiểm tra hiệu ứng hover cho các nút
    if (playButtonSprite.getGlobalBounds().contains(mousePos)) {
        playButtonSprite.setColor(sf::Color(200, 200, 200));
    }
    else {
        playButtonSprite.setColor(sf::Color::White);
    }

    if (settingsButtonSprite.getGlobalBounds().contains(mousePos)) {
        settingsButtonSprite.setColor(sf::Color(200, 200, 200));
    }
    else {
        settingsButtonSprite.setColor(sf::Color::White);
    }

    if (helpButtonSprite.getGlobalBounds().contains(mousePos)) {
        helpButtonSprite.setColor(sf::Color(200, 200, 200));
    }
    else {
        helpButtonSprite.setColor(sf::Color::White);
    }

    if (exitButtonSprite.getGlobalBounds().contains(mousePos)) {
        exitButtonSprite.setColor(sf::Color(200, 200, 200));
    }
    else {
        exitButtonSprite.setColor(sf::Color::White);
    }

    // Hiệu ứng hover cho nút âm thanh
    if (soundButtonSprite.getGlobalBounds().contains(mousePos)) {
        soundButtonSprite.setColor(sf::Color(200, 200, 200));
    }
    else {
        soundButtonSprite.setColor(sf::Color::White);
    }
}

void Menu::toggleSound() {
    if (soundOn) {
        backgroundMusic.stop();  // Dừng nhạc nền
        soundButtonSprite.setTexture(soundButtonTextureOff);  // Thay đổi hình ảnh nút thành tắt âm thanh
    }
    else {
        backgroundMusic.play();  // Bắt đầu lại nhạc nền
        soundButtonSprite.setTexture(soundButtonTextureOn);  // Thay đổi hình ảnh nút thành bật âm thanh
    }
    soundOn = !soundOn;  // Đảo ngược trạng thái âm thanh
}

void Menu::showHelpScreen() {
    sf::Texture helpTexture;
    if (!helpTexture.loadFromFile("assets/help1.jpg")) {
        throw std::runtime_error("Error loading help image!");
    }

    sf::Sprite helpSprite;
    helpSprite.setTexture(helpTexture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                return; // Thoát màn hình trợ giúp khi nhấn phím ESC
            }
        }

        window.clear();
        window.draw(helpSprite);
        window.display();
    }
}

void Menu::showSettingsScreen() {
    sf::Texture settingsTexture;
    if (!settingsTexture.loadFromFile("assets/cre.png")) {
        throw std::runtime_error("Error loading settings image!");
    }

    sf::Sprite settingsSprite;
    settingsSprite.setTexture(settingsTexture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                return; // Thoát màn hình cài đặt khi nhấn phím ESC
            }
        }

        window.clear();
        window.draw(settingsSprite);
        window.display();
    }
}

bool Menu::isPlaySelected() {
    return playSelected;
}

bool Menu::isExitSelected() {
    return exitSelected;
}

bool Menu::isSettingsSelected() {
    return settingsSelected;
}

bool Menu::isHelpSelected() {
    return helpSelected;
}
