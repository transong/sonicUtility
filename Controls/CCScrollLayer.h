#pragma once

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class CCScrollNode;
class CCScrollLayer : public CCLayer
{
public:
	void setIsVertical(bool b) { m_isVertical = b; }
	bool getIsVertical() { return m_isVertical; }
	void setIsReverse(bool b) { m_isReverse = b; }
	bool getIsReverse() { return m_isReverse; }
	void setStaffNumPerPage(unsigned char num){ m_staffNumPerPage = num; }
	void moveToIndex(int index);
	void moveToPosition(CCPoint pos);
	unsigned char getStaffNumPerPage(){ return m_staffNumPerPage;}
	//
	static CCScrollLayer* nodeWithStaff(vector<CCNode*>* nodes, CCRect pageRect);

	bool initWithNodes(vector<CCNode*>* nodes, CCRect pageRect); 

	CCScrollLayer();
	~CCScrollLayer();

	virtual void registerWithTouchDispatcher(void);

	void onEnter();
	void onExit();

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	int m_priority;

	void setPriority(int pri){ m_priority = pri; }

	CCRect m_pageRect;
	CCSize m_winSize;
    
	CCPoint m_iniPos;
public:
	bool m_isVertical;
	bool m_isReverse;
	unsigned char m_staffNumPerPage;
    float m_scrollDiff;
	bool m_isLessThanPage;
	CCScrollNode* m_nodeBoss;
    vector<CCNode*> m_vecStaff;

	CCObject*       m_pListener;
	SEL_CallFunc    m_pfnSelector;

	void setMoveEndTarget(CCObject* target, SEL_CallFunc selector){m_pListener = target; m_pfnSelector = selector;}
	void moveEnd();
};

