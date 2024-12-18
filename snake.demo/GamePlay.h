#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class GamePlay {
public:
    GamePlay(sf::RenderWindow& window);
    void startGame();

private:
    struct Segment {
        sf::Vector2f position;  // Vị trí của đoạn thân
        sf::Sprite sprite;      // Sprite của đoạn thân
    };

    sf::RenderWindow& window;
 
    // Textures và Sprites
    sf::Texture snakeTexture, foodTexture, backgroundTexture;
    sf::Sprite backgroundSprite;

    // Font và Text
    sf::Font font;
    sf::Text scoreText;

    // Âm thanh
    sf::SoundBuffer eatSoundBuffer, gameOverSoundBuffer;
    sf::Sound eatSound, gameOverSound;

    // Rắn và thức ăn
    std::vector<Segment> snake;
    std::vector<sf::Sprite> foods;  // Thức ăn dùng Sprite
    sf::Vector2f direction;

 

    // Biến trạng thái
    int score;
    bool floodFood;   // Kiểm tra có cần tạo thức ăn mới hay không
    bool isPaused;    // Tạm dừng trò chơi
    int eatenFoods = 0;  // Số mồi đã ăn
    int foodCount = 5;   // Số mồi ban đầu
    sf::Clock clock;
    // Hàm tiện ích
    void displayMessage(const std::string& message);
    void createMultipleFoods();
    void moveSnake();
    void processEvents();
    bool checkCollision();
    void update();
    void render();
};

#endif
