#pragma once
#include <SFML/Graphics.hpp>
#include <stdexcept>

class Game
{
public:
    Game();
    void run();
private:
    void processEvents();
    void update(sf::Time dt);
    void render();
    bool contains();
    sf::RenderWindow mWindow;
    sf::Texture mTexture;
    sf::Sprite mSprite;

    bool mIsMovingUp    = false;
    bool mIsMovingDown  = false;
    bool mIsMovingLeft  = false;
    bool mIsMovingRight = false;
    
    bool mIsButtonPressedLeft = false;
    bool mIsMouseMoved = false;
    float mMouseVelocityx = 0.0f;
    float mMouseVelocityy = 0.0f;
    float mMousePosx;
    float mMousePosy;
    sf::Vector2f mOldPos;

    float mVelocity = 30.0f;
    sf::Time mDtFixed;
};