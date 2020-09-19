#include "game.h"

Game::Game() : mWindow(sf::VideoMode(400, 400), "SFML window")
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "SFML window");
    sf::Texture texture;
    if (!mTexture.loadFromFile("cb.bmp"))
        throw std::runtime_error("Cannot open file cb.bmp!");
    mSprite.setTexture(mTexture);
    mDtFixed = sf::seconds(1.0f/60.0f);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time  timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen()){
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate>= mDtFixed){
            timeSinceLastUpdate -= mDtFixed;
            processEvents();
            update(mDtFixed);
        }
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
        while (mWindow.pollEvent(event))
        {
            switch(event.type){
                case sf::Event::Closed:
                    mWindow.close();
                    break;
                case sf::Event::KeyPressed:
                    if(event.key.code== sf::Keyboard::Key::Up)
                        mIsMovingUp=true;
                    if(event.key.code== sf::Keyboard::Key::Down)
                        mIsMovingDown=true;
                    if(event.key.code== sf::Keyboard::Key::Left)
                        mIsMovingLeft=true;
                    if(event.key.code== sf::Keyboard::Key::Right)
                        mIsMovingRight=true;
                    break;
                case sf::Event::KeyReleased:
                    if(event.key.code== sf::Keyboard::Key::Up)
                        mIsMovingUp=false;
                    if(event.key.code== sf::Keyboard::Key::Down)
                        mIsMovingDown=false;
                    if(event.key.code== sf::Keyboard::Key::Left)
                        mIsMovingLeft=false;
                    if(event.key.code== sf::Keyboard::Key::Right)
                        mIsMovingRight=false;
                    break;
                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Button::Left){
                        mIsButtonPressedLeft = true;
                        mOldPos.x=sf::Mouse::getPosition(mWindow).x;
                        mOldPos.y=sf::Mouse::getPosition(mWindow).y;
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if(event.mouseButton.button == sf::Mouse::Button::Left)
                        mIsButtonPressedLeft = false;
                        mIsMouseMoved = false;
                    break;
                case sf::Event::MouseMoved:
                if(mIsButtonPressedLeft && mSprite.getGlobalBounds().contains(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y)){
                    mIsMouseMoved = true;
                    mMouseVelocityx = sf::Mouse::getPosition(mWindow).x - mOldPos.x;
                    mMouseVelocityy = sf::Mouse::getPosition(mWindow).y - mOldPos.y;
                    break;
                }
            }
            
        }
}

void Game::update(sf::Time dt)
{
    sf::Vector2f velocity(0.f, 0.f);
    if(mIsMovingUp)
        velocity.y -=mVelocity;
    if(mIsMovingDown)
        velocity.y +=mVelocity;
    if(mIsMovingLeft)
        velocity.x -=mVelocity;
    if(mIsMovingRight)
        velocity.x +=mVelocity;

    mSprite.move(velocity*dt.asSeconds());
    
    if(mIsMouseMoved)
        mSprite.move(mMouseVelocityx*dt.asSeconds(), mMouseVelocityy*dt.asSeconds());
}
  
void Game::render()
{
    mWindow.clear();
    mWindow.draw(mSprite);
    mWindow.display();
}
