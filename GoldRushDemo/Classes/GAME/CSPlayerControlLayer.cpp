
#include "CSPlayerControlLayer.h"
#include "CSController.h"
#include "../Foundation/CSSeaCraftPopupMsg.h"
#include "CSTransactionLayer.h"

#define  kTagLabel 100

bool CSPlayerControlLayer::init()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	//玩家信息-头像、钱、同谋、资产 CSPlayerInfo
	int nTemp = 0;
	map<int ,USER>::iterator iter;
	CSPlayerInfoSprite *pPlayer;
	for (iter = DataManager::sharedDataManager()->m_mapPlayer.begin(); iter != DataManager::sharedDataManager()->m_mapPlayer.end(); iter++, nTemp++)
	{
		pPlayer = CSPlayerInfoSprite::spriteWithPlayer(&(*iter).second);
		addChild(pPlayer, ZORDER_DEFAULT);
		pPlayer->setPosition(ccp(50 + size.width*nTemp/4, size.height - 80));
		
		if (DataManager::sharedDataManager()->getCurrentUserID() == (*iter).first) 
		{
			m_pPlayer = pPlayer;
		}
	}
	
	
	//货船进度 CSRulerInfo
	m_pRuleInfo = CSRulerInfo::node();
	addChild(m_pRuleInfo, ZORDER_DEFAULT);

	//股市 CSStockMarketInfo
	if(DataManager::sharedDataManager()->xmlStockMarketVisible())
	{
		m_pStockMarketInfo = CSStockMarketInfo::node();
		m_pStockMarketInfo->setPosition(CCPointZero);
		addChild(m_pStockMarketInfo, ZORDER_DEFAULT);
	}
	
	CCMenu *pMenu = CCMenu::menuWithItem(NULL);
	addChild(pMenu);
	pMenu->setPosition(CCPointZero);

	m_pSandglass = CCSprite::spriteWithSpriteFrameName("sandglass.png");
	addChild(m_pSandglass, 0);
	m_pSandglass->setPosition(ccp(size.width - 20, size.height - 20));
	
	m_pImageItemHelp = CSMenuItem::itemFromNormalImage("help.png", "help.png", this, menu_selector(CSPlayerControlLayer::restartLevel));
	pMenu->addChild(m_pImageItemHelp, 0);
	m_pImageItemHelp->setPosition(ccp(size.width - 20, size.height - 60));

	m_pImageItemWallet = CSMenuItem::itemFromNormalImage("wallet.png", "wallet.png", this, menu_selector(CSPlayerControlLayer::transaction));
	pMenu->addChild(m_pImageItemWallet, 0);
	m_pImageItemWallet->setPosition(ccp(size.width - 20, 40));
	
	//启航按钮
	m_pRudderMenuItem = CSRudderMenuItem::itemFromSprite(this, menu_selector(CSPlayerControlLayer::callbackNextRound));
	pMenu->addChild(m_pRudderMenuItem, 0);
	m_pRudderMenuItem->setPosition(ccp(m_pRudderMenuItem->getContentSize().width/2, m_pRudderMenuItem->getContentSize().height/2 + m_pRuleInfo->getContentSize().height));
	
	m_pLabel = CCLabelTTF::labelWithString("", "Arial", 22);
	m_pLabel->setColor(ccORANGE);
	addChild(m_pLabel, ZORDER_HIGHEST, kTagLabel);
	m_pLabel->runAction(CCFadeOut::actionWithDuration(0));

	
	CCActionInterval* move = CCMoveBy::actionWithDuration(0.05, CCPointMake(5,0));
	CCActionInterval* move_ease_inout3 = CCEaseInOut::actionWithAction((CCActionInterval*)(move->copy()->autorelease()), 2.0f);
	CCActionInterval* move_ease_inout_back3 = move_ease_inout3->reverse();
	CCFiniteTimeAction* seq1 = CCSequence::actions( move_ease_inout3, move_ease_inout_back3, NULL);
	CCFiniteTimeAction* repeat = CCRepeat::actionWithAction(seq1, 3);
	CCFiniteTimeAction* seq2 = CCSequence::actions( repeat, CCDelayTime::actionWithDuration(2+CCRANDOM_0_1()), NULL);
	m_action = CCRepeatForever::actionWithAction((CCActionInterval*)seq2);	
	CC_SAFE_RETAIN(m_action);
	
	m_nSequence = 0;
	NextTips(NULL);

	return true;
}


