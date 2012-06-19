#ifndef _CSDICEMENUITEM_H_
#define _CSDICEMENUITEM_H_

#include "../Game/DataManager.h"

class CSDiceMenuItem :public CCMenuItemImage
{
public:
	CSDiceMenuItem(){};
	~CSDiceMenuItem(void){};
	static CSDiceMenuItem * itemFromSprite(SelectorProtocol* target, SEL_MenuHandler selector);

	virtual void selected();

protected:
	bool init();
	void UpdateInfo();
	CCPoint GetPos(int nDice, int nStep);

	CCSprite *m_pDice1;
	CCSprite *m_pDice2;
	CCSprite *m_pDice3;

	CCSprite *m_pSprite;

	bool bSelect;
};
#endif