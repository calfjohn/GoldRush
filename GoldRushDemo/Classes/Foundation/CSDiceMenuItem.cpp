#include "CSDiceMenuItem.h"
#include "../GAME/CSController.h"

CSDiceMenuItem * CSDiceMenuItem::itemFromSprite(SelectorProtocol* target, SEL_MenuHandler selector)
{
	CSDiceMenuItem *pRet = new CSDiceMenuItem();
	if (pRet && pRet->initFromNormalImage("转盘.png", "转盘.png", NULL, target, selector))
	{
		pRet->autorelease();
		pRet->init();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);
	return NULL;
}


bool CSDiceMenuItem::init()
{
	bSelect = false;

	m_pDice1 = CCSprite::spriteWithFile("滚球绿色.png");
	addChild(m_pDice1);
	m_pDice1->setAnchorPoint(CCPointZero);
	m_pDice1->setPosition(ccp(getContentSize().width/2-30, getContentSize().height/2));
	
	m_pDice2 = CCSprite::spriteWithFile("滚球黄色.png");
	addChild(m_pDice2);
	m_pDice2->setAnchorPoint(CCPointZero);
	m_pDice2->setPosition(ccp(getContentSize().width/2, getContentSize().height/2+20));
	
	m_pDice3 = CCSprite::spriteWithFile("滚球蓝色.png");
	addChild(m_pDice3);
	m_pDice3->setAnchorPoint(CCPointZero);
	m_pDice3->setPosition(ccp(getContentSize().width/2+20, getContentSize().height/2));
	
	m_pSprite = CCSprite::spriteWithFile("转盘旋转低速.png");
	addChild(m_pSprite);
	m_pSprite->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
	m_pSprite->setIsVisible(false);


	return true;
}

void CSDiceMenuItem::selected()
{
	if (bSelect)
	{
		return;
	}
	
	reorderChild(m_pSprite, 0);
	m_pSprite->setIsVisible(true);
	m_pSprite->runAction(CCSequence::actions(
		CCRepeat::actionWithAction(CCRotateBy::actionWithDuration(0.5, 360), 5), 
		CCCallFunc::actionWithTarget(this, callfunc_selector(CSDiceMenuItem::UpdateInfo)), NULL));

	CCMenuItemImage::selected();

}

void CSDiceMenuItem::UpdateInfo()
{
	bSelect = false;
	m_pSprite->setIsVisible(false);
	m_pSprite->stopAllActions();

	m_pDice1->setPosition(GetPos(DataManager::sharedDataManager()->m_mapDice[1], 1));
	m_pDice2->setPosition(GetPos(DataManager::sharedDataManager()->m_mapDice[2], 2));
	m_pDice3->setPosition(GetPos(DataManager::sharedDataManager()->m_mapDice[3], 3));

	CSController::sharedController()->ToSail();
}

CCPoint CSDiceMenuItem::GetPos(int nDice, int nStep)
{
	CCPoint nPos = CCPointZero;

	if (nStep == 1)
	{
		switch(nDice)
		{
		case 1:
			nPos = ccp(33,75);
			break;
		case 2:
			nPos = ccp(70,60);
			break;
		case 3:
			nPos = ccp(65,18);
			break;
		case 4:
			nPos = ccp(45,10);
			break;
		case 5:
			nPos = ccp(20,20);
			break;
		case 6:
			nPos = ccp(14,58);
			break;
		}
	}
	else if (nStep == 2)
	{
		switch(nDice)
		{
		case 1:
			nPos = ccp(40,75);
			break;
		case 2:
			nPos = ccp(65,68);
			break;
		case 3:
			nPos = ccp(72,25);
			break;
		case 4:
			nPos = ccp(33,12);
			break;
		case 5:
			nPos = ccp(15,28);
			break;
		case 6:
			nPos = ccp(18,70);
			break;
		}
	}
	else if (nStep == 3)
	{
		switch(nDice)
		{
		case 1:
			nPos = ccp(55,75);
			break;
		case 2:
			nPos = ccp(75,46);
			break;
		case 3:
			nPos = ccp(75,38);
			break;
		case 4:
			nPos = ccp(50,13);
			break;
		case 5:
			nPos = ccp(13,38);
			break;
		case 6:
			nPos = ccp(12,50);
			break;
		}
	}


	return nPos;
}