void CSPlayerControlLayer::restartLevel(CCObject* sender)
{
	CSController::sharedController()->restartLevel();
}

void CSPlayerControlLayer::transaction(CCObject* sender)
{
	CSController::sharedController()->transaction();
}

void CSPlayerControlLayer::UpdateLayer()
{
	if (m_pPlayer) 
	{
		m_pPlayer->UpdateInfo(DataManager::sharedDataManager()->GetCurrentUser());
	}
	m_pRuleInfo->UpdateInfo();
	
	if (CSController::sharedController()->GetGameRound() == 0)
	{
		NextTips(NULL);
	}
}

void CSPlayerControlLayer::callbackNextRound(CCObject* sender)
{
	CSController::sharedController()->NextRound();
	NextTips(NULL);
}

void CSPlayerControlLayer::NextTips(CCNode* sender)
{
	switch (DataManager::sharedDataManager()->getLevel()) 
	{
		case 1:
			NextTips1();
			break;
		case 2:
			NextTips2();
			break;			
		case 3:
			NextTips3();
			break;
		case 4:
			NextTips4();
			break;
		case 5:
			NextTips5();
			break;
		case 6:
			NextTips6();
			break;
		case 7:
			NextTips7();
			break;
		default:
			break;
	}
}

void CSPlayerControlLayer::NextTips1()
{
	m_nSequence++;
	if (m_nSequence == 1)
	{
		m_pRudderMenuItem->setIsEnabled(false);
		m_pImageItemWallet->setIsEnabled(false);
		
		string str = "淘金客时代是最疯狂的时代，听过太多一夜暴富的故事\n我怀揣着仅有的5个金币也开始了伟大的航行！";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(2),
														  CCDelayTime::actionWithDuration(15),
														  CCMoveBy::actionWithDuration(3, CCPointMake(0, 60)),
														  CCFadeOut::actionWithDuration(3),
														  CCCallFuncN::actionWithTarget(this, callfuncN_selector(CSPlayerControlLayer::NextTips)), 
														  NULL);
		
		m_pLabel->runAction(action);
	}
	else if(m_nSequence == 2)
	{
		char tempInfo[64];
		sprintf(tempInfo, "游戏目标：金币数达到%d", DataManager::sharedDataManager()->xmlWingold());
		//DataManager::GBKToUTF8(str1);
		CCLabelTTF *pLabel = CCLabelTTF::labelWithString(tempInfo, "Arial", 25);
		pLabel->setColor(ccRED);
		addChild(pLabel, ZORDER_HIGHEST);
		pLabel->setPosition(ccp(240, 280));
		
		CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName("goldcoin.png");
		pLabel->addChild(pSprite, ZORDER_HIGHEST);
		pSprite->setPosition(ccp(pLabel->getContentSize().width + pSprite->getContentSize().width, pLabel->getContentSize().height/2));
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  CCMoveBy::actionWithDuration(2, CCPointMake(0, 110)),
														  CCCallFuncN::actionWithTarget(this, callfuncN_selector(CSPlayerControlLayer::NextTips)), 
														  NULL);
		
		pLabel->runAction(action);
	}
	
	else if(m_nSequence == 3)
	{
		m_pImageItemWallet->setIsEnabled(true);
		m_pRudderMenuItem->setIsEnabled(true);

		string str = "请部署船员！（点击钱袋）";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());	
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  NULL);
		
		m_pLabel->runAction(action);
		m_pImageItemWallet->runAction(m_action);
	}
	else if(m_nSequence == 4)
	{
		m_pImageItemWallet->stopAllActions();
		m_pImageItemWallet->setIsEnabled(false);

		string str = "部署完成，启动船舵起航！";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCFadeOut::actionWithDuration(0),
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  NULL);
		m_pLabel->runAction(action);
		m_pRudderMenuItem->runAction(m_action);
