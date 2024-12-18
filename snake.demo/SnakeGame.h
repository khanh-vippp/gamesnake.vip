#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <SFML/Graphics.hpp>
#include "Menu.h"      // Bao gồm lớp Menu
#include "GamePlay.h"  // Bao gồm lớp GamePlay

class SnakeGame {
public:
    SnakeGame();         // Constructor: Khởi tạo cửa sổ và các thành phần của trò chơi
    void run();          // Phương thức chạy trò chơi, xử lý menu và bắt đầu game
    void startGame();    // Phương thức bắt đầu trò chơi rắn săn mồi

private:
    sf::RenderWindow window;  // Cửa sổ trò chơi
};

#endif // SNAKEGAME_H
