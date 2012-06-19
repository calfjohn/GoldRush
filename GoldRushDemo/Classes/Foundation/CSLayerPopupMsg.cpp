
#include "CSLayerPopupMsg.h"
#include "CSMenuItem.h"

CSLayerPopupMsg::CSLayerPopupMsg( SelectorProtocol* target, SEL_CallFuncND selector, void *sender)
{
	m_pListener = target;
	m_pfnSelector = selector;
	m_sender = sender;

	m_nCFPriority = HIGHEST_PRIORITY;
	m_bCFSwallowsTouches = true;

	m_strMessage = "我没什么事，您有事吗？";
	m_strTitle = "确认";
	m_popEnum = POPUP_YESNOCANCEL;
	m_responseEnum = RESPONSE_YES;
	m_strBackground = "Images/UI/common/0025.png";
}

CSLayerPopupMsg::~CSLayerPopupMsg()
{
}

bool CSLayerPopupMsg::init(POPUPMSG_ENUM popEnum, string strTitle, string strMessage)
{
	m_strMessage = strMessage;
	m_strTitle = strTitle;
	m_popEnum = popEnum;

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	
	//添加背景
	CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName(m_strBackground.c_str());
	pSprite->setPosition(ccp(size.width/2, size.height/2));
	addChild(pSprite);

	//标题栏
	CCLabelTTF *pLabel = CCLabelTTF::labelWithString(m_strTitle.c_str(), "DFPHaiBaoW12-GB", 28);
	pLabel->setPosition(ccp(size.width*2/5, size.height*7/10));
	pLabel->setColor(ccc3(200,200,255));
	pSprite->addChild(pLabel);

	//信息
	pLabel = CCLabelTTF::labelWithString(m_strMessage.c_str(), "DFPHaiBaoW12-GB", 28);
	pLabel->setPosition(ccp(size.width/2, size.height/2));
	pLabel->setColor(ccc3(155,155,255));
	pSprite->addChild(pLabel);

	//添加按钮
	CCMenu *pMenu = CCMenu::menuWithItems(NULL);
	pMenu->setPosition(CCPointZero);

	CCMenuItem *pItem;
	if (m_popEnum == POPUP_YESNOCANCEL)
	{
		pItem = CCMenuItemImage::itemFromNormalImage("Images/UI/common/0023.png", "Images/UI/common/0023.png", this, menu_selector(CSLayerPopupMsg::callbackClick));
		pItem->setTag(RESPONSE_YES);
		pItem->setPosition(ccp(size.width*3/10, size.height*2/5));
		pMenu->addChild(pItem, 0);	
		pLabel = CCLabelTTF::labelWithString("确认", "DFPHaiBaoW12-GB", 28);
		pLabel->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
		pItem->addChild(pLabel);
		
		pItem = CCMenuItemImage::itemFromNormalImage("Images/UI/common/0022.png", "Images/UI/common/0022.png", this, menu_selector(CSLayerPopupMsg::callbackClick));
		pItem->setTag(RESPONSE_NO);
		pItem->setPosition(ccp(size.width*5/10, size.height*2/5));
		pMenu->addChild(pItem, 0);	
		pLabel = CCLabelTTF::labelWithString("取消", "DFPHaiBaoW12-GB", 28);
		pLabel->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
		pItem->addChild(pLabel);

		pItem = CCMenuItemImage::itemFromNormalImage("Images/UI/common/0022.png", "Images/UI/common/0022.png", this, menu_selector(CSLayerPopupMsg::callbackClick));
		pItem->setTag(RESPONSE_CANCEL);
		pItem->setPosition(ccp(size.width*7/10, size.height*2/5));
		pMenu->addChild(pItem, 0);
		pLabel = CCLabelTTF::labelWithString("关闭", "DFPHaiBaoW12-GB", 28);
		pLabel->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
		pItem->addChild(pLabel);
	}
	else if (m_popEnum == POPUP_YESNO)
	{
		pItem = CCMenuItemImage::itemFromNormalImage("Images/UI/common/0023.png", "Images/UI/common/0023.png", this, menu_selector(CSLayerPopupMsg::callbackClick));
		pItem->setTag(RESPONSE_YES);
		pItem->setPosition(ccp(size.width*3/10, size.height*2/5));
		pMenu->addChild(pItem, 0);	
		pLabel = CCLabelTTF::labelWithString("确认", "DFPHaiBaoW12-GB", 28);
		pLabel->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
		pItem->addChild(pLabel);

		pItem = CCMenuItemImage::itemFromNormalImage("Images/UI/common/0022.png", "Images/UI/common/0022.png", this, menu_selector(CSLayerPopupMsg::callbackClick));
		pItem->setTag(RESPONSE_NO);
		pItem->setPosition(ccp(size.width*5/10, size.height*2/5));
		pMenu->addChild(pItem, 0);	
		pLabel = CCLabelTTF::labelWithString("取消", "DFPHaiBaoW12-GB", 28);
		pLabel->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
		pItem->addChild(pLabel);

	}
	else if (m_popEnum == POPUP_YES)
	{
		pItem = CCMenuItemImage::itemFromNormalImage("Images/UI/common/0023.png", "Images/UI/common/0023.png", this, menu_selector(CSLayerPopupMsg::callbackClick));
		pItem->setTag(RESPONSE_YES);
		pItem->setPosition(ccp(size.width*3/10, size.height*2/5));
		pMenu->addChild(pItem, 0);	
		pLabel = CCLabelTTF::labelWithString("确认", "DFPHaiBaoW12-GB", 28);
		pLabel->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
		pItem->addChild(pLabel);
	}
	
	addChild(pMenu);

	return true;
}

void CSLayerPopupMsg::onEnter()
{
	CSLayer::onEnter();
//	runAction(CFAction::GetMessagePopupMsgAction());
}

void CSLayerPopupMsg::onExit()
{
	CSLayer::onExit();
}

void CSLayerPopupMsg::callbackClick(CCObject* sender)
{
	//SimpleAudioEngine::sharedEngine()->playEffect(SOUND_GAME_CLICKUP);

	m_responseEnum = (POPUPMSG_RESPONSE_ENUM)((CSMenuItem *)sender)->getTag();

	if(m_pListener && m_pfnSelector)
	{
		(m_pListener->*m_pfnSelector)(this,m_sender);
	}

	removeFromParentAndCleanup(true);
}


bool CSLayerPopupMsg::ccTouchBegan(CCTouch* touch, cocos2d::CCEvent* event)
{	
	return true;
}

void CSLayerPopupMsg::ccTouchMoved(CCTouch* touch, cocos2d::CCEvent* event)
{
	
}

void CSLayerPopupMsg::ccTouchEnded(CCTouch* touch, cocos2d::CCEvent* event)
{
} 
