#include "CSMenuItem.h"

CSMenuItem::CSMenuItem()
{

}

CSMenuItem::~CSMenuItem()
{

}

CSMenuItem * CSMenuItem::itemFromNormalImage(const char *normalImage, const char *selectedImage)
{
	return CSMenuItem::itemFromNormalImage(normalImage, selectedImage, NULL, NULL, NULL);
}

CSMenuItem * CSMenuItem::itemFromNormalImage(const char *normalImage, const char *selectedImage, SelectorProtocol* target, SEL_MenuHandler selector)
{
	return CSMenuItem::itemFromNormalImage(normalImage, selectedImage, NULL, target, selector);
}

CSMenuItem * CSMenuItem::itemFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, SelectorProtocol* target, SEL_MenuHandler selector)
{
	CSMenuItem *pRet = new CSMenuItem();
	
	CCNode *normalSprite = CCSprite::spriteWithSpriteFrameName(normalImage);
	CCNode *selectedSprite = NULL;
	CCNode *disabledSprite = NULL;
	
	if (selectedImage)
	{
		selectedSprite = CCSprite::spriteWithSpriteFrameName(selectedImage);
	}

	if (pRet && pRet->initFromNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector))
	{
		pRet->autorelease();
		return pRet;
	}
	
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool CSMenuItem::init()
{
	return true;
}

void CSMenuItem::selected()
{
	setScale((float)1.2);
	CCMenuItemSprite::selected();
}
void CSMenuItem::unselected()
{
	setScale((float)1.0);
	CCMenuItemSprite::unselected();
}