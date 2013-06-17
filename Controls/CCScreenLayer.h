#pragma once

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class CCScrollNode;
class CCScreenLayer : public CCLayer
{
public:

    //properties
	void setIsVertical(bool b) { m_isVertical = b; }
	bool getIsVertical() { return m_isVertical; }

	void setIsLoop(bool b) { m_isLoop = b; }
	bool getIsLoop() { return m_isLoop; }

	void setStaffNumPerPage(unsigned char num){ m_staffNumPerPage = num; }
	unsigned char getStaffNumPerPage(){ return m_staffNumPerPage;}

	CCScrollNode* getBoss(){return m_nodeBoss;}

	//per page only
	unsigned char getPage();
	void setPage(int page);

	//
	static CCScreenLayer* create(vector<CCNode*>* nodes, CCRect pageRect);

	bool initWithNodes(vector<CCNode*>* nodes, CCRect pageRect); 

	CCScreenLayer();
	~CCScreenLayer();

    virtual void registerWithTouchDispatcher(void);
    
    void onEnter();
	void onExit();
    
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void visit();
	void update(float dt);

	void setMoveEndTarget(CCObject* target, SEL_CallFunc selector){m_pListener = target; m_pfnSelector = selector;}
	void moveEnd();

	void setSpacing(float s) { m_spacing = s;}
	float getSpacing() { return m_spacing; }
public:

	CCRect m_pageRect;
	float m_spacing;
	CCSize m_winSize;

	int m_priority;

	void setPriority(int pri){ m_priority = pri; }

public:
	CCScrollNode* m_nodeBoss;

    bool m_isVertical;
	bool m_isLoop;
	int getCurIndex();
	void loopRearrange();

	unsigned char m_staffNumPerPage;

	//per page only
	short m_totalScreens;
    short m_curScreen;

	vector<CCPoint> m_vecPos;
	vector<CCNode*> m_vecStaff;

	CCObject*       m_pListener;
	SEL_CallFunc    m_pfnSelector;

    CCTouch *m_touch;

};
