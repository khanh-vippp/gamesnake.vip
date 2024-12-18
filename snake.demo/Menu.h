#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>  // Thêm thư viện âm thanh

class Menu {
public:
    Menu(sf::RenderWindow& window);
    void displayMenu();  // Hiển thị menu chính
    bool isPlaySelected();  // Kiểm tra xem nút Play có được chọn không
    bool isExitSelected();  // Kiểm tra xem nút Exit có được chọn không
    bool isSettingsSelected();  // Kiểm tra xem nút Settings có được chọn không
    bool isHelpSelected();  // Kiểm tra xem nút Help có được chọn không

    bool isSinglePlayerSelected();  // Kiểm tra xem người chơi đã chọn chế độ 1 Player
    bool isVsAISelected();  // Kiểm tra xem người chơi đã chọn chế độ Player vs AI

private:
    void drawMenu();  // Vẽ menu
    void handleHoverEffect();  // Xử lý hiệu ứng hover khi di chuột qua các nút
    void showHelpScreen();  // Hiển thị màn hình trợ giúp
    void showSettingsScreen();  // Hiển thị màn hình cài đặt
    void toggleSound();  // Bật/tắt âm thanh

    sf::RenderWindow& window;
    sf::Texture backgroundTexture, playButtonTexture, settingsButtonTexture, helpButtonTexture, exitButtonTexture;
    sf::Texture soundButtonTextureOn, soundButtonTextureOff;  // Thêm các texture cho nút âm thanh
    sf::Sprite backgroundSprite, playButtonSprite, settingsButtonSprite, helpButtonSprite, exitButtonSprite, soundButtonSprite;  // Nút âm thanh

    bool playSelected;
    bool exitSelected;
    bool settingsSelected;
    bool helpSelected;
    bool soundOn;  // Biến lưu trạng thái âm thanh (bật/tắt)

    sf::Music backgroundMusic;  // Âm nhạc nền
    sf::SoundBuffer clickBuffer;  // Bộ đệm âm thanh click
    sf::Sound clickSound;  // Âm thanh click
};

#endif
