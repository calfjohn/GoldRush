
#include "CSSprite.h"

CSSprite::CSSprite()
{
	m_nCFPriority = NORMAL_PRIORITY;
	m_bCFSwallowsTouches = true;
}

CSSprite::~CSSprite()
{
}


int CSSprite::getPriority(void)
{
	return m_nCFPriority;
}

void CSSprite::setPriority(int nPriority)
{
	m_nCFPriority = nPriority;
}


bool CSSprite::isSwallowsTouches(void)
{
	return m_bCFSwallowsTouches;
}

void CSSprite::setSwallowsTouches(bool bSwallowsTouches)
{
	m_bCFSwallowsTouches = bSwallowsTouches;
}

CCRect CSSprite::rect()
{
	CCSize s = getTexture()->getContentSize();
	return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

CCRect CSSprite::GetWindowRect()
{
	return CCRectMake(
		m_tPosition.x - (m_tContentSize.width/2),
		m_tPosition.y - (m_tContentSize.height/2),
		m_tContentSize.width,
		m_tContentSize.height);
}

void CSSprite::onEnter()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,m_nCFPriority, m_bCFSwallowsTouches);
	CCSprite::onEnter();
}

void CSSprite::onExit()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}	


bool CSSprite::containsTouchLocation(CCTouch* touch)
{
	return CCRect::CCRectContainsPoint(rect(), convertTouchToNodeSpaceAR(touch));
}

bool CSSprite::containsTouchLocation(CCSprite *pSpirte, CCTouch* touch)
{
	if (!pSpirte)
	{
		return false;
	}
	
	CCSize s = pSpirte->getTexture()->getContentSize();

	return CCRect::CCRectContainsPoint(CCRectMake(0, 0, s.width, s.height), pSpirte->convertTouchToNodeSpace(touch));
}

bool CSSprite::ccTouchBegan(CCTouch* touch, cocos2d::CCEvent* event)
{
	if ( !containsTouchLocation(touch) ) return false;

	return true;
}

void CSSprite::ccTouchMoved(CCTouch* touch, cocos2d::CCEvent* event)
{

}

void CSSprite::ccTouchEnded(CCTouch* touch, cocos2d::CCEvent* event)
{
} 

CSSprite* CSSprite::spriteWithFile(const char *pszFileName)
{
	CSSprite *pobSprite = new CSSprite();
	pobSprite->initWithFile(pszFileName);
	pobSprite->autorelease();

	return pobSprite;
}