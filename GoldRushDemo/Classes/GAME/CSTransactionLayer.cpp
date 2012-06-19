
#include "CSController.h"
#include "CSTransactionLayer.h"
#include "../Foundation/CSGameSpotMenuItem.h"
#include "../Foundation/CSRudderMenuItem.h"

CSTransactionLayer::CSTransactionLayer()
{
	m_nCFPriority = kCCMenuTouchPriority;
	setIsTouchEnabled(true);
}

bool CSTransactionLayer::init()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCSprite *pBackGround = CCSprite::spriteWithSpriteFrameName("background.png");
	pBackGround->setAnchorPoint(CCPointZero);
	addChild(pBackGround, ZORDER_DEFAULT);
	
	CCSprite *pSandglass = CCSprite::spriteWithSpriteFrameName("sandglass.png");
	addChild(pSandglass, ZORDER_HIGH);
	pSandglass->setPosition(ccp(size.width - 20, size.height - 20));
		
	m_pMenu = CCMenu::menuWithItem(NULL);
	addChild(m_pMenu, ZORDER_DEFAULT);
	m_pMenu->setPosition(CCPointZero);
	
	CSMenuItem *pImageItemWallet = CSMenuItem::itemFromNormalImage("wallet.png", "wallet.png", this, menu_selector(CSTransactionLayer::transaction));
	m_pMenu->addChild(pImageItemWallet, ZORDER_HIGH);
	pImageItemWallet->setPosition(ccp(size.width - 20, 40));
	
	//启航按钮
	CSRudderMenuItem * pRudderMenuItem = CSRudderMenuItem::itemFromSprite(this, menu_selector(CSTransactionLayer::callbackNextRound));
	m_pMenu->addChild(pRudderMenuItem, ZORDER_HIGH);
	pRudderMenuItem->setPosition(ccp(pRudderMenuItem->getContentSize().width/2, pRudderMenuItem->getContentSize().height/2 + 20));
	
	initTitleAndCell();
	
	initCellContent();
	
	return true;
}

void CSTransactionLayer::UpdateLayer()
{
	
}

void CSTransactionLayer::transaction(CCObject* sender)
{
	removeFromParentAndCleanup(true);
	CSController::sharedController()->UpdatePlayerInfo();
}

void CSTransactionLayer::decideGameSpot(CCObject* sender)
{
	CSGameSpotMenuItem *pItem = (CSGameSpotMenuItem *)sender;

	if(CSController::sharedController()->DecideGamespot(DataManager::sharedDataManager()->GetCurrentUser()->uid, pItem->getId()))
	{
		pItem->UpdateInfo();
	}
}

void CSTransactionLayer::callbackNextRound(CCObject* sender)
{
	CSController::sharedController()->NextRound();
	CSController::sharedController()->NextTips();
	transaction(NULL);
}

