
#include "CSLayer.h"


CSLayer::CSLayer()
{
	m_nCFPriority = NORMAL_PRIORITY;
	m_bCFSwallowsTouches = true;
	setIsTouchEnabled(true);
}

CSLayer::~CSLayer()
{
}

int CSLayer::getPriority(void)
{
	return m_nCFPriority;
}

void CSLayer::setPriority(int nPriority)
{
	m_nCFPriority = nPriority;
}

bool CSLayer::isSwallowsTouches(void)
{
	return m_bCFSwallowsTouches;
}

void CSLayer::setSwallowsTouches(bool bSwallowsTouches)
{
	m_bCFSwallowsTouches = bSwallowsTouches;
}

void CSLayer::registerWithTouchDispatcher(void)
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, m_nCFPriority, m_bCFSwallowsTouches);
}

bool CSLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

bool CSLayer::containsTouchLocation(CCSprite *pSpirte, CCTouch* touch)
{
	if (!pSpirte)
	{
		return false;
	}
	
	CCSize s = pSpirte->getTexture()->getContentSize();
	
	return CCRect::CCRectContainsPoint(CCRectMake(0, 0, s.width, s.height), pSpirte->convertTouchToNodeSpace(touch));
}