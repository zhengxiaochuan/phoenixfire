#include "Game.h"

#include "Bullet.h"

USING_NS_CC;

CCScene* Game::scene()
{
    CCScene *scene = CCScene::create();
    Game *layer = Game::create();
    scene->addChild(layer);

    return scene;
}

bool Game::init()
{
	if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) )
    {
        return false;
    }
    
    visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	bulletsList = CCSet::create();
	bulletsList->retain();

    // init hero
    Hero = Plane::create("hero.png", 0.3f);
	Hero->setScale(0.13f);
    Hero->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/10 + origin.y));
    this->addChild(Hero, 1);

	this->scheduleUpdate();
	this->setTouchEnabled(true);
    
    return true;
}

void Game::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *pTouch = (CCTouch *)pTouches->anyObject();
	CCPoint currentPoint = pTouch->getLocationInView();
	currentPoint = CCDirector::sharedDirector()->convertToGL(currentPoint);
	CCPoint previousPoint = pTouch->getPreviousLocationInView();
	previousPoint = CCDirector::sharedDirector()->convertToGL(previousPoint);

	CCPoint distance = ccpSub(currentPoint, previousPoint);
	CCPoint destination = ccpAdd(Hero->getPosition(), distance);

	Hero->moveTo(destination);
}

float fireEneryBar = 0;
float interval = 0.6f;
void Game::update(float delta)
{
	Bullet *lastBullet = Hero->fire(delta);
	if(lastBullet)
	{
		bulletsList->addObject(lastBullet);
		addChild(lastBullet, 0);
	}

	CCSetIterator it = bulletsList->begin();
	Bullet *bullet;
	CCSet *bulletsToDelete = CCSet::create();

	while (it!=bulletsList->end())
	{
		bullet = (Bullet *)(*it);
		if(bullet->getPositionY() > visibleSize.height)
		{
			bulletsToDelete->addObject(bullet);
		}
		it++;
	}

	it = bulletsToDelete->begin();
	while(it != bulletsToDelete->end())
	{
		bullet = (Bullet *)(*it);
		bulletsList->removeObject(bullet);
		bullet->removeFromParent();
		it++;	
	}
}