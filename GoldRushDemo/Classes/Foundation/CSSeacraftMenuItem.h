#ifndef _CSSEACRAFTMENUITEM_H_
#define _CSSEACRAFTMENUITEM_H_

#include "../Game/DataManager.h"

class CSSeacraftMenuItem :public CCMenuItemImage
{
public:
	CSSeacraftMenuItem(int id){m_nId = id;}
	~CSSeacraftMenuItem(void){};
	static CSSeacraftMenuItem * itemFromSprite(int id, SelectorProtocol* target, SEL_MenuHandler selector);

	 bool init();
	 void UpdateInfo();

	 /** The item was selected (not activated), similar to "mouse-over" */
	 virtual void selected();
	 /** The item was unselected */
	 virtual void unselected();

	 int getId(){return m_nId;}
protected:
	int m_nId;	//´¬ID
	CCSprite *m_pForground;
};
#endif