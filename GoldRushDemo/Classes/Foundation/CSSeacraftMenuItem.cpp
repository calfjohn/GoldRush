#include "CSSeacraftMenuItem.h"
#include "../GAME/CSController.h"

#define kTagEmployee 100

CSSeacraftMenuItem * CSSeacraftMenuItem::itemFromSprite(int id, SelectorProtocol* target, SEL_MenuHandler selector)
{
	CSSeacraftMenuItem *pRet = new CSSeacraftMenuItem(id);
	
	SEACRAFT *pSeaCraft = DataManager::sharedDataManager()->FindSeaCraft(id);
	if (!pSeaCraft)
	{
		return NULL;
	}	
	
	if (pRet && pRet->initFromNormalImage(pSeaCraft->pCargo->pStock->seaCraftFile.c_str(), pSeaCraft->pCargo->pStock->seaCraftFile.c_str(), NULL, target, selector))
	{
		pRet->autorelease();
		pRet->init();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool CSSeacraftMenuItem::init()
{
	m_pForground = CCSprite::spriteWithSpriteFrameName("锦旗01.png");
	addChild(m_pForground);
	m_pForground->setPosition(ccp(getContentSize().width/2, -10));
	
	UpdateInfo();
	
	return true;
}

void CSSeacraftMenuItem::UpdateInfo()
{
	m_pForground->removeAllChildrenWithCleanup(true);
	
	int nStep;			//间隔长度
	CCPoint pointBegin;	//起始位置
	SEACRAFT *pSeaCraft = DataManager::sharedDataManager()->FindSeaCraft(m_nId);
	if (pSeaCraft->pCargo->spotList.size() == 4)
	{
		nStep = 15;
		pointBegin = ccp(30, 15);
	}
	else//==3
	{
		nStep = 25;
		pointBegin = ccp(30, 15);
	}
	
	int i;
	vector<GAMESPOT>::iterator iter;
	for(i=1, iter = pSeaCraft->pCargo->spotList.begin(); iter != pSeaCraft->pCargo->spotList.end(); iter++,i++)
	{
		CCLabelTTF *pLabel = CCLabelTTF::labelWithString(ConvertToString(i).c_str(), "Airal", 18);
		m_pForground->addChild(pLabel);
		pLabel->setAnchorPoint(CCPointZero);
		pLabel->setPosition(ccp(pointBegin.x + nStep*(i-1), pointBegin.y));
		pLabel->setColor(ccBLACK);
		
		USER *pUser = DataManager::sharedDataManager()->FindUserById((*iter).uid);
		if(pUser)
		{
			CCSprite *pEmployee = CCSprite::spriteWithSpriteFrameName(pUser->pEmployee->employeeFile.c_str());
			m_pForground->addChild(pEmployee, ZORDER_LOW, kTagEmployee);
			pEmployee->setAnchorPoint(CCPointZero);
			pEmployee->setPosition(ccp(pointBegin.x - 20 + nStep*(i-1), 0));
		}
	}
}

void CSSeacraftMenuItem::selected()
{
	setScale((float)0.84);
	CCMenuItemImage::selected();
}
void CSSeacraftMenuItem::unselected()
{
	setScale((float)0.7);
	CCMenuItemImage::unselected();
}