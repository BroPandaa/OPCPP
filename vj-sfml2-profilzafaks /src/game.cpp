#include "game.h"
#include <stdexcept>
#include <ctime>
#include <cstdlib>
#include <cmath>
// Kod dopuniti/promijeniti po potrebi. 

Game::Game() : mWindow(sf::VideoMode(1280, 1024), "Asteroids") {
    mBackgroundSprite.setTexture(mTextureHolder.getTexture(Textures::background));
    
    mDtFixed = sf::seconds(1.0f/60.0f);

    srand(time(nullptr));
    std::random_device dev;
    std::default_random_engine generator(dev());
    std::uniform_int_distribution<int> wdth(0, 1280);
    std::uniform_int_distribution<int> hght(0, 1024);

    mPlayer.reset(new Player(mTextureHolder.getTexture(Textures::player)));
    mPlayer->setPosition(1280/2, 1024/2);

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%4+4)))));

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%4+4)))));

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+8)))));

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+8)))));

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+10)))));

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+10)))));
    
    mExplosion.reset(
        new Animation(mTextureHolder.getTexture(Textures::explosion), 4, 4, 256, 256, 0.2f)
                    );
    auto it = mAsteroids.begin();
    for(int i =0; i<6; i++){
        (*it)->setPosition(wdth(generator), hght(generator));
        it++;
    }
    
    if(!mFont.loadFromFile("font.ttf"))
        throw("Error loading font");

    mLivesText.setFont(mFont);
    mLivesText.setPosition(20.0f, 20.0f);
    mLivesText.setCharacterSize(30);
    mLivesText.setStyle(sf::Text::Regular);

    mScoreText.setFont(mFont);
    mScoreText.setPosition(950.0f, 10.0f);
    mScoreText.setCharacterSize(30);
    mScoreText.setStyle(sf::Text::Regular);
}

void Game::run(){
    // Start the game loop
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate >= mDtFixed){
            timeSinceLastUpdate -= mDtFixed;
            processEvents();
            update(mDtFixed);
        }       
        render();
    }

}

void Game::processEvents(){
        // Process events
        sf::Event event;
        while (mWindow.pollEvent(event))
        {
            // Close window : exit
            switch(event.type){
                case sf::Event::Closed: 
                    mWindow.close();
                    break;
                case sf::Event::KeyPressed:
                    handlePlayerInput(event.key.code, true);
                    break;
                case sf::Event::KeyReleased:
                    handlePlayerInput(event.key.code, false);
                    break;
                case sf::Event::MouseButtonPressed:
                    mMouseButtonPressed = true;
                    break;
                case sf::Event::MouseButtonReleased:
                    mMouseButtonPressed = false;
                    break;
            }
            
        }
}

void Game::update(sf::Time dt){
    sf::Vector2f velocity(0.f,0.f);
    float rotate=0;
    if(mIsMovingLeft)
       rotate -= mRotate;
    if(mIsMovingRight)
        rotate += mRotate;
    if(mIsShooting){
        mBullets.push_back(std::unique_ptr<Bullet>(new Bullet(mTextureHolder.getTexture(Textures::bullet))));
        auto it = mBullets.end();
        it--;
        (*it)->spawn(mPlayer->shootPosition(), mPlayer->shootRotation());
        mIsShooting = false;
    }
    for(const auto& asteroid: mAsteroids) asteroid->update(dt);

    if(mCounter || rotate) mPlayer->update(mCounter, dt, rotate);

    for(const auto& bullet: mBullets) bullet->update(dt);

    for(auto it=mAsteroids.begin(); it!=mAsteroids.end(); it++){
        if(((*it)->radius()+mPlayer->radius())>distance((*it)->getCenter(), mPlayer->getCenter())){
            mPlayer->setPosition(1280/2, 1024/2);
            mLives--;
        }
    }

    mExplosion->update();

    mScoreText.setString("Score: " + std::to_string(mScore));
    mLivesText.setString("Lives: " + std::to_string(mLives));
}

void Game::render(){
       // Clear screen
        mWindow.clear();
        mWindow.draw(mBackgroundSprite);
        // Draw the sprite
        mWindow.draw(*mPlayer);
        for(const auto& bullet: mBullets) mWindow.draw(*bullet);
        for(const auto& asteroid: mAsteroids) mWindow.draw(*asteroid);
        mWindow.draw(mScoreText);
        mWindow.draw(mLivesText);
        //mWindow.draw(*mExplosion);
        // Update the window
        mWindow.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key code, bool isPressed){
    if(code == sf::Keyboard::Key::Up){
        mCounter++;
        if(mCounter>3) mCounter=3;
    }
    if(code == sf::Keyboard::Key::Down){
        mCounter--;
        if(mCounter<0) mCounter=0;
    }
    if(code == sf::Keyboard::Key::Left)
        mIsMovingLeft = isPressed;
    if(code == sf::Keyboard::Key::Right)
        mIsMovingRight = isPressed;
    if(code == sf::Keyboard::Key::Space)
        mIsSpace = true;
    if(code == sf::Keyboard::Key::Space && !isPressed &&  mIsSpace){
        mIsSpace = false;
        mIsShooting = true;
    }            
}

void Game::listUpdate(){
    for(auto it=mBullets.begin(); it!=mBullets.end(); it++) if(!(*it)->isInside()) mBullets.erase(it);
}

float Game::distance(sf::Vector2f a, sf::Vector2f b){
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}