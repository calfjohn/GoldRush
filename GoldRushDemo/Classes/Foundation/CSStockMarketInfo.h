#ifndef _CSSTOCKMARKETINFO_H_
#define _CSSTOCKMARKETINFO_H_

#include "../Game/DataManager.h"

class CSStockMarketInfo :public CCNode
{
public:
	CSStockMarketInfo(void){};
	~CSStockMarketInfo(void){};
	 static CSStockMarketInfo* node();
	 bool init();

	 void UpdateInfo();

protected:
	CCSprite *m_pStock1;
	CCSprite *m_pStock2;
	CCSprite *m_pStock3;
	CCSprite *m_pStock4;

	CCLabelTTF *m_pLabel1;
	CCLabelTTF *m_pLabel2;
	CCLabelTTF *m_pLabel3;
	CCLabelTTF *m_pLabel4;
};
#endif