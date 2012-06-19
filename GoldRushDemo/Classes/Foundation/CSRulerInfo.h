#ifndef _CSRULERINFO_H_
#define _CSRULERINFO_H_

#include "../Game/DataManager.h"

class CSRulerInfo :public CCSprite
{
public:
	CSRulerInfo(void);
	~CSRulerInfo(void);
	 static CSRulerInfo* node();
	 bool init();

	 void UpdateInfo();

protected:
	CCSprite *m_pSeaCraft1;
	CCSprite *m_pSeaCraft2;
	CCSprite *m_pSeaCraft3;
};
#endif