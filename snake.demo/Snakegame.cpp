#include "SnakeGame.h"
#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

// Constructor SnakeGame, khởi tạo cửa sổ game
SnakeGame::SnakeGame() : window(sf::VideoMode(800, 600), "Snake Game") {
    // Thiết lập các tham số, có thể thêm như âm thanh, font chữ, v.v...
}

// Hàm run: Chạy toàn bộ trò chơi, xử lý menu và các sự kiện
void SnakeGame::run() {
    Menu menu(window);  // Tạo đối tượng menu với cửa sổ đã tạo
    menu.displayMenu();  // Hiển thị menu

    // Kiểm tra nếu người chơi chọn Play từ menu
    if (menu.isPlaySelected()) {
        startGame();  // Nếu chọn Play, bắt đầu game
    }

    // Nếu chọn Exit, đóng cửa sổ
    if (menu.isExitSelected()) {
        window.close();
    }
}

// Hàm startGame: Khởi động trò chơi chính
void SnakeGame::startGame() {
    // Đây là phần cài đặt trò chơi chính
    // Bạn có thể tạo class GamePlay hoặc logic riêng cho trò chơi rắn săn mồi
    GamePlay gameplay(window);
    gameplay.startGame();  // Bắt đầu trò chơi
}
