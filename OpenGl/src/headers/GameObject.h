#pragma once
#include <glm/glm.hpp>
#include <string>
#include "texture.h"
#include "SpriteRenderer.h"
#include "BulletSystem.h"

class Texture2D;
// Container object for holding all state relevant for a single game object entity.
// Each object in the game likely needs the minimal of state as described within GameObject.
class GameObject
{
public:
    int UniqueId;
    std::string Name;
    glm::vec2 Position, Size, Velocity;
    glm::vec3 Color;
    float Rotation;
    bool IsSolid;
    bool Destroyed = false;

    std::string SpritePath;    
    Texture2D Sprite;

    GameObject();
    GameObject(unsigned int uniqueId, std::string name, glm::vec2 pos, glm::vec2 size, std::string SpritePath, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

    // Draws this GameObject Sprite
    virtual void Draw(SpriteRenderer& renderer);

    // Set the values for this GameObject Instance
    void SetValues(const std::string& tag, const std::string& value);
};