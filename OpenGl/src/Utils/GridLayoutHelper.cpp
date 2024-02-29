#include "GridLayoutHelper.h"

glm::vec2 GridLayoutHelper::scalePosition(const glm::vec2& position) {
    float scaleX = static_cast<float>(SCREEN_WIDTH2) / 800;
    float scaleY = static_cast<float>(SCREEN_HEIGHT2) / 600;
    return { position.x * scaleX, position.y * scaleY };
}

void GridLayoutHelper::CreateGridLayoutForEnemies() {
    std::ofstream file("res/levels/level.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    float sizeX = 24.0f;
    float sizeY = 20.5f;
    float startX = 60.0f;
    float startY = 40.0f;
    float offsetX = 20.0f;
    float offsetY = 40.0f;
    float velocityX = 10.f;
    float velocityY = 0.0f;

    int columns = 11;

    std::unordered_map<int, std::string> EnemySpriteMap = {
        { 1, "green.png" },
        { 2, "yellow.png" },
        { 3, "yellow.png" },
        { 4, "red.png" },
        { 5, "red.png" },
        { 6, "extra.png" },
    };

    for (int i = 0; i < 55; ++i) {
        int spriteIndex = (i / 11) + 1;
        glm::vec2 scaledPosition = scalePosition(glm::vec2(startX, startY));

        file << "[GameObject]Enemy" << std::setfill('0') << std::setw(2) << (i + 1) << std::endl;
        file << "\t[Position]" << scaledPosition.x << "," << scaledPosition.y << "[/Position]" << std::endl;
        file << "\t[Size]" << sizeX << "," << sizeY << "[/ Size]" << std::endl;
        file << "\t[Velocity]" << velocityX << "," << velocityY << "[/Velocity]" << std::endl;
        file << "\t[Rotation]0.0[/Rotation]" << std::endl;
        file << "\t[IsSolid]false[/IsSolid]" << std::endl;
        file << "\t[Sprite]" << EnemySpriteMap[spriteIndex] << "[/Sprite]" << std::endl;
        //file << "\t[Sprite2]" << EnemySpriteMap[spriteIndex] << "[/Sprite2]" << std::endl;
        file << "[/GameObject]\n" << std::endl;

        startX += offsetX + 30.0f;
        if ((i + 1) % columns == 0) {
            startX = 60.0f;
            startY += offsetY;
        }
    }

    file.close();
}
