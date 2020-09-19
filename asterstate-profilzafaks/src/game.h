#pragma once
#include <SFML/Graphics.hpp>
#include "gamestate.h"
#include "playstate.h"
// Klasu dopuniti po potrebi.

class Game{
public:
    friend class ScoreState;
    Game();
    void run();
    void setState(GameState::State newState);
    void processEvents();
    sf::RenderWindow* getWindow(){return &mWindow;}
    void setScore(int score){mScore = score;}
private:
    void render();
    void restart();// resetira igru
    
    int mScore = 0;
    sf::Time mDtFixed;
    GameState* mpCurrentState;

    sf::RenderWindow mWindow;
    std::array<GameState*, 3> mAllStates;
};