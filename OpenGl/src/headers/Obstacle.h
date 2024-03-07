#pragma once
#include <vector>
#include "GameObject.h"
#include "Block.h"

class Obstacle : public GameObject {
public:
    Obstacle() = default;
    Obstacle(glm::vec2 position);
    
    int blockSize = 10;
    std::vector<Block> blocks;
    std::vector<std::vector<int>> grid = {
        { 0, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 }
    };
    void Draw(SpriteRenderer& renderer) override;
};