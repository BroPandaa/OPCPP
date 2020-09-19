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
    
    auto it = mAsteroids.begin();
    for(int i =0; i<6; i++){
        (*it)->setPosition(wdth(generator), hght(generator));
        if(i==0 || i==1) (*it)->setValue(10);
        if(i==2 || i==3) (*it)->setValue(20);
        if(i==4 || i==5) (*it)->setValue(40);
        it++;
    }
    
    if(!mFont.loadFromFile("neuropol_x_rg.ttf"))
        throw("Error loading font");

    mLivesText.setFont(mFont);
    mLivesText.setPosition(20.0f, 20.0f);
    mLivesText.setCharacterSize(30);
    mLivesText.setStyle(sf::Text::Regular);

    mScoreText.setFont(mFont);
    mScoreText.setPosition(950.0f, 10.0f);
    mScoreText.setCharacterSize(30);
    mScoreText.setStyle(sf::Text::Regular);

    mEndMessage.setFont(mFont);
    mEndMessage.setString("Game over! Press SPACE to continue");
    mEndMessage.setCharacterSize(30);
    mEndMessage.setStyle(sf::Text::Italic);
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
        if(mGameOver && mIsShooting) restart();
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
    if(!mLives || mAsteroids.empty()) mGameOver=true;
    if(!mGameOver){
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
    listUpdate();

    for(auto it=mAsteroids.begin(); it!=mAsteroids.end(); it++){
        if(((*it)->radius()+mPlayer->radius())>distance((*it)->getCenter(), mPlayer->getCenter())){
            mExplosions.push_back(std::unique_ptr<Animation>(new Animation(mTextureHolder.getTexture(Textures::explosion), 4, 4, 256, 256, 0.4f)));
            auto it2 = mExplosions.end();
            it2--;
            (*it2)->setPosition(mPlayer->getCenter()+sf::Vector2f(1280/10.0f, 1024/10.0f));
            (*it2)->isAlive = true;

            mLives--;
            if(mLives){
                mPlayer->setPosition(1280/2, 1024/2);
                mPlayer->rotation();
                mCounter=0;
                }
            else
            {
                mPlayer->setPosition(-30.0f, -30.0f);
            }
            

            /*if((*it)->getValue()==10){
                mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+10)))));
                mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+10)))));
                auto itn = mAsteroids.end();
                itn--;
                auto vel = (*itn)->velocity();
                (*itn)->setValue(40);
                vel.x*=-1;
                vel.y*=-1;
                (*itn)->setPosition((*it)->getPosition() + sf::Vector2f(10.0f, 10.0f));
                itn--;
                (*itn)->setValue(40);
                (*itn)->setVelocity(vel);
                (*itn)->setPosition((*it)->getPosition() - sf::Vector2f(10.0f, 10.0f));
            }*/

            mAsteroids.erase(it);
            break;
        }
    }

    bool breakloop;
    for(auto itA=mAsteroids.begin(); itA!=mAsteroids.end(); itA++){
        breakloop = false;
        for(auto itB=mBullets.begin(); itB!=mBullets.end(); itB++){
            if(((*itA)->radius()+(*itB)->radius())>distance((*itA)->getPosition(), (*itB)->getPosition()+mPlayer->getPosition())){
                breakloop = true;
                mScore+=(*itA)->getValue();

                mExplosions.push_back(std::unique_ptr<Animation>(new Animation(mTextureHolder.getTexture(Textures::explosion), 4, 4, 256, 256, 0.4f)));
                auto it = mExplosions.end();
                it--;
                (*it)->setPosition((*itA)->getCenter()+sf::Vector2f(1280/10.0f, 1024/10.0f));
                (*it)->isAlive = true;

                if((*itA)->getValue()==10){
                mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+10)))));
                mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+10)))));
                auto itn = mAsteroids.end();
                itn--;
                auto vel = (*itn)->velocity();
                (*itn)->setValue(40);
                vel.x*=-1;
                vel.y*=-1;
                (*itn)->setPosition((*itA)->getPosition() + sf::Vector2f(10.0f, 10.0f));
                itn--;
                (*itn)->setVelocity(vel);
                (*itn)->setValue(40);
                (*itn)->setPosition((*itA)->getPosition() - sf::Vector2f(10.0f, 10.0f));
            }

                mBullets.erase(itB);
                mAsteroids.erase(itA);
                break;
            }
        }
        if(breakloop) break;
    }

    for(const auto& explosion: mExplosions) explosion->update();
    
    mScoreText.setString("Score: " + std::to_string(mScore));
    mLivesText.setString("Lives: " + std::to_string(mLives));
    }

    else if(mGameOver && !mTotalEnd){
        mLivesText=mEndMessage;
        for(const auto& explosion: mExplosions) if(explosion->isAlive) explosion->update();
        for(const auto& asteroid: mAsteroids) asteroid->update(dt);
        mBullets.erase(mBullets.begin(), mBullets.end());
        auto itExp = mExplosions.end();
        itExp--;
        if(!(*itExp)->isAlive) mTotalEnd = true;
        mPlayer->setPosition(-30.f, -30.0f);
        }
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
        if(mLives!=3 || mScore!=0) for(auto& explosion: mExplosions) if(explosion->isAlive) mWindow.draw(*explosion);
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

void Game::restart(){
    mLives = 3;
    mScore = 0;
    mCounter = 0;
    mGameOver = false;
    mIsShooting = false;
    mTotalEnd = false;

    mExplosions.erase(mExplosions.begin(), mExplosions.end());
    mAsteroids.erase(mAsteroids.begin(), mAsteroids.end());

    srand(time(nullptr));
    std::random_device dev;
    std::default_random_engine generator(dev());
    std::uniform_int_distribution<int> wdth(0, 1280);
    std::uniform_int_distribution<int> hght(0, 1024);

    mPlayer->setPosition(1280/2, 1024/2);
    mPlayer->rotation();

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%4+4)))));

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%4+4)))));

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+8)))));

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+8)))));

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+10)))));

    mAsteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(static_cast<Textures::ID>(rand()%2+10)))));
    
    auto it = mAsteroids.begin();
    for(int i =0; i<6; i++){
        (*it)->setPosition(wdth(generator), hght(generator));
        if(i==0 || i==1) (*it)->setValue(10);
        if(i==2 || i==3) (*it)->setValue(20);
        if(i==4 || i==5) (*it)->setValue(40);
        it++;
    }

    mLivesText.setPosition(20.0f, 20.0f);
    mLivesText.setCharacterSize(30);
    mLivesText.setStyle(sf::Text::Regular);
}