void CSTransactionLayer::initCellContent()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	
	CCLabelTTF *pLabel = NULL;
	CSGameSpotMenuItem *pGamespotMenuItem = NULL;
	CCPoint tempPoint = CCPointZero;
	
	//股市
	pLabel = CCLabelTTF::labelWithString("名称", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	tempPoint = ccp(pLabel->getContentSize().width/2 + 43, size.height - pLabel->getContentSize().height/2 - 33);
	pLabel->setPosition(tempPoint);
	
	pLabel = CCLabelTTF::labelWithString("价格", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height - 3));

	pLabel = CCLabelTTF::labelWithString("数量", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height - 3));
	
	map<int, STOCK>::iterator iterTemp; 
	for (iterTemp = DataManager::sharedDataManager()->m_mapStockMarket.begin(); iterTemp != DataManager::sharedDataManager()->m_mapStockMarket.end(); iterTemp++)
	{
		pLabel = CCLabelTTF::labelWithString((*iterTemp).second.name.c_str(), "Arial", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);	
		tempPoint.x += pLabel->getContentSize().width + 8;
		pLabel->setPosition(tempPoint);
		
		pLabel = CCLabelTTF::labelWithString(ConvertToString((*iterTemp).second.price).c_str(), "Arial Black", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);	
		pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height));
		
		pLabel = CCLabelTTF::labelWithString(ConvertToString((*iterTemp).second.count).c_str(), "Arial Black", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);	
		pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height));
	}
	//道具
	
	//保险
	pLabel = CCLabelTTF::labelWithString("保险", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	tempPoint = ccp(pLabel->getContentSize().width/2 + 355, size.height - pLabel->getContentSize().height/2 - 33);
	pLabel->setPosition(tempPoint);
	
	pLabel = CCLabelTTF::labelWithString("价格", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height - 3));
	
	pLabel = CCLabelTTF::labelWithString("占有", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height - 3));
	
	pLabel = CCLabelTTF::labelWithString(ConvertToString(DataManager::sharedDataManager()->m_insurance.buy).c_str(), "Arial Black", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x + pLabel->getContentSize().width + 18, tempPoint.y - pLabel->getContentSize().height));
	
	pGamespotMenuItem = CSGameSpotMenuItem::itemFromSprite(DataManager::sharedDataManager()->m_insurance.index, 
														   DataManager::sharedDataManager()->xmlInsuranceVisible(), 
														   DataManager::sharedDataManager()->xmlInsuranceEnable(),
														   this, menu_selector(CSTransactionLayer::decideGameSpot));
	
	m_pMenu->addChild(pGamespotMenuItem, ZORDER_HIGH);
	pGamespotMenuItem->setPosition(ccp(tempPoint.x + pGamespotMenuItem->getContentSize().width + 15, tempPoint.y - 2*pLabel->getContentSize().height));

	//1号船
	pLabel = CCLabelTTF::labelWithString("货舱", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	tempPoint = ccp(pLabel->getContentSize().width/2 + 43, size.height - pLabel->getContentSize().height/2 - 108);
	pLabel->setPosition(tempPoint);
	
	pLabel = CCLabelTTF::labelWithString(DataManager::sharedDataManager()->m_mapSeacraft[1].pCargo->pStock->name.c_str(), "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height - 3));
		
	pLabel = CCLabelTTF::labelWithString("占有", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height - 3));
	
	int i=1;
	vector<GAMESPOT>::iterator iterGameSpot; 
	CARGO *pCargo = DataManager::sharedDataManager()->m_mapSeacraft[1].pCargo;
	for (iterGameSpot = pCargo->spotList.begin(); iterGameSpot !=  pCargo->spotList.end(); iterGameSpot++, i++)
	{
		char tempChar[16];
		sprintf(tempChar, "位置%d", i);
		pLabel = CCLabelTTF::labelWithString(tempChar, "Arial", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);
		tempPoint.x += pLabel->getContentSize().width + 4;
		pLabel->setPosition(tempPoint);
		
		pLabel = CCLabelTTF::labelWithString(ConvertToString((*iterGameSpot).buy).c_str(), "Arial Black", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);	
		pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height));
		
		//占有位置
		pGamespotMenuItem = CSGameSpotMenuItem::itemFromSprite((*iterGameSpot).index, 
															   DataManager::sharedDataManager()->xmlCargoVisible(pCargo->pStock->code, i), 
															   DataManager::sharedDataManager()->xmlCargoEnable(pCargo->pStock->code, i),
															   this, menu_selector(CSTransactionLayer::decideGameSpot));
		m_pMenu->addChild(pGamespotMenuItem, ZORDER_HIGH);
		pGamespotMenuItem->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height));
	}
	
	//2号船
	pLabel = CCLabelTTF::labelWithString("货舱", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	tempPoint = ccp(pLabel->getContentSize().width/2 + 43, size.height - pLabel->getContentSize().height/2 - 183);
	pLabel->setPosition(tempPoint);
	
	pLabel = CCLabelTTF::labelWithString(DataManager::sharedDataManager()->m_mapSeacraft[2].pCargo->pStock->name.c_str(), "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height - 3));
	
	pLabel = CCLabelTTF::labelWithString("占有", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height - 3));
	
	i=1;
	pCargo = DataManager::sharedDataManager()->m_mapSeacraft[2].pCargo;
	for (iterGameSpot = pCargo->spotList.begin(); iterGameSpot !=  pCargo->spotList.end(); iterGameSpot++, i++)
	{
		char tempChar[16];
		sprintf(tempChar, "位置%d", i);
		pLabel = CCLabelTTF::labelWithString(tempChar, "Arial", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);
		tempPoint.x += pLabel->getContentSize().width + 10;
		pLabel->setPosition(tempPoint);
		
		pLabel = CCLabelTTF::labelWithString(ConvertToString((*iterGameSpot).buy).c_str(), "Arial Black", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);	
		pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height));
		
		//占有位置
		pGamespotMenuItem = CSGameSpotMenuItem::itemFromSprite((*iterGameSpot).index, 
															   DataManager::sharedDataManager()->xmlCargoVisible(pCargo->pStock->code, i), 
															   DataManager::sharedDataManager()->xmlCargoEnable(pCargo->pStock->code, i),
															   this, menu_selector(CSTransactionLayer::decideGameSpot));
		m_pMenu->addChild(pGamespotMenuItem, ZORDER_HIGH);
		pGamespotMenuItem->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height));
	}
	
	//3号船
	pLabel = CCLabelTTF::labelWithString("货舱", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	tempPoint = ccp(pLabel->getContentSize().width/2 + 43, size.height - pLabel->getContentSize().height/2 - 258);
	pLabel->setPosition(tempPoint);
	
	pLabel = CCLabelTTF::labelWithString(DataManager::sharedDataManager()->m_mapSeacraft[3].pCargo->pStock->name.c_str(), "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height - 3));
	
	pLabel = CCLabelTTF::labelWithString("占有", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height - 3));
	
	i=1;
	pCargo = DataManager::sharedDataManager()->m_mapSeacraft[3].pCargo;
	for (iterGameSpot = pCargo->spotList.begin(); iterGameSpot !=  pCargo->spotList.end(); iterGameSpot++, i++)
	{
		char tempChar[16];
		sprintf(tempChar, "位置%d", i);
		pLabel = CCLabelTTF::labelWithString(tempChar, "Arial", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);
		tempPoint.x += pLabel->getContentSize().width + 10;
		pLabel->setPosition(tempPoint);
		
		pLabel = CCLabelTTF::labelWithString(ConvertToString((*iterGameSpot).buy).c_str(), "Arial Black", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);	
		pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height));
		
		//占有位置
		pGamespotMenuItem = CSGameSpotMenuItem::itemFromSprite((*iterGameSpot).index, 
															   DataManager::sharedDataManager()->xmlCargoVisible(pCargo->pStock->code, i), 
															   DataManager::sharedDataManager()->xmlCargoEnable(pCargo->pStock->code, i),
															   this, menu_selector(CSTransactionLayer::decideGameSpot));
		m_pMenu->addChild(pGamespotMenuItem, ZORDER_HIGH);
		pGamespotMenuItem->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height));
	}
	
	//港口
	tempPoint = ccp(pLabel->getContentSize().width/2 + 240, size.height - pLabel->getContentSize().height/2 - 106);
	pLabel = CCLabelTTF::labelWithString("价格", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height - 3));
	
	pLabel = CCLabelTTF::labelWithString("占有", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height - 3));
	
	i=1;
	map<int, PORTS>::iterator iterPorts; 
	for (iterPorts = DataManager::sharedDataManager()->m_mapPorts.begin(); iterPorts != DataManager::sharedDataManager()->m_mapPorts.end(); iterPorts++, i++)
	{
		char tempChar[16];
		sprintf(tempChar, "港口%d", i);
		pLabel = CCLabelTTF::labelWithString(tempChar, "Arial", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);	
		tempPoint.x += pLabel->getContentSize().width + 8;
		pLabel->setPosition(tempPoint);
		
		pLabel = CCLabelTTF::labelWithString(ConvertToString((*iterPorts).second.spot.buy).c_str(), "Arial Black", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);	
		pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height));
		
		//占有位置
		pGamespotMenuItem = CSGameSpotMenuItem::itemFromSprite((*iterPorts).second.spot.index, 
															   DataManager::sharedDataManager()->xmlPortsVisible((*iterPorts).first), 
															   DataManager::sharedDataManager()->xmlPortsEnable((*iterPorts).first),
															   this, menu_selector(CSTransactionLayer::decideGameSpot));
		m_pMenu->addChild(pGamespotMenuItem, ZORDER_HIGH);
		pGamespotMenuItem->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height));
	}	
	
	//修船厂
	tempPoint = ccp(pLabel->getContentSize().width/2 + 240, size.height - pLabel->getContentSize().height/2 - 183);
	pLabel = CCLabelTTF::labelWithString("价格", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height - 3));
	
	pLabel = CCLabelTTF::labelWithString("占有", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height - 3));
	
	i=1;
	map<int, SHIPYARD>::iterator iterShipyard; 
	for (iterShipyard = DataManager::sharedDataManager()->m_mapShipyard.begin(); iterShipyard != DataManager::sharedDataManager()->m_mapShipyard.end(); iterShipyard++, i++)
	{
		char tempChar[16];
		sprintf(tempChar, "船厂%d", i);
		pLabel = CCLabelTTF::labelWithString(tempChar, "Arial", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);	
		tempPoint.x += pLabel->getContentSize().width + 8;
		pLabel->setPosition(tempPoint);
		
		pLabel = CCLabelTTF::labelWithString(ConvertToString((*iterShipyard).second.spot.buy).c_str(), "Arial Black", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);	
		pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height));
		
		//占有位置
		pGamespotMenuItem = CSGameSpotMenuItem::itemFromSprite((*iterShipyard).second.spot.index, 
															   DataManager::sharedDataManager()->xmlShipyardVisible((*iterShipyard).first), 
															   DataManager::sharedDataManager()->xmlShipyardEnable((*iterShipyard).first),
															   this, menu_selector(CSTransactionLayer::decideGameSpot));
		m_pMenu->addChild(pGamespotMenuItem, ZORDER_HIGH);
		pGamespotMenuItem->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height));
	}	
	
	//海盗/领航员
	tempPoint = ccp(pLabel->getContentSize().width/2 + 240, size.height - pLabel->getContentSize().height/2 - 258);
	
	pLabel = CCLabelTTF::labelWithString("价格", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height - 3));
	
	pLabel = CCLabelTTF::labelWithString("占有", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x, tempPoint.y - 2*pLabel->getContentSize().height - 3));
	
	pLabel = CCLabelTTF::labelWithString("海盗头目", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);
	pLabel->setPosition(ccp(tempPoint.x + 48, tempPoint.y));
	
	pLabel = CCLabelTTF::labelWithString("海盗", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x + 92, tempPoint.y));
	
	pLabel = CCLabelTTF::labelWithString("大领航员", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x + 135, tempPoint.y));

	pLabel = CCLabelTTF::labelWithString("领航员", "Arial", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(tempPoint.x + 182, tempPoint.y));
	
	tempPoint = ccp(pLabel->getContentSize().width + 240, size.height - 2*pLabel->getContentSize().height - 253);
	map<int, GAMESPOT>::iterator iterPirate;
	for (iterPirate = DataManager::sharedDataManager()->m_mapPirate.begin(); iterPirate != DataManager::sharedDataManager()->m_mapPirate.end(); iterPirate++)
	{
		pLabel = CCLabelTTF::labelWithString(ConvertToString((*iterPirate).second.buy).c_str(), "Arial Black", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);
		pLabel->setPosition(tempPoint);
		
		pGamespotMenuItem = CSGameSpotMenuItem::itemFromSprite((*iterPirate).second.index, 
															   DataManager::sharedDataManager()->xmlPirateVisible((*iterPirate).first), 
															   DataManager::sharedDataManager()->xmlPirateEnable((*iterPirate).first),
															   this, menu_selector(CSTransactionLayer::decideGameSpot));
		m_pMenu->addChild(pGamespotMenuItem, ZORDER_HIGH);
		pGamespotMenuItem->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height));

		tempPoint.x += 45;
	}
	
	map<int, NAVIGATOR>::iterator iterNav;
	for (iterNav = DataManager::sharedDataManager()->m_mapNavigator.begin(); iterNav != DataManager::sharedDataManager()->m_mapNavigator.end(); iterNav++)
	{
		pLabel = CCLabelTTF::labelWithString(ConvertToString((*iterNav).second.spot.buy).c_str(), "Arial Black", 13);
		pLabel->setColor(ccBLACK);
		addChild(pLabel, ZORDER_NORMAL);
		pLabel->setPosition(tempPoint);
		
		pGamespotMenuItem = CSGameSpotMenuItem::itemFromSprite((*iterNav).second.spot.index, 
															   DataManager::sharedDataManager()->xmlNavigatorVisible((*iterNav).first), 
															   DataManager::sharedDataManager()->xmlNavigatorEnable((*iterNav).first),
															   this, menu_selector(CSTransactionLayer::decideGameSpot));
		m_pMenu->addChild(pGamespotMenuItem, ZORDER_HIGH);
		pGamespotMenuItem->setPosition(ccp(tempPoint.x, tempPoint.y - pLabel->getContentSize().height));
		
		tempPoint.x += 45;
	}
}


