#pragma once
#include "GameObject.h"

class Block : public GameObject {
public:
	Block(glm::vec2 pos, glm::vec2 size, std::string spritePath, Texture2D sprite, glm::vec3 color)
		: GameObject(pos, size, spritePath, sprite, color) {};
	void Damage();
	bool ToBeDestroyed() const;
private:
	unsigned int Life = 3;
};