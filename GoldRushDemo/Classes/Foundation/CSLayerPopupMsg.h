#ifndef _CFLAYERPOPUPMSG_H_
#define _CFLAYERPOPUPMSG_H_

#include "CSLayer.h"

//MESSAGEBOX类型
typedef enum 
{
	POPUP_YES,		//默认
	POPUP_YESNO,
	POPUP_YESNOCANCEL,
}POPUPMSG_ENUM; 

typedef enum 
{
	RESPONSE_YES,		//默认
	RESPONSE_NO,		//
	RESPONSE_CANCEL,
}POPUPMSG_RESPONSE_ENUM; 

//消息对话框基类
//包含标题、图标、信息
//关闭、yes/no、取消等按钮
class CSLayerPopupMsg:public CSLayer
{
public:
	CSLayerPopupMsg( SelectorProtocol* target, SEL_CallFuncND selector, void *sender);
	virtual ~CSLayerPopupMsg();
	bool init(POPUPMSG_ENUM popEnum, string strTitle, string strMessage); 

	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, cocos2d::CCEvent* event);	
	POPUPMSG_RESPONSE_ENUM GetResponse(){return m_responseEnum;}

protected:
	virtual void callbackClick(CCObject* sender);

	string m_strMessage;	//信息
	string m_strBackground;	//背景图片
	string m_strTitle;	//标题栏
	
	POPUPMSG_ENUM m_popEnum;
	POPUPMSG_RESPONSE_ENUM m_responseEnum;

protected:
	SelectorProtocol*	m_pListener;
	SEL_CallFuncND		m_pfnSelector;
	void *m_sender;
};

#endif