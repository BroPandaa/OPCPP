#include "entities.h"

// Dopuniti po potrebi.
Entity::Entity(){}

Entity::Entity(sf::Texture const & texture){
    mSprite.setTexture(texture);
    auto lb = mSprite.getLocalBounds();
    mSprite.setOrigin(lb.width/2, lb.height/2);
    mVelocity.x=mVelocity.y= 30.0f; 
    isAlive = true;

    mRadius = 1.0*std::min(lb.width/2,lb.height/2);
    mCircle.setRadius(mRadius);
    mCircle.setFillColor(sf::Color(0, 255, 0, 0));
    mCircle.setOutlineThickness(1);
    mCircle.setOutlineColor(sf::Color::Yellow);
    mCircle.setOrigin(mRadius, mRadius);
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    states.transform *= getTransform();
    target.draw(mSprite, states);
    //target.draw(mCircle, states);
}

void Entity::update(sf::Time dt){
    auto pos = getPosition();
    float x = pos.x + mVelocity.x*dt.asSeconds();
    float y = pos.y + mVelocity.y*dt.asSeconds();
    setPosition(x,y);
}

Animation::Animation(sf::Texture const & texture, int rows, int cols, int width, int height, float vel) :
  Entity(texture), mRows(rows), mCols(cols), mWidth(width/cols), mHeight(height/rows), mSpeed(vel),
  mCurrentCol(0.0f),  mCurrentRow(0) {

  }

 void Animation::update(sf::Time dt){
    if(!isAlive) return;
    mCurrentCol += mSpeed;
    if(static_cast<int>(mCurrentCol) == mCols){
        mCurrentCol = 0.0f;
        ++mCurrentRow;
    }
    if(mCurrentRow == mRows){
        mCurrentCol = 0.0f;
        mCurrentRow = 0;
        isAlive = false;
        return;
    }

    int i = mCurrentCol;
    int j = mCurrentRow; 
    mSprite.setTextureRect(sf::IntRect(i*mWidth, j*mHeight, mWidth, mHeight));
 } 

Asteroid::Asteroid(sf::Texture const & texture) : Entity(texture)
{
    std::random_device dev;
    std::default_random_engine generator(dev());
    std::uniform_real_distribution<float> vel(-60.0f,60.0f);
    std::uniform_int_distribution<int> rot(-5,5);
    mVelocity.x= vel(generator);
    mVelocity.y = vel(generator);
    mRotation=rot(generator);
}

void Asteroid::update(sf::Time dt){
    auto pos = getPosition();
    if((int)pos.x==1280) pos.x=1.0f;
    if((int)pos.x==0) pos.x=1280.0f;
    if((int)pos.y==1024) pos.y=1.0f;
    if((int)pos.y==0) pos.y=1024.0f;
    float x = pos.x + mVelocity.x*dt.asSeconds();
    float y = pos.y + mVelocity.y*dt.asSeconds();
    setPosition(x,y);
    rotate(mRotation);
}

Player::Player(sf::Texture const & texture):Entity(texture)
{}

void Player::update(int vel, sf::Time dt, float rotate){
    auto pos = getPosition();
    auto lb  = mSprite.getLocalBounds();
    if((int)pos.x>=1280) pos.x=1.0f;
    if((int)pos.x<=0) pos.x=1280.0f;
    if((int)pos.y>=1024) pos.y=1.0f;
    if((int)pos.y<=0) pos.y=1024.0f;
    float x = pos.x + lb.width/2*dt.asSeconds()*(-1)*std::cos(mSprite.getRotation()*M_PI/180.0f+M_PI/2)*vel;
    float y = pos.y + lb.height*dt.asSeconds()*std::sin(mSprite.getRotation()*M_PI/180.0f-M_PI/2)*vel;
    mSprite.rotate(rotate*dt.asSeconds());
    setPosition(x,y);
}

Bullet::Bullet(sf::Texture const & texture): Entity(texture)
{
    mVelocity.x=mVelocity.y= 40.0f;
}

void Bullet::spawn(sf::Vector2f pos, float rotation){
    mSprite.setPosition(pos);
    mSprite.setRotation(rotation);
}

void Bullet::update(sf::Time dt){
    auto pos = getPosition();
    auto lb  = mSprite.getLocalBounds();
    float x  = pos.x + lb.width/2*dt.asSeconds()*std::cos(mSprite.getRotation()*M_PI/180.0f)*mVelocity.x;
    float y  = pos.y + lb.height*dt.asSeconds()*std::sin(mSprite.getRotation()*M_PI/180.0f)*mVelocity.y;
    setPosition(x,y);
}

bool Bullet::isInside()
{
    auto pos = mSprite.getPosition();
    if((int)pos.x>=1280 || (int)pos.x<=0 || (int)pos.y<=0 || (int)pos.y>=1024) return false;
    else return true;
}

Bullet::~Bullet() = default;

