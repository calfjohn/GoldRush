#ifndef _CSPLAYERCONTROLLAYER_H_
#define _CSPLAYERCONTROLLAYER_H_

#include "DataManager.h"
#include "../Foundation/CSMenuItem.h"
#include "../Foundation/CSPlayerInfoSprite.h"
#include "../Foundation/CSRulerInfo.h"
#include "../Foundation/CSSeacraftMenuItem.h"
#include "../Foundation/CSStockMarketInfo.h"
#include "../Foundation/CSRudderMenuItem.h"
//操作面板层
//包含 货船进度 CSRulerMenu 玩家信息-头像、钱、同谋、资产 CSPlayerMenu 股市 CSStockMarketMenu  聊天框 待定，单机版不做

class CSPlayerControlLayer:public CCLayer
{
public:
	CSPlayerControlLayer(){}
	virtual ~CSPlayerControlLayer(){CC_SAFE_RELEASE(m_action)}
	LAYER_NODE_FUNC(CSPlayerControlLayer);
	bool init();
 	void UpdateLayer();
	
	void NextTips(CCNode* sender);
protected:
	void transaction(CCObject* sender);
	void restartLevel(CCObject* sender);

//	void callbackSeaCraft(CCObject* sender);
//	void callbackBuySeaCraft(CCNode* sender, void *data);
	void callbackNextRound(CCObject* sender);

	CSRulerInfo *m_pRuleInfo;
	CSPlayerInfoSprite *m_pPlayer;
	CSStockMarketInfo *m_pStockMarketInfo;
	CSRudderMenuItem *m_pRudderMenuItem;
	CSMenuItem *m_pImageItemWallet;
	CSMenuItem *m_pImageItemHelp;
	CCSprite *m_pSandglass;
	
	int m_nSequence;

	CCLabelTTF *m_pLabel;
	CCActionInterval *m_action;
protected:
	//关卡相关
	void NextTips1();
	void NextTips2();
	void NextTips3();
	void NextTips4();
	void NextTips5();
	void NextTips6();
	void NextTips7();
};


#endif