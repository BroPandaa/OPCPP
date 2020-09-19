#pragma once
#include "gamestate.h"
#include "textureHolder.h"
#include "entities.h"
#include <random>
#include <functional>
#include <list>

class PlayState : public GameState{
public:
    PlayState(Game * pGame);
    void update(sf::Time dt) override;
    void render() override;
    void handleEvents(sf::Event event) override;
    void init() override;
private:
    void processEvents();
    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);
    void listUpdate(); //uklanja metke koji su izašli izvan okvira
    float distance(sf::Vector2f a, sf::Vector2f b); //udaljenost dva sredista kruznica

    TextureHolder mTextureHolder;  
    sf::Sprite mBackgroundSprite;
    std::unique_ptr<Asteroid> mAsteroid;
    std::unique_ptr<Animation> mExplosion;
    std::unique_ptr<Player> mPlayer;
    std::unique_ptr<Bullet> mBullet;
    std::list<std::unique_ptr<Bullet>> mBullets;
    std::list<std::unique_ptr<Asteroid>> mAsteroids;
    std::list<std::unique_ptr<Animation>> mExplosions;

    bool mIsMovingLeft  = false;
    bool mIsMovingRight = false;
    bool mIsShooting    = false;
    bool mIsSpace       = false;
    bool mGameOver      = false;
    bool mTotalEnd      = false;
    bool mDoNotShoot    = true; // izbjegavanje pucanja u slucaju da je pritisnut SPACE tokom WelcomeState-a
    
    int mCounter       = 0;
    int mLives         = 3;
    int mScore         = 0;
    
    sf::Text mScoreText;
    sf::Text mLivesText;
    sf::Text mEndMessage;
    sf::Font mFont;

    bool mMouseButtonPressed = false;

    float mVelocity = 120.0f; // pixel/sec
    float mRotate   = 200.0f;
};
