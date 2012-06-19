#ifndef _CSGAMESPOTMENUITEM_H_
#define _CSGAMESPOTMENUITEM_H_

#include "CSMenuItem.h"

class CSGameSpotMenuItem :public CSMenuItem
{
public:
	CSGameSpotMenuItem(int id){m_nId = id;}
	~CSGameSpotMenuItem(void){}
	static CSGameSpotMenuItem * itemFromSprite(int id, bool bVisible, bool bEnable, SelectorProtocol* target, SEL_MenuHandler selector);

	 bool init();
	 void UpdateInfo();

	 int getId(){return m_nId;}
protected:
	int m_nId;
	
	CCSprite *m_pSprite;
};
#endif