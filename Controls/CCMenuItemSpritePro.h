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
		m_bEnabled = bEnabled;
		if(bEnabled)
			setColor(ccc3(255,255,255));
		else
			setColor(ccc3(130,130,130));
	}

	//RGBAProtocol
	void setColor(const ccColor3B& color);
	const ccColor3B& getColor(void);

	GLubyte getOpacity(void);
	void setOpacity(GLubyte opacity);

	static CCMenuItemSpritePro* create(CCNode* sprite);	
	static CCMenuItemSpritePro* create(CCNode* sprite, CCObject* target, SEL_MenuHandler selector);	
	bool initFromNormalSprite(CCNode* sprite, CCObject* target, SEL_MenuHandler selector);

	float        m_fOriginalScale;

	bool  m_selectEffectChangeColor;
    
};
#endif //__CCMENU_ITEM_SPRITEPRO_H__