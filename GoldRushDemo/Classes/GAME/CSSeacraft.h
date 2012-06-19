#ifndef _CSSEACRAFT_H_
#define _CSSEACRAFT_H_

#include "../Foundation/CSSprite.h"
#include "DataManager.h"

//兔子类 用来做宣传、活动、供玩家调戏的npc
class CSSeacraft:public CSSprite 
{
public:
	CSSeacraft();
	virtual ~CSSeacraft();

	virtual bool ccTouchBegan(CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchCancelled(CCTouch* touch, cocos2d::CCEvent* event);

	static CSSeacraft* spriteWithFile(const char *pszFilename, int id);

	int ToSail(bool bDirect);//航行到当前位置,返回需要的时间

	void SetId(int id){m_id = id;}
protected:
	bool init();
	int m_nPos;		//当前位置

	int m_id;		//船id

	bool bSail;
};

#endif