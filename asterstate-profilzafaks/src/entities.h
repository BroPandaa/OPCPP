#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#define _USE_MATH_DEFINES
#include <cmath>

class Entity : public sf::Drawable, public sf::Transformable {
public:
    Entity(sf::Texture const & texture);
    Entity();
    void setVelocity(sf::Vector2f const &  vel) { mVelocity = vel;}
    virtual void update(sf::Time dt = sf::Time::Zero); 
    virtual ~Entity() {}
    bool isAlive;
    float radius(){return mRadius;}
    virtual sf::Vector2f getCenter(){return getPosition();}
    sf::Vector2f velocity(){return mVelocity;}
protected:
    sf::Sprite mSprite;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::Vector2f mVelocity;
    sf::CircleShape mCircle;
    float mRadius;
};

class Animation : public Entity{
public:
    Animation(sf::Texture const & texture, int rows, int cols, int width, int height, float vel);
    virtual void update(sf::Time dt = sf::Time::Zero)  override; 
private:
    int mRows, mCols, mWidth, mHeight;
    float mSpeed;
    float mCurrentCol;
    int   mCurrentRow;
};

// dopuniti
class Asteroid : public Entity{
public:
    Asteroid(sf::Texture const & texture);
    void update(sf::Time dt = sf::Time::Zero);
    void setValue(int val){mValue = val;}
    int getValue(){return mValue;}
private:
    float mRotation;
    int mValue=0;
};


// dopuniti
class Player : public Entity{
public:
    Player(sf::Texture const & texture);
    void update(int vel, sf::Time dt = sf::Time::Zero, float rotate=0);
    sf::Vector2f shootPosition(){return getPosition();}
    float shootRotation(){return mSprite.getRotation()-90;}
    void rotation(int rot=0){mSprite.setRotation(rot);}
private:
};

class Bullet : public Entity{
public:
    Bullet(sf::Texture const & texture);
    ~Bullet();
    void spawn(sf::Vector2f pos, float rotation);
    void update(sf::Time dt = sf::Time::Zero);
    bool isInside();
};