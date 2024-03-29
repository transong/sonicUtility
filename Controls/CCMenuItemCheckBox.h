/****************************************************************************
Copyright (c) Sonic Zhao
*/

#ifndef __CCMENU_ITEM_CHECKBOX_H__
#define __CCMENU_ITEM_CHECKBOX_H__

#include "menu_nodes/CCMenuItem.h"

using namespace cocos2d;

class CCMenuItemCheckBox : public CCMenuItem
{

public:
    //properties
	bool getIsChecked(){ return m_isChecked; }
	void setIsChecked(bool bCheck);
	//    
    
	void selected();
	void unselected();

	void setEnabled(bool bEnabled)
	{
		m_bEnabled = bEnabled;
  		if(bEnabled)
  			setColor(ccc3(255,255,255));
  		else
  			setColor(ccc3(150,150,150));
	}
    
	void activate();

	//RGBAProtocol
	void setColor(const ccColor3B& color);
	const ccColor3B& getColor(void);

	GLubyte getOpacity(void);
	void setOpacity(GLubyte opacity);
	//
	void setOpacityModifyRGB(bool bValue);
	bool isOpacityModifyRGB(void) ;
    
	void setIsHide(bool isHide){m_isHide = isHide;}

	//the reason to use CCNode instead of string is it's efficient to generate a ccsprite from a spritesheet
	static CCMenuItemCheckBox* create(CCNode* frameSprite, CCNode* checkSprite);	
	static CCMenuItemCheckBox* create(CCNode* frameSprite, CCNode* checkSprite, CCObject* target, SEL_MenuHandler selector);	
	bool initFromNormalSprite(CCNode* frameSprite, CCNode* checkSprite, CCObject* target, SEL_MenuHandler selector);
protected:
private:
	bool            m_isHide;
    bool            m_isChecked;
	CCNode*         m_checkImage;
	CCNode*         m_frameImage;

	float        m_fOriginalScale;
	bool  m_selectEffectChangeColor;

};

#endif //__CCMENU_ITEM_CHECKBOX_H__