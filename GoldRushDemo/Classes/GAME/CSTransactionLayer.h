#ifndef _CSTRANSACTIONLAYER_H_
#define _CSTRANSACTIONLAYER_H_

#include "../Foundation/CSLayer.h"

//游戏层，包含地图、船
class CSTransactionLayer:public CSLayer
{
public:
	CSTransactionLayer();
	virtual ~CSTransactionLayer(){}
	LAYER_NODE_FUNC(CSTransactionLayer);	

	bool init();

	void UpdateLayer();

protected:
	void transaction(CCObject* sender);
	
	void decideGameSpot(CCObject* sender);
	
	void callbackNextRound(CCObject* sender);

	void initTitleAndCell();

	void initCellContent();

	CCMenu *m_pMenu;
};


#endif