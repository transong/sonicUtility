/****************************************************************************
Copyright (c) Sonic Zhao
*/

#ifndef __CCMENU_ADVANCE_H__
#define __CCMENU_ADVANCE_H__

#include "menu_nodes/CCMenu.h"
#include "CCMenuItemCheckBox.h"
#include "CCMenuItemSpritePro.h"

using namespace cocos2d;
using namespace std;
enum eMenuType
{
   MenuType_Block,
   MenuType_Ghost,
   MenuType_PreAct,
};

class CCMenuAdvance : public CCMenu
{
public:
    
	CCMenuAdvance();

	static CCMenuAdvance* create(CCMenuItem* item, ...);
	static CCMenuAdvance* createWithItem(CCMenuItem* item);
    
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);

	CCMenuItem* itemForTouch(CCTouch *touch);

	void setResponseRect(CCRect& rc) { m_responseRect = rc; }

	void setMenuType(int type){ m_menuType = type; }
	void setPriority(int pri){ m_priority = pri; }
	void setIsLarger(bool b) { m_isLarger = b; }
	void setLargerIndent(CCPoint indent){ m_largerIndent = indent;}
    
	void setEnabled( bool value );
public:
	bool m_isEnabled;
	//sound
	string m_tapBefore;
	string m_tapEnd;
protected:
private:
	bool m_isLarger;
	CCPoint m_largerIndent;
	CCRect m_responseRect;
	int m_menuType;
	int m_priority;
	

};

#endif //__CCMENU_ADVANCE_H__