
#include "CSSeacraft.h"
#include "CSController.h"

CSSeacraft::CSSeacraft()
{
	bSail = false;
}

CSSeacraft::~CSSeacraft()
{
}

CSSeacraft* CSSeacraft::spriteWithFile(const char *pszFilename, int id)
{
	CSSeacraft *pobSprite = new CSSeacraft();
	pobSprite->initWithSpriteFrameName(pszFilename);
//	pobSprite->initWithFile(pszFilename);
	pobSprite->autorelease();

	pobSprite->SetId(id);
	if (!pobSprite->init())
	{
		return NULL;	
	}

	return pobSprite;
}

bool CSSeacraft::init()
{
	SEACRAFT * pSeaCraft = DataManager::sharedDataManager()->FindSeaCraft(m_id);
	if (!pSeaCraft)
	{
		return false;
	}
	
	m_nPos = pSeaCraft->position;
	POSITION *pPos = DataManager::sharedDataManager()->FindPostion(m_nPos);
	if (pPos)
	{
		setPosition(ccp(pPos->x, pPos->y));
	}	
	
	CCLabelTTF *pLabel = CCLabelTTF::labelWithString(pSeaCraft->pCargo->pStock->seaCraftName.c_str(), "Airal", 20);
	addChild(pLabel, ZORDER_DEFAULT);
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2, 0));

	return true;
}


bool CSSeacraft::ccTouchBegan(CCTouch* touch, cocos2d::CCEvent* event)
{		
//	if(!containsTouchLocation(touch)) calf comment it for test
	{
		return false;
	}
	
	return true;
}

void CSSeacraft::ccTouchMoved(CCTouch* touch, cocos2d::CCEvent* event)
{
}

void CSSeacraft::ccTouchEnded(CCTouch* touch, cocos2d::CCEvent* event)
{
} 

void CSSeacraft::ccTouchCancelled(CCTouch* touch, cocos2d::CCEvent* event)
{
	ccTouchEnded(touch, event);
}

int CSSeacraft::ToSail(bool bDirect)
{
	//bDirect为true 直接航行到目标位置，否则从当前位置航行到位置13后，再移动到目标位置
	int Pos = DataManager::sharedDataManager()->FindSeaCraft(m_id)->position;

	if (Pos == m_nPos)
	{
		return 0;
	}

	if (m_nPos > Pos)
	{
		//倒退，特殊分支
		m_nPos = Pos;
	}

	if (bDirect)
	{
		m_nPos = Pos;
	}
		
	int nTime = 0;
	CCArray *pActionArray = CCArray::array();
	POSITION *srcPos = DataManager::sharedDataManager()->FindPostion(m_nPos);
	for (int i = m_nPos; i <= Pos; i++)
	{
		if (i <= 3)
		{
			i = 4;
		}

		POSITION *pPos = DataManager::sharedDataManager()->FindPostion(i);
		if (pPos == NULL)
		{
			break;
		}

		pActionArray->addObject(CCMoveTo::actionWithDuration(1, ccp(pPos->x ,pPos->y)));
		nTime++;
	}

	m_nPos = Pos;
	runAction(CCSequence::actionsWithArray(pActionArray));	

	return nTime;
}