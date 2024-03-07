#include "Obstacle.h"
#include "ResourceManager.h"

Obstacle::Obstacle(glm::vec2 position)
{
    this->Position = position;

    for (unsigned int row = 0; row < this->grid.size(); ++row) {
        for (unsigned int column = 0; column < this->grid[0].size(); ++column) {
            if (this->grid[row][column] == 1) {
                float pos_x = position.x + column * blockSize;
                float pos_y = position.y + row * blockSize;

                Block block = Block(
                    glm::vec2{ pos_x, pos_y }, glm::vec2{ blockSize, blockSize },
                    std::string{ "" }, ResourceManager::GetTexture("square.png"), glm::vec3{1.0,1.0,1.0});
                blocks.push_back(block);
            }
        }
    }
}

void Obstacle::Draw(SpriteRenderer& renderer) {
    for (auto it = blocks.begin(); it != blocks.end();) {
        if (it->Destroyed) {
            it = blocks.erase(it);
        }
        else {
            it->Draw(renderer);
            ++it;
        }
    }
}