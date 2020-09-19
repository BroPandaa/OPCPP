#include "gamestate.h"
#include <stdexcept>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include "game.h"

GameState::GameState(Game * pGame): mpGame(pGame){}

WelcomeState::WelcomeState(Game * pGame): GameState(pGame){
    if(!mFont.loadFromFile("neuropol_x_rg.ttf"))
        throw("Error loading font");
    
    mMessageTitle.setFont(mFont);
    mMessage.setFont(mFont);

    mMessageTitle.setCharacterSize(40);
    mMessageTitle.setString("Asteroids clone by Filip Oresic");
    auto lb = mMessageTitle.getLocalBounds();
    mMessageTitle.setOrigin(lb.width/2, lb.height/2);
    mMessageTitle.setPosition(1280/2, 1024/2);

    mMessage.setCharacterSize(26);
    mMessage.setString("Press any key to continue");
    auto lb1 = mMessage.getLocalBounds();
    mMessage.setOrigin(lb1.width/2, lb1.height/2);
    auto pos = mMessageTitle.getPosition();
    pos.y += 50.0f;
    mMessage.setPosition(pos);
}

void WelcomeState::handleEvents(sf::Event event){
    if(event.type == sf::Event::KeyPressed || event.type==sf::Event::MouseButtonPressed) mpGame->setState(State::Playing);
}

void WelcomeState::render(){
    mpGame->getWindow()->draw(mMessageTitle);
    mpGame->getWindow()->draw(mMessage);
}

ScoreState::ScoreState(Game * pGame): GameState(pGame){
    if(!mFont.loadFromFile("neuropol_x_rg.ttf"))
        throw("Error loading font");

    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(40);
    mScoreText.setString("Score: ");
    auto lb = mScoreText.getLocalBounds();
    mScoreText.setOrigin(lb.width/2, lb.height/2);
    mScoreText.setPosition(1280/2, 1024/2);

    mEndMessage.setFont(mFont);
    mEndMessage.setCharacterSize(28);
    mEndMessage.setString("Enter to continue, ESC to finish");
    auto lb1 = mEndMessage.getLocalBounds();
    mEndMessage.setOrigin(lb1.width/2, lb1.height/2);
    auto pos = mScoreText.getPosition();
    pos.y += 50.0f;
    mEndMessage.setPosition(pos);
}

void ScoreState::init(){
    mScore = mpGame->mScore;
    mScoreText.setString("Score: "+ std::to_string(mScore));
    auto lb = mScoreText.getLocalBounds();
    mScoreText.setOrigin(lb.width/2, lb.height/2);
    mScoreText.setPosition(1280/2, 1024/2);
}

void ScoreState::handleEvents(sf::Event event){
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape) mpGame->getWindow()->close();
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Return) mpGame->setState(Playing);
}

void ScoreState::render(){
    mpGame->getWindow()->draw(mScoreText);
    mpGame->getWindow()->draw(mEndMessage);
}