#include "GamePlay.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

GamePlay::GamePlay(sf::RenderWindow& window)
    : window(window), score(0), direction(20.f, 0.f), floodFood(false), isPaused(false) {
    // Tải các texture
    if (!snakeTexture.loadFromFile("assets/sn.png") ||
        !foodTexture.loadFromFile("assets/food.png") ||
        !backgroundTexture.loadFromFile("assets/background.png")) {
        std::cerr << "Error loading images!" << std::endl;
        exit(1);
    }

    // Tải âm thanh
    if (!eatSoundBuffer.loadFromFile("assets/eat.wav") ||
        !gameOverSoundBuffer.loadFromFile("assets/lose.wav")) {
        std::cerr << "Error loading sound files!" << std::endl;
        exit(1);
    }

    eatSound.setBuffer(eatSoundBuffer);
    gameOverSound.setBuffer(gameOverSoundBuffer);

    // Thiết lập background
    backgroundSprite.setTexture(backgroundTexture);
    auto windowSize = window.getSize();
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
        static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
    );

    // Khởi tạo đầu rắn
    Segment head;
    head.position = sf::Vector2f(200.f, 200.f);
    head.sprite.setTexture(snakeTexture);
    head.sprite.setScale(0.15f, 0.15f);  // Tùy chỉnh tỉ lệ hình ảnh
    head.sprite.setPosition(head.position);
    snake.push_back(head);

    // Tải font
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        exit(1);
    }

    // Hiển thị điểm số
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);

    // Random seed
    srand(static_cast<unsigned>(time(0)));
    createMultipleFoods();
}

void GamePlay::createMultipleFoods() {
    int numFoods = foodCount;  // Sử dụng biến foodCount để xác định số lượng mồi cần tạo

    for (int i = 0; i < numFoods; ++i) {
        sf::Sprite food;
        food.setTexture(foodTexture);
        food.setScale(0.05f, 0.05f);
        food.setPosition(
            static_cast<float>((rand() % 40) * 20),
            static_cast<float>((rand() % 30) * 20)
        );
        foods.push_back(food);
    }
}




void GamePlay::moveSnake() {
    for (size_t i = snake.size() - 1; i > 0; --i) {
        snake[i].position = snake[i - 1].position;
        snake[i].sprite.setPosition(snake[i].position);
    }

    snake[0].position += direction;
    snake[0].sprite.setPosition(snake[0].position);
}

void GamePlay::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                isPaused = !isPaused;  // Dừng/tiếp tục trò chơi
            }
            else if (event.key.code == sf::Keyboard::Up && direction.y == 0) {
                direction = sf::Vector2f(0, -20.f);
            }
            else if (event.key.code == sf::Keyboard::Down && direction.y == 0) {
                direction = sf::Vector2f(0, 20.f);
            }
            else if (event.key.code == sf::Keyboard::Left && direction.x == 0) {
                direction = sf::Vector2f(-20.f, 0);
            }
            else if (event.key.code == sf::Keyboard::Right && direction.x == 0) {
                direction = sf::Vector2f(20.f, 0);
            }
        }
    }
}

bool GamePlay::checkCollision() {
    // Kiểm tra va chạm với tường
    if (snake[0].position.x < 0 || snake[0].position.x >= window.getSize().x ||
        snake[0].position.y < 0 || snake[0].position.y >= window.getSize().y) {
        return true;
    }

    // Kiểm tra va chạm với chính mình
    for (size_t i = 1; i < snake.size(); ++i) { // Bỏ qua phần đầu rắn
        if (snake[0].position == snake[i].position) {
            return true;
        }
    }

    return false; // Không có va chạm
}


void GamePlay::update() {
    if (isPaused) return;  // Nếu game đang dừng thì không làm gì

    moveSnake();

    // Kiểm tra va chạm trước khi xử lý thêm thức ăn
    if (checkCollision()) {
        gameOverSound.play();
        displayMessage("Game Over! Final Score: " + std::to_string(score));
        window.close();
        return;
    }

    // Kiểm tra ăn thức ăn
    for (auto it = foods.begin(); it != foods.end();) {
        if (snake[0].sprite.getGlobalBounds().intersects(it->getGlobalBounds())) {
            eatSound.play();
            score++;
            eatenFoods++;  // Tăng số mồi đã ăn
            scoreText.setString("Score: " + std::to_string(score));

            // Thêm đoạn thân mới ở vị trí cuối cùng
            Segment newSegment;
            newSegment.position = snake.back().position;
            newSegment.sprite.setTexture(snakeTexture);
            newSegment.sprite.setScale(0.15f, 0.15f);
            newSegment.sprite.setPosition(newSegment.position);
            snake.push_back(newSegment);

            // Xóa thức ăn đã ăn
            it = foods.erase(it);

            // Tăng số lượng mồi sau mỗi 2 mồi đã ăn
            if (eatenFoods % 2 == 0) {
                foodCount++;  // Mỗi lần ăn 2 mồi, tăng số mồi cần tạo thêm
            }
        }
        else {
            ++it;
        }
    }

    // Nếu không còn thức ăn, tạo thức ăn mới
    if (foods.empty()) {
        createMultipleFoods();
    }
}



void GamePlay::displayMessage(const std::string& message) {
    sf::Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(50);
    messageText.setFillColor(sf::Color::Red);
    messageText.setString(message);

    messageText.setPosition(
        window.getSize().x / 2.f - messageText.getGlobalBounds().width / 2.f,
        window.getSize().y / 2.f - messageText.getGlobalBounds().height / 2.f
    );

    window.clear();
    window.draw(backgroundSprite);
    window.draw(messageText);
    window.display();

    sf::sleep(sf::seconds(3));  // Chờ 3 giây trước khi tiếp tục
}


void GamePlay::render() {
    window.clear();
    window.draw(backgroundSprite);

    for (const auto& segment : snake) {
        window.draw(segment.sprite);
    }
    for (const auto& food : foods) {
        window.draw(food);
    }
    window.draw(scoreText);
    window.display();
}

void GamePlay::startGame() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();

        if (clock.getElapsedTime().asMilliseconds() > 150) {
            update();
            clock.restart();
        }

        render();
    }
}
