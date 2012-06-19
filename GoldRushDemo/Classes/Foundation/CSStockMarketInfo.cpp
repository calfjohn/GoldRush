#include "CSStockMarketInfo.h"
#include "../GAME/CSController.h"

#define kTagStock	10000
#define kTagPrice	10001
CSStockMarketInfo* CSStockMarketInfo::node()
{
	CSStockMarketInfo * pobSprite = new CSStockMarketInfo();
	pobSprite->init();
	pobSprite->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - pobSprite->getContentSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
	pobSprite->autorelease();

	pobSprite->UpdateInfo();

	return pobSprite;
}

bool CSStockMarketInfo::init()
{
	CCLabelTTF *pLabel = NULL;
	CCSprite *pSprite = NULL;
	
	int i = 0;
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPoint posTemp = (ccp(size.width - 30, size.height/2 + 60));
	
	map<int, STOCK>::iterator iterTemp;
	for(iterTemp = DataManager::sharedDataManager()->m_mapStockMarket.begin(); iterTemp !=  DataManager::sharedDataManager()->m_mapStockMarket.end(); ++iterTemp, i++)
	{
		pSprite = CCSprite::spriteWithSpriteFrameName((*iterTemp).second.stockFile.c_str());
		pSprite->setAnchorPoint(CCPointZero);
		addChild(pSprite, 0, kTagStock);
		pSprite->setPosition(ccp(posTemp.x, posTemp.y - i*40));

		pLabel = CCLabelTTF::labelWithString(ConvertToString((*iterTemp).second.price).c_str(), "Arial", 18);
		pLabel->setColor(ccORANGE);
		pSprite->addChild(pLabel, 0, kTagPrice);
	}
	/*
	m_pStock1 = CCSprite::spriteWithSpriteFrameName("丝绸.png");
	
	m_pStock1->setAnchorPoint(CCPointZero);
	addChild(m_pStock1);
	m_pLabel1 = CCLabelTTF::labelWithString("5", "Arial", 18);
	m_pLabel1->setColor(ccORANGE);
	m_pStock1->addChild(m_pLabel1);
	m_pLabel1->setPosition(ccp(m_pStock1->getContentSize().width-10, m_pStock1->getContentSize().height/3));
	
	m_pStock2 = CCSprite::spriteWithSpriteFrameName("人参.png");
	m_pStock2->setAnchorPoint(CCPointZero);
	addChild(m_pStock2);
	m_pLabel2 = CCLabelTTF::labelWithString("5", "Arial", 18);
	m_pLabel2->setColor(ccORANGE);
	m_pStock2->addChild(m_pLabel2);
	m_pLabel2->setPosition(ccp(m_pStock2->getContentSize().width-10, m_pStock1->getContentSize().height/3));
	
	m_pStock3 = CCSprite::spriteWithSpriteFrameName("绿宝石.png");
	m_pStock3->setAnchorPoint(CCPointZero);
	addChild(m_pStock3);
	m_pLabel3 = CCLabelTTF::labelWithString("5", "Arial", 18);
	m_pLabel3->setColor(ccORANGE);
	m_pStock3->addChild(m_pLabel3);
	m_pLabel3->setPosition(ccp(m_pStock3->getContentSize().width-10, m_pStock1->getContentSize().height/3));
	
	m_pStock4 = CCSprite::spriteWithSpriteFrameName("肉豆扣.png");
	m_pStock4->setAnchorPoint(CCPointZero);
	addChild(m_pStock4);
	m_pLabel4 = CCLabelTTF::labelWithString("5", "Arial", 18);
	m_pLabel4->setColor(ccORANGE);
	m_pStock4->addChild(m_pLabel4);
	m_pLabel4->setPosition(ccp(m_pStock4->getContentSize().width-10, m_pStock1->getContentSize().height/3));
	 */
	return true;
}

void CSStockMarketInfo::UpdateInfo()
{
	
}