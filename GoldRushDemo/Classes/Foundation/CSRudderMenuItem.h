#ifndef _CSRUDDERMENUITEM_H_
#define _CSRUDDERMENUITEM_H_

#include "../Game/DataManager.h"

class CSRudderMenuItem :public CCMenuItemImage
{
public:
	CSRudderMenuItem(){};
	~CSRudderMenuItem(void){};
	static CSRudderMenuItem * itemFromSprite(SelectorProtocol* target, SEL_MenuHandler selector);

	virtual void selected();

protected:
	bool init();
	void UpdateInfo();

	bool bSelect;
};
#endif