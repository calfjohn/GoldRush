#ifndef _CSGAMELAYER_H_
#define _CSGAMELAYER_H_

#include "DataManager.h"

//游戏层，包含地图、船
class CSGameLayer:public CCLayer
{
public:
	CSGameLayer();
	virtual ~CSGameLayer();
	LAYER_NODE_FUNC(CSGameLayer);	
	bool containsTouchLocation(CCSprite* pSpirte, CCTouch* touch);

	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	bool init();

	void UpdateLayer();

protected:
	void VisionFollow(ccTime dt);// 地图视角跟随船移动
	void adjustlimit();
	void adjustPosition(CCPoint &newPosition);
	void callbackSail();

	CGFloat	m_preDistance;
	CCPoint m_limitPos;
	CCSize m_mapSize;
	long m_lastClick;

	bool m_bStatues;
};


#endif