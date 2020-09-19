#pragma once
#include <SFML/Graphics.hpp>
#include "textureHolder.h"
#include "entities.h"
#include <random>
#include <functional>
#include <list>
// Klasu dopuniti po potrebi.

class Game{
public:
    Game();
    void run();
private:
    void processEvents();
    void update(sf::Time dt);
    void render();
    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);
    void listUpdate(); //uklanja metke koji su izašli izvan okvira
    float distance(sf::Vector2f a, sf::Vector2f b);

    sf::RenderWindow mWindow;
    TextureHolder mTextureHolder;  
    sf::Sprite mSprite;
    sf::Sprite mBackgroundSprite;
    std::unique_ptr<Asteroid> mAsteroid;
    std::unique_ptr<Animation> mExplosion;
    std::unique_ptr<Player> mPlayer;
    std::unique_ptr<Bullet> mBullet;
    std::list<std::unique_ptr<Bullet>> mBullets;
    std::list<std::unique_ptr<Asteroid>> mAsteroids;
    bool mIsMovingLeft  = false;
    bool mIsMovingRight = false;
    bool mIsShooting    = false;
    bool mIsSpace       = false;
    
    int mCounter = 0;
    int mLives   = 3;
    int mScore   = 0;
    
    sf::Text mScoreText;
    sf::Text mLivesText;
    sf::Font mFont;

    bool mMouseButtonPressed = false;

    float mVelocity = 120.0f; // pixel/sec
    float mRotate   = 200.0f;
    sf::Time mDtFixed;
};
