#include "game.h"
#include <stdexcept>
#include <ctime>
#include <cstdlib>
#include <cmath>
// Kod dopuniti/promijeniti po potrebi. 

Game::Game() : mWindow(sf::VideoMode(1280, 1024), "Asteroids") {
    mAllStates[0] = new WelcomeState(this);
    mAllStates[1] = new PlayState(this);
    mAllStates[2] = new ScoreState(this);

    mDtFixed = sf::seconds(1.0f/60.0f);
    mpCurrentState = mAllStates[0];
}

void Game::render(){
        mWindow.clear();
        mpCurrentState->render();
        mWindow.display();
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
            mpCurrentState->update(mDtFixed);
        }       
        render();
    }

}

void Game::processEvents(){
        // Process events
        sf::Event event;
        while (mWindow.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                mWindow.close();
            else
                mpCurrentState->handleEvents(event);
        }
}


void Game::setState(GameState::State newState){
    mpCurrentState = mAllStates[newState];
    if(newState == GameState::State::Playing || newState== GameState::State::Score) mpCurrentState->init();
}