//		CSController::sharedController()->NextRound();
//		m_pRudderMenuItem->selected();	
	}
	else if(m_nSequence == 5)
	{
		m_pRudderMenuItem->setIsEnabled(false);
		m_pRudderMenuItem->stopAllActions();
		
		string str = "等待游戏结束！";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCFadeOut::actionWithDuration(0),
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  NULL);
		
		m_pLabel->runAction(action);
	}
	else if(m_nSequence == 6)
	{
		m_pLabel->setIsVisible(false);
		
		CCSprite *pSprite;
		if (DataManager::sharedDataManager()->GetCurrentUser()->beenz >= DataManager::sharedDataManager()->xmlWingold())
		{
			pSprite = CCSprite::spriteWithSpriteFrameName("missioncomplete.png");
		}
		else
		{
			pSprite = CCSprite::spriteWithSpriteFrameName("missionfailed.png");
		}
		addChild(pSprite, ZORDER_HIGHEST);
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCFadeOut::actionWithDuration(0),
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCMoveBy::actionWithDuration(2, CCPointMake(0, 60)),
														  NULL);
		
		pSprite->runAction(action);
		
		string str;
		USER *pUser = DataManager::sharedDataManager()->GetCurrentUser();
		
		str = "恭喜你，你成功的淘到了货物——";
		vector<USERSTOCK>::iterator iter;
		for ( iter = pUser->stockList.begin(); iter != pUser->stockList.end(); iter++)
		{
			if((*iter).pStock->price > 5)
			{
				//				char tempChar[16];
				//				sprintf(tempChar, "Stock%sArrive", (*iter).pStock->code.c_str());
				//				if (CCUserDefault::sharedUserDefault()->getIntegerForKey(tempChar, 0) == 1)
				//				{
				str += (*iter).pStock->name + " ";
				//				}
			}
			
		}
		
		if (pUser->beenz < DataManager::sharedDataManager()->xmlWingold())
		{
			str = "海洋变幻莫测，船长们请点击卷轴，重新起航！";
		}
		else 
		{
			
			DataManager::sharedDataManager()->setLevel(DataManager::sharedDataManager()->getLevel()+1);
		}

		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action2 = CCSequence::actions(
														   CCFadeOut::actionWithDuration(0),
														   CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														   CCFadeIn::actionWithDuration(1),
														   CCBlink::actionWithDuration(5, 10),
														   NULL);
		
		m_pLabel->runAction(action2);
	}
}

void CSPlayerControlLayer::NextTips2()
{
	m_nSequence++;
	if (m_nSequence == 1)
	{
		m_pRudderMenuItem->setIsEnabled(false);
		m_pImageItemWallet->setIsEnabled(false);
		
		string str = "到港口接货，运气好也能赚点零花钱！";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(2),
														  CCDelayTime::actionWithDuration(15),
														  CCMoveBy::actionWithDuration(3, CCPointMake(0, 60)),
														  CCFadeOut::actionWithDuration(3),
														  CCCallFuncN::actionWithTarget(this, callfuncN_selector(CSPlayerControlLayer::NextTips)), 
														  NULL);
		
		m_pLabel->runAction(action);
	}
	else if(m_nSequence == 2)
	{
		char tempInfo[64];
		sprintf(tempInfo, "游戏目标：金币数达到%d", DataManager::sharedDataManager()->xmlWingold());
		CCLabelTTF *pLabel = CCLabelTTF::labelWithString(tempInfo, "Arial", 25);
		pLabel->setColor(ccRED);
		addChild(pLabel, ZORDER_HIGHEST);
		pLabel->setPosition(ccp(240, 280));
		
		CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName("goldcoin.png");
		pLabel->addChild(pSprite, ZORDER_HIGHEST);
		pSprite->setPosition(ccp(pLabel->getContentSize().width + pSprite->getContentSize().width, pLabel->getContentSize().height/2));
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  CCMoveBy::actionWithDuration(2, CCPointMake(0, 110)),
														  CCCallFuncN::actionWithTarget(this, callfuncN_selector(CSPlayerControlLayer::NextTips)), 
														  NULL);
		
		pLabel->runAction(action);
	}
	
	else if(m_nSequence == 3)
	{
		m_pImageItemWallet->setIsEnabled(true);
		m_pRudderMenuItem->setIsEnabled(true);
		
		string str = "请部署船员！（点击钱袋）";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());	
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  NULL);
		
		m_pLabel->runAction(action);
		m_pImageItemWallet->runAction(m_action);
	}
	else if(m_nSequence == 4)
	{
		m_pImageItemWallet->stopAllActions();
		m_pImageItemWallet->setIsEnabled(false);
		
		string str = "部署完成，启动船舵起航！";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCFadeOut::actionWithDuration(0),
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  NULL);
		m_pLabel->runAction(action);
		m_pRudderMenuItem->runAction(m_action);
	}
	else if(m_nSequence == 5)
	{
		m_pRudderMenuItem->setIsEnabled(false);
		m_pRudderMenuItem->stopAllActions();
		
		string str = "等待游戏结束！";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCFadeOut::actionWithDuration(0),
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  NULL);
		
		m_pLabel->runAction(action);
	}
	else if(m_nSequence == 6)
	{
		m_pLabel->setIsVisible(false);
		
		CCSprite *pSprite;
		if (DataManager::sharedDataManager()->GetCurrentUser()->beenz >= DataManager::sharedDataManager()->xmlWingold())
		{
			pSprite = CCSprite::spriteWithSpriteFrameName("missioncomplete.png");
		}
		else
		{
			pSprite = CCSprite::spriteWithSpriteFrameName("missionfailed.png");
		}
		addChild(pSprite, ZORDER_HIGHEST);
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCFadeOut::actionWithDuration(0),
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCMoveBy::actionWithDuration(2, CCPointMake(0, 60)),
														  NULL);
		
		pSprite->runAction(action);
		
		string str = "点击卷轴进入下一关";
		USER *pUser = DataManager::sharedDataManager()->GetCurrentUser();
		if (pUser->beenz < DataManager::sharedDataManager()->xmlWingold())
		{
			str = "海洋变幻莫测，船长们请点击卷轴，重新起航！";
		}
		else 
		{
			DataManager::sharedDataManager()->setLevel(DataManager::sharedDataManager()->getLevel()+1);
		}

		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action2 = CCSequence::actions(
														   CCFadeOut::actionWithDuration(0),
														   CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														   CCFadeIn::actionWithDuration(1),
														   CCBlink::actionWithDuration(5, 10),
														   NULL);
		
		m_pLabel->runAction(action2);
	}
}

