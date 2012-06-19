#include "CSPlayerInfoSprite.h"
#include "../GAME/CSController.h"


CSPlayerInfoSprite::CSPlayerInfoSprite(void)
{
}

CSPlayerInfoSprite::~CSPlayerInfoSprite(void)
{
}

CSPlayerInfoSprite* CSPlayerInfoSprite::spriteWithPlayer(USER *pUser)
{
	CSPlayerInfoSprite * pobSprite = new CSPlayerInfoSprite();
	pobSprite->init(pUser);
	pobSprite->autorelease();
	
	pobSprite->UpdateInfo(pUser);
	
	return pobSprite;
}

bool CSPlayerInfoSprite::init(USER *pUser)
{
	m_pName = CCLabelTTF::labelWithString("GoldRush", "Arial", 18);
	addChild(m_pName);
	m_pName->setColor(ccORANGE);
	m_pName->setPosition(ccp(0, 60));
	
	m_pIcon =  CCSprite::spriteWithSpriteFrameName("player.png");
	addChild(m_pIcon);
	m_pIcon->setPosition(ccp(0, 15));
	
	m_pCoin =  CCSprite::spriteWithSpriteFrameName("goldcoin.png");
	addChild(m_pCoin);
	m_pCoin->setPosition(ccp(-15, -10));
	
	m_pCoinLable = CCLabelTTF::labelWithString("100", "Arial", 18);
	m_pCoin->addChild(m_pCoinLable);
	m_pCoinLable->setColor(ccORANGE);
	m_pCoinLable->setPosition(ccp(30, 5));
	
	m_pEmployee =  CCSprite::spriteWithSpriteFrameName(pUser->pEmployee->employeeFile.c_str());
	addChild(m_pEmployee);
	m_pEmployee->setPosition(ccp(30, -15));
	m_pEmployee->setFlipX(true);
	
	m_pEmployeeLable = CCLabelTTF::labelWithString(ConvertToString(pUser->pEmployee->count).c_str(), "Arial", 18);
	m_pEmployee->addChild(m_pEmployeeLable);
	m_pEmployeeLable->setColor(ccBLACK);
	m_pEmployeeLable->setPosition(ccp(50, 30));
	
	return true;
}

void CSPlayerInfoSprite::UpdateInfo(USER *pUser)
{
	assert(pUser!=NULL);
	
	DataManager::GBKToUTF8(pUser->nickName);
	
	m_pName->setString(pUser->nickName.c_str());
	
	char temp[8];
	sprintf(temp, "%d", pUser->beenz);
	m_pCoinLable->setString(temp);
	
	sprintf(temp, "%d", pUser->pEmployee->count);
	m_pEmployeeLable->setString(temp);
	
	//m_pEmployee->setTexture(CCTextureCache::sharedTextureCache()->addImage(pUser->pEmployee->employeeFile.c_str()));
}