#include "Player.h"
#include <iostream>

Player::Player(ISoundEngine* soundEngine, std::shared_ptr<BulletSystem> bulletSystem, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity)
    : SoundEngine(soundEngine), bulletSystem(bulletSystem), GameObject(pos, size, sprite, color, velocity) { };

void Player::Shoot() {
    if (canShoot) {
        glm::vec2 bulletP = { this->Position.x + this->Size.x / 2 , this->Position.y };

        currentBullet = bulletSystem->FireBullet(currentBulletType, 
            bulletP, bulletVelocity, true);
        canShoot = false;
        SoundEngine->play2D("res/audio/shoot.wav", false);
    }
    if (currentBullet->Destroyed) {
        canShoot = true;
    }
}

void Player::Reset(glm::vec2 playerStartPos) {
    if (!playerHit) {
        this->Position = playerStartPos;
        SoundEngine->play2D("res/audio/explosion.wav", false);
        playerHit = true;
    }
}