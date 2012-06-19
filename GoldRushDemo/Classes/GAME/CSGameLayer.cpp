
#include "CSGameLayer.h"
#include "CSSeacraft.h"
#include "CSController.h"

#define kTagTileMap 100
#define kTagSeaCraft 101
CSGameLayer::CSGameLayer()
{
	m_preDistance = 0.0;
	m_lastClick = 0;
	m_bStatues = 0;
}

bool CSGameLayer::init()
{
	CCSprite *pMap = CCSprite::spriteWithSpriteFrameName("portvila.png");
	pMap->setAnchorPoint(CCPointZero);
	addChild(pMap, 0, kTagTileMap);
	adjustlimit();
	pMap->setPosition(ccp(-m_limitPos.x, 0));

	setIsTouchEnabled(true);

	CSSeacraft *pSeaCraft;
	map<int, SEACRAFT>::iterator itercraft;
	for (itercraft = DataManager::sharedDataManager()->m_mapSeacraft.begin(); itercraft != DataManager::sharedDataManager()->m_mapSeacraft.end(); itercraft++)
	{
		pSeaCraft = CSSeacraft::spriteWithFile((*itercraft).second.pCargo->pStock->seaCraftFile.c_str(), (*itercraft).second.id);
		pSeaCraft->setScale(0.5);
		pMap->addChild(pSeaCraft, 0, kTagSeaCraft);
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCRotateBy::actionWithDuration(1, 5),
														  CCDelayTime::actionWithDuration(CCRANDOM_0_1()+0.5),
														  CCRotateBy::actionWithDuration(1, -5),
														  NULL);
		
		pSeaCraft->runAction(CCRepeatForever::actionWithAction((CCActionInterval*)action));
	}

	VisionFollow(0);
//	this->schedule(schedule_selector(CSGameLayer::VisionFollow), 0.1);

	return true;
}

CSGameLayer::~CSGameLayer()
{
}

bool CSGameLayer::containsTouchLocation(CCSprite *pSpirte, CCTouch* touch)
{
	if (!pSpirte)
	{
		return false;
	}
	
	CCSize s = pSpirte->getTexture()->getContentSize();
	
	return CCRect::CCRectContainsPoint(CCRectMake(0, 0, s.width, s.height), pSpirte->convertTouchToNodeSpace(touch));
}

void CSGameLayer::adjustlimit()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCNode *node = getChildByTag(kTagTileMap);
	
	m_mapSize.width = node->getScaleX()*node->getContentSize().width;
	m_mapSize.height = node->getScaleY()*node->getContentSize().height;
	m_limitPos = ccp(m_mapSize.width - winSize.width, m_mapSize.height - winSize.height);
}

void CSGameLayer::adjustPosition(CCPoint &newPosition)
{
	//X轴最小为止
	if (newPosition.x + m_limitPos.x < 1e-10) 
	{
		newPosition.x = -m_limitPos.x;
	}
	
	//y轴最小位置 
	if (newPosition.y + m_limitPos.y < 1e-10) 
	{
		newPosition.y = -m_limitPos.y;
	}
	
	//X轴最大为止
	if (newPosition.x > 1e-10) 
	{
		newPosition.x = 0.0;
	}
	
	//y轴最大位置
	if (newPosition.y > 1e-10) 
	{
		newPosition.y = 0.0;
	}
	
}

void CSGameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if (pTouches->count() > 1) 
	{		
		CCPoint m_Pos1 = CCPointZero;
		CCPoint m_Pos2 = CCPointZero;
		
		CCSetIterator it;
		CCTouch* touch;
		for( it = pTouches->begin(); it != pTouches->end(); it++) 
		{
			touch = (CCTouch*)(*it);
			
			if(!touch)
				break;
			
			CCPoint location = touch->locationInView(touch->view());
			location = CCDirector::sharedDirector()->convertToGL(location);
			CCLog("ccTouchesBegan:touchx:%.2f    touchy:%.2f",location.x, location.y);

			if (it == pTouches->begin()) 
			{
				m_Pos1 = location;	
			}
			else 
			{
				m_Pos2 = location;
			}
		}
		
		m_preDistance = ccpDistance(m_Pos1, m_Pos2);
	}
	else
	{
		CCTouch* touch = (CCTouch*)(*pTouches->begin());
		CCPoint location = touch->locationInView(touch->view());
		location = CCDirector::sharedDirector()->convertToGL(location);
		CCPoint pos  = ccp(getChildByTag(kTagTileMap)->getContentSize().width - (m_limitPos.x + getChildByTag(kTagTileMap)->getPosition().x) - CCDirector::sharedDirector()->getWinSize().width + location.x, -getChildByTag(kTagTileMap)->getPosition().y + location.y);
		CCLog("ccTouchesBegan:touchx:%.2f    touchy:%.2f  %.2f %.2f",location.x, location.y , pos.x, pos.y);
	}
}

void CSGameLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if (pTouches->count() > 1) 
	{
		m_bStatues = 1;

		CCPoint m_Pos1 = CCPointZero;
		CCPoint m_Pos2 = CCPointZero;
		
		CCSetIterator it;
		CCTouch* touch;
		for( it = pTouches->begin(); it != pTouches->end(); it++) 
		{
			touch = (CCTouch*)(*it);
			
			if(!touch)
				break;
			
			CCPoint location = touch->locationInView(touch->view());
			location = CCDirector::sharedDirector()->convertToGL(location);
//			CCLog("ccTouchesBegan:touchx:%.2f    touchy:%.2f",location.x, location.y);
			
			if (it == pTouches->begin()) 
			{
				m_Pos1 = location;	
			}
			else 
			{
				m_Pos2 = location;
			}
		}
		
		CGFloat tempDistance = ccpDistance(m_Pos1, m_Pos2);
		float fScale =  tempDistance / m_preDistance;		
		CCNode *node = getChildByTag(kTagTileMap);
		
		if(node->getScale()*fScale - 0.5 < 1e-10 || node->getScale()*fScale - 1.0 > 1e-10)
		{
			return;
		}

//		CCLog("%.2f  %.2f  %.2f  %.2f  %.2f" ,node->getScale()*fScale, node->getScale(), fScale, tempDistance , m_preDistance);
		node->setScale(node->getScale()*fScale);
		m_preDistance = tempDistance;
		adjustlimit();
	}
	else if(!m_bStatues)
	{	
		CCSetIterator it = pTouches->begin();
		CCTouch* touch = (CCTouch*)(*it);
		
		CCPoint touchLocation = touch->locationInView( touch->view() );	
		CCPoint prevLocation = touch->previousLocationInView( touch->view() );	
		
		touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
		prevLocation = CCDirector::sharedDirector()->convertToGL( prevLocation );
		
		CCPoint diff = ccpSub(touchLocation, prevLocation);
		
		CCNode *node = getChildByTag(kTagTileMap);
		CCPoint currentPos = node->getPosition();
		
		CCPoint newPos = ccpAdd(currentPos, diff);
		adjustPosition(newPos);		
		node->setPosition(newPos);
		//CCLog("new: x:%.2f %.2f current x:%.2f y:%.2f	diff x:%.2f y:%.2f", newPos.x, newPos.y, currentPos.x, currentPos.y, diff.x, diff.y);
	}
}

void CSGameLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCLog("ccTouchesEnded");
	m_bStatues = 0;
}

void CSGameLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	ccTouchesEnded(pTouches, pEvent);
}

void CSGameLayer::UpdateLayer()
{
	//更新船位置
	CSSeacraft *pSeaCraft;
	CCArray* pChildren = getChildByTag(kTagTileMap)->getChildren();
	if(pChildren && pChildren->count() > 0)
	{
		int nDelay = 0;
		CCObject* child;
		CCARRAY_FOREACH(pChildren, child)
		{
			CCNode* pChild = (CCNode*) child;
			if (pChild && pChild->getTag() == kTagSeaCraft)
			{
				pSeaCraft = (CSSeacraft *)pChild;
				int nTemp = pSeaCraft->ToSail(false);
				if (nDelay < nTemp)
				{
					nDelay = nTemp;
				}
			}
		}

		if (nDelay != 0)
		{
			CCFiniteTimeAction*  action = CCSequence::actions(
				CCDelayTime::actionWithDuration((ccTime)nDelay),
				CCCallFunc::actionWithTarget(this, callfunc_selector(CSGameLayer::callbackSail)), 
				NULL);
			runAction(action);
		}
	}
}

void CSGameLayer::callbackSail()
{
	CSController::sharedController()->SailTo();
}

void CSGameLayer::VisionFollow(ccTime dt)
{
    CC_UNUSED_PARAM(dt);

	//取最前那艘船作为视觉中心点，移动地图
	CCArray* pChildren = getChildByTag(kTagTileMap)->getChildren();
	if(pChildren && pChildren->count() > 0)
	{
		CCPoint positionMap = getChildByTag(kTagTileMap)->getPosition();
		CCPoint pointMove = ccp(1e+10, positionMap.y);
		CCObject* child;
		CCARRAY_FOREACH(pChildren, child)
		{
			CCNode* pChild = (CCNode*) child;
			if (pChild && pChild->getTag() == kTagSeaCraft)
			{
				float fTemp = pChild->getPosition().x;
				if (pointMove.x > fTemp)
				{
					pointMove.x = fTemp;
				}
			}
		}

		//CCLOG("船位置%.f 地图位置 %f", pointMove.x, positionMap.x);
		pointMove.x = 240 - pointMove.x;
 		adjustPosition(pointMove);
 
 		if (pointMove.x != positionMap.x)
 		{	
 			getChildByTag(kTagTileMap)->runAction(CCMoveTo::actionWithDuration(abs((int)(positionMap.x - pointMove.x))/50, pointMove));
 		}
	}
}