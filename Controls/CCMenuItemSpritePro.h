/****************************************************************************
Songyuan
*/

#ifndef __CCMENU_ITEM_SPRITEPRO_H__
#define __CCMENU_ITEM_SPRITEPRO_H__

#include "menu_nodes/CCMenuItem.h"

using namespace cocos2d;

class CCMenuItemSpritePro : public CCMenuItemSprite
{
public :
	void selected();
	void unselected();

	void setEnabled(bool bEnabled)
	{
		m_bIsEnabled = bEnabled;
		if(bEnabled)
			setColor(ccc3(255,255,255));
		else
			setColor(ccc3(180,180,180));
	}

	static CCMenuItemSpritePro* itemFromNormalSprite(CCNode* sprite);	
	static CCMenuItemSpritePro* itemFromNormalSprite(CCNode* sprite, CCObject* target, SEL_MenuHandler selector);	
	bool initFromNormalSprite(CCNode* sprite, CCObject* target, SEL_MenuHandler selector);

	float        m_fOriginalScale;
    
};
#endif //__CCMENU_ITEM_SPRITEPRO_H__