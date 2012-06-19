
#include "CSSeaCraftPopupMsg.h"
#include "CSMenuItem.h"

CSSeaCraftPopupMsg::CSSeaCraftPopupMsg(int nPlayerId, int nSeaCraftId,  SelectorProtocol* target, SEL_CallFuncND selector, void *sender):CSLayerPopupMsg(target,selector,sender)
{
	m_nCFPriority = HIGHEST_PRIORITY;

	m_strMessage = "";
	DataManager::GBKToUTF8(m_strMessage);
	m_strTitle = "";
	m_popEnum = POPUP_YESNO;
	m_responseEnum = RESPONSE_YES;
	m_strBackground = "界面01.png";

	m_nPlayerId  = nPlayerId;
	m_nSeaCraftId = nSeaCraftId;
}

bool CSSeaCraftPopupMsg::init()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	SEACRAFT *pSeaCraft = DataManager::sharedDataManager()->FindSeaCraft(m_nSeaCraftId);

	int nGold = 0;
	vector<GAMESPOT>::iterator iter;
	for (iter = pSeaCraft->pCargo->spotList.begin(); iter != pSeaCraft->pCargo->spotList.end(); iter++)
	{
		if ( (*iter).uid == NO_USER)
		{		
			nGold = (*iter).buy;
			break;
		}
	}
	
	//添加背景
	CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName(m_strBackground.c_str());
	pSprite->setPosition(ccp(size.width/2, size.height/2));
	addChild(pSprite);

	//船图案
	CCPoint postionTemp;
	CCSprite *pSpriteTemp = CCSprite::spriteWithSpriteFrameName(pSeaCraft->pCargo->pStock->seaCraftFile.c_str());
	pSpriteTemp->setPosition(ccp(pSprite->getContentSize().width/2, pSprite->getContentSize().height - pSpriteTemp->getContentSize().height/2 + 30));
	postionTemp = pSpriteTemp->getPosition();
	pSprite->addChild(pSpriteTemp);

	CCLabelTTF *pLabel;
	string strTemp = "收益";
	DataManager::GBKToUTF8(strTemp);
	sprintf((char *)strTemp.c_str(), "收益%dG", pSeaCraft->pCargo->pStock->value);
	DataManager::GBKToUTF8(strTemp);
	pLabel = CCLabelTTF::labelWithString(strTemp.c_str(), "DFPHaiBaoW12-GB", 16);
	pLabel->setColor(ccORANGE);
	pLabel->setPosition(ccp(postionTemp.x + 80, postionTemp.y));
	pSprite->addChild(pLabel);

	postionTemp = ccp(pSpriteTemp->getPosition().x, pSpriteTemp->getPosition().y - pSpriteTemp->getContentSize().height/2);

	//提示信息
	strTemp = "您希望花费    购买        号船位吗？";
	DataManager::GBKToUTF8(strTemp);
	pLabel = CCLabelTTF::labelWithString(strTemp.c_str(), CCSizeMake(230, 0), CCTextAlignmentLeft, "DFPHaiBaoW12-GB", 12);
	pLabel->setPosition(ccp(postionTemp.x, postionTemp.y - pLabel->getContentSize().height/2 - 10));
	postionTemp = pLabel->getPosition();
	pSprite->addChild(pLabel);

	sprintf((char *)strTemp.c_str(), "%dG", nGold);
	DataManager::GBKToUTF8(strTemp);
	pLabel = CCLabelTTF::labelWithString(strTemp.c_str(), "DFPHaiBaoW12-GB", 12);
	pLabel->setColor(ccORANGE);
	pLabel->setPosition(ccp( postionTemp.x - 20, postionTemp.y + 5));
	pSprite->addChild(pLabel);

	strTemp = pSeaCraft->pCargo->pStock->seaCraftName;
	DataManager::GBKToUTF8(strTemp);
	pLabel = CCLabelTTF::labelWithString(strTemp.c_str(), "DFPHaiBaoW12-GB", 12);
	pLabel->setColor(ccYELLOW);
	pLabel->setPosition(ccp( postionTemp.x + 55, postionTemp.y + 5));
	pSprite->addChild(pLabel);

	//货物图案及信息
	pSpriteTemp = CCSprite::spriteWithSpriteFrameName(pSeaCraft->pCargo->pStock->stockFile.c_str());
	pSpriteTemp->setPosition(ccp(postionTemp.x - pSpriteTemp->getContentSize().width, postionTemp.y - pSpriteTemp->getContentSize().height/2 - 10));
	postionTemp = pSpriteTemp->getPosition();
	pSprite->addChild(pSpriteTemp);

	sprintf((char *)strTemp.c_str(), "%s股价%dG", pSeaCraft->pCargo->pStock->name.c_str(), pSeaCraft->pCargo->pStock->price);
	DataManager::GBKToUTF8(strTemp);
	pLabel = CCLabelTTF::labelWithString(strTemp.c_str(), "DFPHaiBaoW12-GB", 12);
	pLabel->setPosition(ccp( postionTemp.x + 70, postionTemp.y));
	pSprite->addChild(pLabel);


	USER *pUser = DataManager::sharedDataManager()->FindUserById(m_nPlayerId);
	CCSprite *pSpritePlayer = CCSprite::spriteWithSpriteFrameName(pUser->pEmployee->employeeFile.c_str());
	pSpritePlayer->setOpacity(128);
	pSprite->addChild(pSpritePlayer);

	postionTemp = ccp(pSprite->getContentSize().width/2 - pSpritePlayer->getContentSize().width, postionTemp.y - pSpritePlayer->getContentSize().height/2 -20);

	strTemp = "船位";
	DataManager::GBKToUTF8(strTemp);
	pLabel = CCLabelTTF::labelWithString(strTemp.c_str(), "DFPHaiBaoW12-GB", 12);
	pLabel->setPosition(ccp( postionTemp.x - 30, postionTemp.y));
	pSprite->addChild(pLabel);

	strTemp = "买入价";
	DataManager::GBKToUTF8(strTemp);
	pLabel = CCLabelTTF::labelWithString(strTemp.c_str(), "DFPHaiBaoW12-GB", 12);
	pLabel->setPosition(ccp( postionTemp.x - 30, postionTemp.y - pSpriteTemp->getContentSize().height/2));
	pSprite->addChild(pLabel);

	//船员图案及信息
	for (iter = pSeaCraft->pCargo->spotList.begin(); iter != pSeaCraft->pCargo->spotList.end(); iter++)
	{
		if ( (*iter).uid == NO_USER)
		{
			if (pSpritePlayer)
			{
				pSpritePlayer->setPosition(postionTemp);
				pSpritePlayer = NULL;
			}
		}
		else
		{
			USER *pUser = DataManager::sharedDataManager()->FindUserById((*iter).uid);
			if (pUser)
			{
				pSpriteTemp = CCSprite::spriteWithSpriteFrameName(pUser->pEmployee->employeeFile.c_str());
				pSpriteTemp->setPosition(postionTemp);
				pSprite->addChild(pSpriteTemp);
			}
		}

		sprintf((char *)strTemp.c_str(), "%d", (*iter).buy);
		DataManager::GBKToUTF8(strTemp);
		pLabel = CCLabelTTF::labelWithString(strTemp.c_str(), "DFPHaiBaoW12-GB", 16);
		pLabel->setPosition(ccp( postionTemp.x, postionTemp.y - pSpriteTemp->getContentSize().height/2));
		pLabel->setColor(ccORANGE);
		pSprite->addChild(pLabel);

		postionTemp.x += 60;
	}	




	//添加按钮
	CCMenu *pMenu = CCMenu::menuWithItems(NULL);
	pMenu->setPosition(CCPointZero);

	CCMenuItem *pItem;
	if (m_popEnum == POPUP_YESNO)
	{
		pItem = CSMenuItem::itemFromNormalImage("确定.png", "确定.png", this, menu_selector(CSSeaCraftPopupMsg::callbackClick));
		pItem->setTag(RESPONSE_YES);
		pItem->setPosition(ccp(pSprite->getContentSize().width*3/10, pSprite->getContentSize().height/8));
		pMenu->addChild(pItem, 0);	

		pItem = CSMenuItem::itemFromNormalImage("否.png", "否.png", this, menu_selector(CSSeaCraftPopupMsg::callbackClick));
		pItem->setTag(RESPONSE_NO);
		pItem->setPosition(ccp(pSprite->getContentSize().width*6/10, pSprite->getContentSize().height/8));
		pMenu->addChild(pItem, 0);	
	}

	pSprite->addChild(pMenu);

	return true;
}