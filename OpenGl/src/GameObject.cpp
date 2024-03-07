#include "GameObject.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

GameObject::GameObject()
    : UniqueId(-1), Name(""),
      Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), 
      SpritePath(""), Sprite(), 
      IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, std::string spritePath, Texture2D sprite, glm::vec3 color)
    : UniqueId(-1), Name("name"),
    Position(pos), Size(size), Velocity(0.0f), Color(color), Rotation(0.0f),
    SpritePath(spritePath), Sprite(sprite),
    IsSolid(false), Destroyed(false) { }

GameObject::GameObject(unsigned int uniqueId, std::string name, glm::vec2 pos, glm::vec2 size, std::string spritePath, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : UniqueId(uniqueId), Name(name), 
      Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), 
      SpritePath(spritePath), Sprite(sprite), 
      IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : UniqueId(-1), Name(""),
    Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f),
    SpritePath(""), Sprite(sprite),
    IsSolid(false), Destroyed(false) { }

void GameObject::Draw(SpriteRenderer& renderer){
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

void GameObject::SetValues(const std::string& tag, const std::string& value) {

    if (tag == "GameObject") {
        this->Name = value;
    }
    else if (tag == "Position") {
        std::stringstream ss(value);
        char comma;
        ss >> this->Position.x >> std::noskipws >> comma >> this->Position.y;
    }
    else if (tag == "Size") {
        std::stringstream ss(value);
        char comma;
        ss >> this->Size.x >> std::noskipws >> comma >> this->Size.y;
    }
    else if (tag == "Velocity") {
        std::stringstream ss(value);
        char comma;
        ss >> this->Velocity.x >> std::noskipws >> comma >> this->Velocity.y;
    }
    else if (tag == "Rotation") {
        this->Rotation = std::stof(value);
    }
    else if (tag == "Sprite") {
        this->SpritePath = value;
    }
}