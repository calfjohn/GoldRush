#ifndef _CSPLAYERINFOSPRITE_H_
#define _CSPLAYERINFOSPRITE_H_

#include "../Game/DataManager.h"

class CSPlayerInfoSprite :public CCNode
{
public:
	CSPlayerInfoSprite(void);
	~CSPlayerInfoSprite(void);
	 static CSPlayerInfoSprite* spriteWithPlayer(USER *pUser);
	 bool init(USER *pUser);

	 void UpdateInfo(USER *pUser);

protected:
	CCLabelTTF *m_pName;
	CCSprite *m_pIcon;
	CCSprite *m_pCoin;
	CCLabelTTF *m_pCoinLable;
	CCSprite *m_pEmployee;
	CCLabelTTF *m_pEmployeeLable;
};
#endif