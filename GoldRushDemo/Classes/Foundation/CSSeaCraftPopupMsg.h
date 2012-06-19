#ifndef _CSSEACRAFTPOPUPMSG_H_
#define _CSSEACRAFTPOPUPMSG_H_

#include "CSLayerPopupMsg.h"

//购买船上位置对话框
class CSSeaCraftPopupMsg:public CSLayerPopupMsg
{
public:
	CSSeaCraftPopupMsg(int nPlayerId, int nSeaCraftId, SelectorProtocol* target, SEL_CallFuncND selector, void *sender);
	virtual ~CSSeaCraftPopupMsg(){}
	bool init(); 

protected:
	int m_nPlayerId;
	int m_nSeaCraftId;
};

#endif