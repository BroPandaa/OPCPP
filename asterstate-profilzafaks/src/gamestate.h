#pragma once
#include <SFML/Graphics.hpp>
class Game;

class GameState
{
public:
    GameState();
    GameState(Game *pGame);
    friend class Game;
    virtual ~GameState() {}
    // Sva moguća stanja igre.
        enum State{
            Welcome,
            Playing,
            Score,
            SIZE
        };
    // Metode koje moraju osigurati sva stanja igre.
    virtual void init() = 0;
    virtual void handleEvents(sf::Event event) = 0;
    virtual void update(sf::Time dt = sf::Time::Zero) = 0;
    virtual void render() = 0;

protected:
    Game * const mpGame; // Pokazivač na Game klasu
    sf::Font mFont;  
};


class WelcomeState : public GameState{
public:
    WelcomeState(Game * pGame);
    void init() override{}
    void handleEvents(sf::Event event) override;
    void render() override;
    void update(sf::Time dt = sf::Time::Zero) override{}
private:
    sf::Text mMessageTitle;
    sf::Text mMessage;
};

class ScoreState : public GameState{
public:
    ScoreState(Game * pGame);
    void init() override;
    void update(sf::Time dt = sf::Time::Zero) override{}
    void handleEvents(sf::Event event) override;
    void render() override;
private:
    int mScore;
    sf::Text mScoreText;
    sf::Text mEndMessage;
};


