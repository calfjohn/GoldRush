#include "CSRulerInfo.h"
#include "../GAME/CSController.h"

CSRulerInfo::CSRulerInfo(void)
{
}

CSRulerInfo::~CSRulerInfo(void)
{
}

CSRulerInfo* CSRulerInfo::node()
{
	CSRulerInfo * pobSprite = new CSRulerInfo;
	pobSprite->initWithSpriteFrameName("progress.png");
	pobSprite->init();
	pobSprite->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, pobSprite->getContentSize().height/2));
	pobSprite->autorelease();

	pobSprite->UpdateInfo();

	return pobSprite;
}

bool CSRulerInfo::init()
{
	int i;
	CCSprite *pSeaCraft;
	map<int, SEACRAFT>::iterator iter;
	for (i = 1,iter = DataManager::sharedDataManager()->m_mapSeacraft.begin(); iter != DataManager::sharedDataManager()->m_mapSeacraft.end(); iter++, i++)
	{
		pSeaCraft = CCSprite::spriteWithSpriteFrameName((*iter).second.pCargo->pStock->seaCraftRulerFile.c_str());
		pSeaCraft->setAnchorPoint(CCPointZero);
		addChild(pSeaCraft);
		
		if (i == 1)
		{
			m_pSeaCraft1 = pSeaCraft;
		}
		else if(i == 2)
		{
			m_pSeaCraft2 = pSeaCraft;
		}
		else if (i ==3)
		{
			m_pSeaCraft3 = pSeaCraft;
		}
	}
	return true;
}

void CSRulerInfo::UpdateInfo()
{
	int i;
//	CCSprite *pSeaCraft;
	map<int, SEACRAFT>::iterator iter;
	for (i = 1,iter = DataManager::sharedDataManager()->m_mapSeacraft.begin(); iter != DataManager::sharedDataManager()->m_mapSeacraft.end(); iter++, i++)
	{
		int nTemp = (*iter).second.position;
		if (nTemp > 16)
		{
			nTemp = 17;
		}
		
		nTemp -= 3;
		POSITION *srcPos = DataManager::sharedDataManager()->FindRulerPostion(nTemp);
		if (i == 1)
		{
			m_pSeaCraft1->setPosition(ccp(srcPos->x, srcPos->y));
		}
		else if(i == 2)
		{
			m_pSeaCraft2->setPosition(ccp(srcPos->x, srcPos->y));
			
		}
		else if (i == 3)
		{
			m_pSeaCraft3->setPosition(ccp(srcPos->x, srcPos->y));
		}
	}
}