#ifndef _CSMENUITEM_H_
#define _CSMENUITEM_H_

#include "../GAME/data.h"

//菜单项目基类
//其他菜单项由此继承
//只添加了一个背景图
class CSMenuItem : public CCMenuItemImage
{
public:
	CSMenuItem();
	~CSMenuItem();
	
	/** creates a menu item with a normal and selected image*/
	static CSMenuItem* itemFromNormalImage(const char *normalImage, const char *selectedImage);
	/** creates a menu item with a normal,selected  and disabled image*/
	static CSMenuItem* itemFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage);
	/** creates a menu item with a normal and selected image with target/selector */
	static CSMenuItem* itemFromNormalImage(const char *normalImage, const char *selectedImage, SelectorProtocol* target, SEL_MenuHandler selector);
	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	static CSMenuItem* itemFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, SelectorProtocol* target, SEL_MenuHandler selector);

	virtual bool init();

	/** The item was selected (not activated), similar to "mouse-over" */
	virtual void selected();
	/** The item was unselected */
	virtual void unselected();
};
#endif 
