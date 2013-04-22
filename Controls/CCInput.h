#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class CCInput : public CCLayer, public CCIMEDelegate, public CCTextFieldDelegate
{
public:

	CCInput(CCRect& rc);
	~CCInput();

	static CCInput *create(CCRect rc);

	void registerWithTouchDispatcher();
    
	void onEnter();
	void onExit();
	// CCLayer
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void callbackRemoveNodeWhenDidAction(CCNode * pNode);
	// CCTextFieldDelegate
	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * pSender);
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
	virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
	virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);
	virtual bool onDraw(CCTextFieldTTF * pSender);

public:
	void setString(const char *text);
    CCString* getString(); //用这个是因为 CCString可以转成任何你想要的类型

	void setTarget(CCObject *rec, SEL_CallFuncN selector);
public:
	CCTextFieldTTF *    m_pTextField;
	CCAction *          m_pTextFieldAction;
	bool                m_bAction;
	int                 m_nCharLimit;       // the textfield max char limit

	CCRect m_responseRect; //world rect
protected:
	CCObject*       m_pListener;
	SEL_CallFuncN   m_pfnSelector;
};