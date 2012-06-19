#include "CSRudderMenuItem.h"
#include "../GAME/CSController.h"

CSRudderMenuItem * CSRudderMenuItem::itemFromSprite(SelectorProtocol* target, SEL_MenuHandler selector)
{
	CSRudderMenuItem *pRet = new CSRudderMenuItem();
	
	CCNode *normalSprite = CCSprite::spriteWithSpriteFrameName("rudder.png");
	CCNode *SelectSprite = CCSprite::spriteWithSpriteFrameName("rudder.png");
	if (pRet && pRet->initFromNormalSprite(normalSprite, SelectSprite, NULL, target, selector))
	{
		pRet->autorelease();
		pRet->init();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);
	return NULL;
}


bool CSRudderMenuItem::init()
{
	bSelect = false;

	return true;
}

void CSRudderMenuItem::selected()
{
	if (bSelect)
	{
		return;
	}
	
	runAction(CCSequence::actions(
		CCRepeat::actionWithAction(CCRotateBy::actionWithDuration(0.8, 360), 5), 
		CCCallFunc::actionWithTarget(this, callfunc_selector(CSRudderMenuItem::UpdateInfo)), NULL));

	CCMenuItemImage::selected();

}

void CSRudderMenuItem::UpdateInfo()
{
	bSelect = false;
	CSController::sharedController()->NextTips();
	CSController::sharedController()->ToSail();
}