void CSPlayerControlLayer::NextTips3()
{
	m_nSequence++;
	if (m_nSequence == 1)
	{
		m_pRudderMenuItem->setIsEnabled(false);
		m_pImageItemWallet->setIsEnabled(false);
		
		string str = "不是每艘船都能到达港口，但每艘船都可以带来利润。\n不妨来修船厂试试看，多学一门技术有时候还是很重要的呢。";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(2),
														  CCDelayTime::actionWithDuration(1),
														  CCMoveBy::actionWithDuration(3, CCPointMake(0, 60)),
														  CCFadeOut::actionWithDuration(3),
														  CCCallFuncN::actionWithTarget(this, callfuncN_selector(CSPlayerControlLayer::NextTips)), 
														  NULL);
		
		m_pLabel->runAction(action);
	}
	else if(m_nSequence == 2)
	{
		char tempInfo[64];
		sprintf(tempInfo, "游戏目标：金币数达到%d", DataManager::sharedDataManager()->xmlWingold());
		CCLabelTTF *pLabel = CCLabelTTF::labelWithString(tempInfo, "Arial", 25);
		pLabel->setColor(ccRED);
		addChild(pLabel, ZORDER_HIGHEST);
		pLabel->setPosition(ccp(240, 280));
		
		CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName("goldcoin.png");
		pLabel->addChild(pSprite, ZORDER_HIGHEST);
		pSprite->setPosition(ccp(pLabel->getContentSize().width + pSprite->getContentSize().width, pLabel->getContentSize().height/2));
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  CCMoveBy::actionWithDuration(2, CCPointMake(0, 110)),
														  CCCallFuncN::actionWithTarget(this, callfuncN_selector(CSPlayerControlLayer::NextTips)), 
														  NULL);
		
		pLabel->runAction(action);
	}
	
	else if(m_nSequence == 3)
	{
		m_pImageItemWallet->setIsEnabled(true);
		m_pRudderMenuItem->setIsEnabled(true);
		
		string str = "请部署船员！（点击钱袋）";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());	
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  NULL);
		
		m_pLabel->runAction(action);
		m_pImageItemWallet->runAction(m_action);
	}
	else if(m_nSequence == 4)
	{
		m_pImageItemWallet->stopAllActions();
		m_pImageItemWallet->setIsEnabled(false);
		
		string str = "部署完成，启动船舵起航！";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCFadeOut::actionWithDuration(0),
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  NULL);
		m_pLabel->runAction(action);
		m_pRudderMenuItem->runAction(m_action);
	}
	else if(m_nSequence == 5)
	{
		m_pRudderMenuItem->setIsEnabled(false);
		m_pRudderMenuItem->stopAllActions();
		
		string str = "等待游戏结束！";
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCFadeOut::actionWithDuration(0),
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCBlink::actionWithDuration(5, 10),
														  NULL);
		
		m_pLabel->runAction(action);
	}
	else if(m_nSequence == 6)
	{
		m_pLabel->setIsVisible(false);
		
		CCSprite *pSprite;
		if (DataManager::sharedDataManager()->GetCurrentUser()->beenz >= DataManager::sharedDataManager()->xmlWingold())
		{
			pSprite = CCSprite::spriteWithSpriteFrameName("missioncomplete.png");
		}
		else
		{
			pSprite = CCSprite::spriteWithSpriteFrameName("missionfailed.png");
		}
		addChild(pSprite, ZORDER_HIGHEST);
		
		CCFiniteTimeAction*  action = CCSequence::actions(
														  CCFadeOut::actionWithDuration(0),
														  CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														  CCFadeIn::actionWithDuration(1),
														  CCMoveBy::actionWithDuration(2, CCPointMake(0, 60)),
														  NULL);
		
		pSprite->runAction(action);
		
		string str = "点击卷轴进入下一关";
		USER *pUser = DataManager::sharedDataManager()->GetCurrentUser();
		
		if (pUser->beenz < DataManager::sharedDataManager()->xmlWingold())
		{
			str = "海洋变幻莫测，船长们请点击卷轴，重新起航！";
		}
		else 
		{
			DataManager::sharedDataManager()->setLevel(DataManager::sharedDataManager()->getLevel()+1);
		}
		
		
		DataManager::GBKToUTF8(str);
		m_pLabel->setString(str.c_str());
		
		CCFiniteTimeAction*  action2 = CCSequence::actions(
														   CCFadeOut::actionWithDuration(0),
														   CCMoveTo::actionWithDuration(0, CCPointMake(240, 160)),
														   CCFadeIn::actionWithDuration(1),
														   CCBlink::actionWithDuration(5, 10),
														   NULL);
		
		m_pLabel->runAction(action2);
	}
}

