#include "CSGameSpotMenuItem.h"
#include "../game/DataManager.h"
#define kTagEmployee 100

CSGameSpotMenuItem * CSGameSpotMenuItem::itemFromSprite(int id, bool bVisible, bool bEnable, SelectorProtocol* target, SEL_MenuHandler selector)
{	
	CSGameSpotMenuItem *pRet = new CSGameSpotMenuItem(id);
	
	CCNode *normalSprite = CCSprite::spriteWithSpriteFrameName("gamespot.png");
	CCNode *selectedSprite = CCSprite::spriteWithSpriteFrameName("gamespot.png");
	CCNode *disableSprite = CCSprite::spriteWithSpriteFrameName("gamespot.png");
	
	if (pRet && pRet->initFromNormalSprite(normalSprite, selectedSprite, disableSprite, target, selector))
	{
		pRet->init();
		pRet->setIsVisible(bVisible);
		pRet->setIsEnabled(bEnable);
		pRet->autorelease();

		return pRet;
	}
	
	CC_SAFE_DELETE(pRet);
	return NULL;

}

bool CSGameSpotMenuItem::init()
{
	m_pSprite = NULL;
	UpdateInfo();
	
	return true;
}

void CSGameSpotMenuItem::UpdateInfo()
{
	USER *pUser = DataManager::sharedDataManager()->FindUserByGamespotID(m_nId);
	if (pUser && !m_pSprite)
	{
		m_pSprite = CCSprite::spriteWithSpriteFrameName(pUser->pEmployee->employeeFile_s.c_str());
		addChild(m_pSprite);
		m_pSprite->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
	}
	else if(m_pSprite)
	{
		removeChild(m_pSprite, true);
		m_pSprite = NULL;
	}
}
