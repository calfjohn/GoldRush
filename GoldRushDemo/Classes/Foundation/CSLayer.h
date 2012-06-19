#ifndef _CFLAYER_H_
#define _CFLAYER_H_

#include "../GAME/DataManager.h"

class CSLayer:public CCLayer
{
public:
	CSLayer();
	virtual ~CSLayer();
	//LAYER_NODE_FUNC(CSLayer);
	/** priority */
	int getPriority(void);
	void setPriority(int nPriority);

	/** whether or not the touches are swallowed */
	bool isSwallowsTouches(void);
	void setSwallowsTouches(bool bSwallowsTouches);
	virtual void registerWithTouchDispatcher(void);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
	bool containsTouchLocation(CCSprite* pSpirte, CCTouch* touch);
protected:
	int	m_nCFPriority;
	bool m_bCFSwallowsTouches;
};


#endif