void CSPlayerControlLayer::NextTips4()
{
	
}

void CSPlayerControlLayer::NextTips5()
{
	
}

void CSPlayerControlLayer::NextTips6()
{

}

void CSPlayerControlLayer::NextTips7()
{
	
}

//void CSPlayerControlLayer::callbackSeaCraft(CCObject* sender)
//{
//	CSSeacraftMenuItem *pCSSeacraftMenuItem = (CSSeacraftMenuItem *)sender;
//	CSSeaCraftPopupMsg *pSeaCraft = new CSSeaCraftPopupMsg(1, pCSSeacraftMenuItem->getId(), this, callfuncND_selector(CSPlayerControlLayer::callbackBuySeaCraft), pCSSeacraftMenuItem);
//	pSeaCraft->init();
//	addChild(pSeaCraft, ZORDER_HIGHEST);
//	pSeaCraft->autorelease();
//}

//void CSPlayerControlLayer::callbackBuySeaCraft(CCNode* sender, void *data)
//{
//	CSSeaCraftPopupMsg *pPopupMsg = (CSSeaCraftPopupMsg *)sender;
//	if (!pPopupMsg || pPopupMsg->GetResponse() != RESPONSE_YES)
//	{
//		return;
//	}
//
//	USER *pUser = &DataManager::sharedDataManager()->GetCurrentUser();
//
//	CSSeacraftMenuItem *pCSSeacraftMenuItem = (CSSeacraftMenuItem *)data;
//	CSController::sharedController()->BoardSeaCraft(pUser, pCSSeacraftMenuItem->getId());
//	pCSSeacraftMenuItem->UpdateInfo();
//	m_pPlayer->UpdateInfo(pUser);
//
//	m_nSequence = 3;
//	NextTips();
//}