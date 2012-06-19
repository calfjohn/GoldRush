#ifndef _CSSPRITE_H_
#define _CSSPRITE_H_

#include "../GAME/DataManager.h"

//精灵基类
class CSSprite:public CCSprite, public CCTargetedTouchDelegate
{
public:
	CSSprite();
	virtual ~CSSprite();

	CCRect rect();
	CCRect GetWindowRect();
	virtual void onEnter();
	virtual void onExit();

	bool containsTouchLocation(CCSprite* pSpirte, CCTouch* touch);
	bool containsTouchLocation(CCTouch* touch);
	virtual bool ccTouchBegan(CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, cocos2d::CCEvent* event);
	static CSSprite* spriteWithFile(const char *pszFileName);

	/** priority */
	int getPriority(void);
	void setPriority(int nPriority);

	/** whether or not the touches are swallowed */
	bool isSwallowsTouches(void);
	void setSwallowsTouches(bool bSwallowsTouches);

protected:
	int	m_nCFPriority;
	bool m_bCFSwallowsTouches;
};

#endif