void CSTransactionLayer::initTitleAndCell()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	
	CCLabelTTF *pLabel = NULL;
	CCSprite *pCell = NULL;
	CCPoint tempPoint = CCPointZero;
	
	pLabel = CCLabelTTF::labelWithString("股市", "黑体", 16);
	pLabel->setColor(ccORANGE);
	addChild(pLabel, ZORDER_NORMAL);
	tempPoint = ccp(pLabel->getContentSize().width/2 + 40, size.height - pLabel->getContentSize().height/2 - 10);
	pLabel->setPosition(tempPoint);
	
	pCell = CCSprite::spriteWithSpriteFrameName("cell1.png");
	addChild(pCell, ZORDER_NORMAL);
	pCell->setPosition(ccp(pCell->getContentSize().width/2 + 40, tempPoint.y - pLabel->getContentSize().height/2 - pCell->getContentSize().height/2));
	
	pLabel = CCLabelTTF::labelWithString("道具", "黑体", 16);
	pLabel->setColor(ccORANGE);
	addChild(pLabel, ZORDER_NORMAL);
	tempPoint.x += 190;
	pLabel->setPosition(tempPoint);
	
    pCell = CCSprite::spriteWithSpriteFrameName("cell2.png");
    addChild(pCell, ZORDER_NORMAL);
	pCell->setPosition(ccp(pCell->getContentSize().width/2 + 230, tempPoint.y - pLabel->getContentSize().height/2 - pCell->getContentSize().height/2));
	
	pLabel = CCLabelTTF::labelWithString("保险", "黑体", 16);
	pLabel->setColor(ccORANGE);
	addChild(pLabel, ZORDER_NORMAL);
	tempPoint.x += 120;
	pLabel->setPosition(tempPoint);
	
	pCell = CCSprite::spriteWithSpriteFrameName("cell2.png");
	addChild(pCell, ZORDER_NORMAL);
	pCell->setPosition(ccp(pCell->getContentSize().width/2 + 352, tempPoint.y - pLabel->getContentSize().height/2 - pCell->getContentSize().height/2));	
	
	pLabel = CCLabelTTF::labelWithString("股市", "黑体", 16);
	pLabel->setColor(ccORANGE);
	addChild(pLabel, ZORDER_NORMAL);
	tempPoint = ccp(pLabel->getContentSize().width/2 + 40, size.height - pLabel->getContentSize().height/2 - 10);
	pLabel->setPosition(tempPoint);
	
	pCell = CCSprite::spriteWithSpriteFrameName("cell1.png");
	addChild(pCell, ZORDER_NORMAL);
	pCell->setPosition(ccp(pCell->getContentSize().width/2 + 40, tempPoint.y - pLabel->getContentSize().height/2 - pCell->getContentSize().height/2));
	
	
	pLabel = CCLabelTTF::labelWithString(DataManager::sharedDataManager()->m_mapSeacraft[1].pCargo->pStock->seaCraftName.c_str(), "黑体", 16);
	pLabel->setColor(ccORANGE);
	addChild(pLabel, ZORDER_NORMAL);
	tempPoint = ccp(pLabel->getContentSize().width/2 + 40, size.height - pLabel->getContentSize().height/2 - 85);
	pLabel->setPosition(tempPoint);
	
	pLabel = CCLabelTTF::labelWithString(ConvertToString(DataManager::sharedDataManager()->m_mapSeacraft[1].pCargo->pStock->value).c_str(), "Arial Black", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2 + 200, size.height - pLabel->getContentSize().height/2 - 85));
	
    pCell = CCSprite::spriteWithSpriteFrameName("cell1.png");
    addChild(pCell, ZORDER_NORMAL);
	pCell->setPosition(ccp(pCell->getContentSize().width/2 + 40, tempPoint.y - pLabel->getContentSize().height/2 - pCell->getContentSize().height/2));
	
	pLabel = CCLabelTTF::labelWithString("港口", "黑体", 16);
	pLabel->setColor(ccORANGE);
	addChild(pLabel, ZORDER_NORMAL);
	tempPoint.x += 175;
	pLabel->setPosition(tempPoint);
	
	pCell = CCSprite::spriteWithSpriteFrameName("cell1.png");
    addChild(pCell, ZORDER_NORMAL);
	pCell->setPosition(ccp(pCell->getContentSize().width/2 + 230, tempPoint.y - pLabel->getContentSize().height/2 - pCell->getContentSize().height/2));
	
	pLabel = CCLabelTTF::labelWithString(DataManager::sharedDataManager()->m_mapSeacraft[2].pCargo->pStock->seaCraftName.c_str(), "黑体", 16);
	pLabel->setColor(ccORANGE);
	addChild(pLabel, ZORDER_NORMAL);
	tempPoint = ccp(pLabel->getContentSize().width/2 + 40, size.height - pLabel->getContentSize().height/2 - 160);
	pLabel->setPosition(tempPoint);
	
	pLabel = CCLabelTTF::labelWithString(ConvertToString(DataManager::sharedDataManager()->m_mapSeacraft[2].pCargo->pStock->value).c_str(), "Arial Black", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2 + 200, size.height - pLabel->getContentSize().height/2 - 160));
	
    pCell = CCSprite::spriteWithSpriteFrameName("cell1.png");
    addChild(pCell, ZORDER_NORMAL);
	pCell->setPosition(ccp(pCell->getContentSize().width/2 + 40, tempPoint.y - pLabel->getContentSize().height/2 - pCell->getContentSize().height/2));
	
	pLabel = CCLabelTTF::labelWithString("修船厂", "黑体", 16);
	pLabel->setColor(ccORANGE);
	addChild(pLabel, ZORDER_NORMAL);
	tempPoint.x += 190;
	pLabel->setPosition(tempPoint);
	
	pCell = CCSprite::spriteWithSpriteFrameName("cell1.png");
    addChild(pCell, ZORDER_NORMAL);
	pCell->setPosition(ccp(pCell->getContentSize().width/2 + 230, tempPoint.y - pLabel->getContentSize().height/2 - pCell->getContentSize().height/2));
	
	
	pLabel = CCLabelTTF::labelWithString(DataManager::sharedDataManager()->m_mapSeacraft[3].pCargo->pStock->seaCraftName.c_str(), "黑体", 16);
	pLabel->setColor(ccORANGE);
	addChild(pLabel, ZORDER_NORMAL);
	tempPoint = ccp(pLabel->getContentSize().width/2 + 40, size.height - pLabel->getContentSize().height/2 - 235);
	pLabel->setPosition(tempPoint);
	
	pLabel = CCLabelTTF::labelWithString(ConvertToString(DataManager::sharedDataManager()->m_mapSeacraft[3].pCargo->pStock->value).c_str(), "Arial Black", 13);
	pLabel->setColor(ccBLACK);
	addChild(pLabel, ZORDER_NORMAL);	
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2 + 200, size.height - pLabel->getContentSize().height/2 - 235));
	
    pCell = CCSprite::spriteWithSpriteFrameName("cell1.png");
    addChild(pCell, ZORDER_NORMAL);
	pCell->setPosition(ccp(pCell->getContentSize().width/2 + 40, tempPoint.y - pLabel->getContentSize().height/2 - pCell->getContentSize().height/2));
	
	pLabel = CCLabelTTF::labelWithString("海盗/领航员", "黑体", 16);
	pLabel->setColor(ccORANGE);
	addChild(pLabel, ZORDER_NORMAL);
	tempPoint.x += 210;
	pLabel->setPosition(tempPoint);
	
	pCell = CCSprite::spriteWithSpriteFrameName("cell3.png");
    addChild(pCell, ZORDER_NORMAL);
	pCell->setPosition(ccp(pCell->getContentSize().width/2 + 230, tempPoint.y - pLabel->getContentSize().height/2 - pCell->getContentSize().height/2));	
}