#include "CCScreenLayer.h"
#include "CCScrollNode.h"

static const float kFriction = 0.95f;
static const float kEaseMoveTime = 0.3f/2;
static const float kEaseMoveRate = 1.5f/2;
static const float kDragOverItemRate = 0.35;

CCScreenLayer::CCScreenLayer()
: m_isVertical(false)
,m_staffNumPerPage(8)
,m_totalScreens(10)
,m_curScreen(0)
,m_nodeBoss(NULL)
,m_pListener(NULL)
,m_pfnSelector(NULL)
,m_touch(NULL)
,m_priority(kCCMenuHandlerPriority)
,m_isLoop(false)
{
    m_winSize = CCDirector::sharedDirector()->getWinSize();
    m_spacing = m_winSize.width;
}

CCScreenLayer::~CCScreenLayer()
{
    
}

void CCScreenLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, m_priority, true);
}

CCScreenLayer* CCScreenLayer::nodeWithStaff(vector<CCNode*>* nodes, CCRect pageRect)
{
	CCScreenLayer* pRet = new CCScreenLayer();
	if (pRet && pRet->initWithNodes(nodes, pageRect))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CCScreenLayer::initWithNodes(vector<CCNode*>* nodes, CCRect pageRect)
{
	if (!CCLayer::init())
		return false;
    
	if (!nodes || nodes->size() == 0)
	{
		return false;
	}
    
	CC_ASSERT(nodes && nodes->size());
    
	setTouchEnabled(true);
    
	m_pageRect = pageRect;
    
	m_vecStaff = *nodes;
    
	return true;
}

void CCScreenLayer::onEnter()
{
    CCLayer::onEnter();
    
    m_totalScreens = m_vecStaff.size()/m_staffNumPerPage + (m_vecStaff.size()%m_staffNumPerPage == 0 ? 0 : 1);
    
    m_vecPos.clear();
    
    m_nodeBoss = new CCScrollNode(m_pageRect);
    addChild(m_nodeBoss);
    m_nodeBoss->release();
    
    for (unsigned int i = 0; i < m_vecStaff.size(); i++)
    {
        m_vecStaff[i]->retain();
    }
    
    int halfNum = m_staffNumPerPage/2;
    if(halfNum <= 0)
        halfNum = 1;
    for (int i = 0; i < m_staffNumPerPage; i++)
    {
        CCPoint pos;
        if (1 == m_staffNumPerPage)
        {
            pos = ccp(m_pageRect.origin.x + m_pageRect.size.width/2, m_pageRect.origin.y + m_pageRect.size.height/2 );
        }
        else
        {
            if (i < halfNum)
            {
                pos = ccp(m_pageRect.origin.x + m_pageRect.size.width/m_staffNumPerPage + i * m_pageRect.size.width/halfNum, m_pageRect.origin.y + m_pageRect.size.height*3/4 );
            }
            else
            {
                pos = ccp(m_pageRect.origin.x + m_pageRect.size.width/m_staffNumPerPage + (i%halfNum) * m_pageRect.size.width/halfNum, m_pageRect.origin.y + m_pageRect.size.height*1/4);
            }
        }
        
        m_vecPos.push_back(pos);
        
    }
    
    for (unsigned int i = 0; i < m_vecStaff.size(); i++)
    {
        if(m_isVertical)
            m_vecStaff[i]->setPosition(ccp(m_vecPos[i%m_staffNumPerPage].x, m_vecPos[i%m_staffNumPerPage].y + (i/m_staffNumPerPage) * m_spacing));
        else
            m_vecStaff[i]->setPosition(ccp(m_vecPos[i%m_staffNumPerPage].x + (i/m_staffNumPerPage) * m_spacing, m_vecPos[i%m_staffNumPerPage].y));
        
        m_nodeBoss->addChild(m_vecStaff[i]);
    }
    
    if (m_isVertical)
    {
        m_nodeBoss->setPosition(ccp(0, 0));
    }
    else
    {
        m_nodeBoss->setPosition(ccp(0, 0));
    }
    
    scheduleUpdate();
}


void CCScreenLayer::onExit()
{
	CCLayer::onExit();
    
	for (unsigned int i = 0; i < m_vecStaff.size(); i++)
	{
		CC_SAFE_RELEASE(m_vecStaff[i]);
	}
}

bool CCScreenLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (m_touch != NULL) {
        
        return false;
    }
    
	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}
    
    CCPoint a = pTouch->getLocation();
    
	if (m_pageRect.containsPoint(a))
	{
        m_touch = pTouch;
        return true;
	}
	else
		return false;
	
}

void CCScreenLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	m_nodeBoss->stopAllActions();
    
	// simple position update
	CCPoint a = pTouch->getPreviousLocation();
	CCPoint b = pTouch->getLocation();
	CCPoint nowPosition = m_nodeBoss->getPosition();

	float offset = 0;

	if(m_isVertical)
	{
		offset = b.y - a.y;

		nowPosition.y += offset;
	}
	else
	{
		offset = b.x - a.x;

		float curX = m_nodeBoss->getPosition().x;
		if (curX > 0 || curX < -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage)))
			offset /= 2;

	    nowPosition.x += offset;
	}
	m_nodeBoss->setPosition(nowPosition);
    
    
    if (m_isLoop)
	{
		loopRearrange();
	}
    
}

void CCScreenLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touch = NULL;
    
    //run a action to bounce back
	CCEaseIn* EaseMove=NULL;
    
	bool bRefresh = false;
	if (m_isVertical)
	{
		float curY = m_nodeBoss->getPosition().y;
        
		if(!m_isLoop)
		{
			if (curY > 0)
			{
				EaseMove = CCEaseIn::create(CCMoveTo::create(kEaseMoveTime, ccp(0, 0)), kEaseMoveRate);
			}
			else if (curY < -(m_totalScreens - 1)*m_spacing)
			{
				EaseMove = CCEaseIn::create(CCMoveTo::create(kEaseMoveTime, ccp(0, -(m_totalScreens - 1)*m_spacing)), kEaseMoveRate);
			}
			else
			{
 				curY = -curY;
                
 				if (curY > m_curScreen*m_spacing + m_spacing/5/2)
 				{
 					m_curScreen++;
                    
 					if (curY > (m_curScreen + 1)*m_spacing - m_spacing*4/5/2)
 					{
 						m_curScreen++;
 					}
                    
 					bRefresh = true;
 				}
 				else if (curY < m_curScreen*m_spacing - m_spacing/5/2)
 				{
 					m_curScreen--;
                    
 					if (curY < (m_curScreen - 1)*m_spacing + m_spacing*4/5/2)
 					{
 						m_curScreen--;
 					}
                    
 					bRefresh = true;
 				}
 				EaseMove = CCEaseIn::create(CCMoveTo::create(kEaseMoveTime, ccp(0, -m_curScreen*m_spacing)), kEaseMoveRate);
                
			}
		}
		else
		{
			curY = -curY;
            
			if (curY > m_curScreen*m_spacing + m_spacing/5/2)
			{
				m_curScreen++;
                
				if (curY > (m_curScreen + 1)*m_spacing - m_spacing*4/5/2)
				{
					m_curScreen++;
				}
                
				bRefresh = true;
			}
			else if (curY < m_curScreen*m_spacing - m_spacing/5/2)
			{
				m_curScreen--;
                
				if (curY < (m_curScreen - 1)*m_spacing + m_spacing*4/5/2)
				{
					m_curScreen--;
				}
                
				bRefresh = true;
			}
			EaseMove = CCEaseIn::create(CCMoveTo::create(kEaseMoveTime, ccp(0, -m_curScreen*m_spacing)), kEaseMoveRate);
		}
	}
	else
	{
		float curX = m_nodeBoss->getPosition().x;
        
		if(!m_isLoop)
		{
			if (curX > 0)
			{
				EaseMove = CCEaseIn::create(CCMoveTo::create(kEaseMoveTime, ccp(0, 0)), kEaseMoveRate);
			}
			else if (curX < -(m_totalScreens - 1)*m_spacing)
			{
				EaseMove = CCEaseIn::create(CCMoveTo::create(kEaseMoveTime, ccp(-(m_totalScreens - 1)*m_spacing, 0)), kEaseMoveRate);
			}
			else
			{
				curX = -curX;
                
				if (curX > m_curScreen*m_spacing + m_spacing/5/2)
				{
					m_curScreen++;
                    
					if (curX > (m_curScreen + 1)*m_spacing - m_spacing*4/5/2)
					{
						m_curScreen++;
					}
                    
					bRefresh = true;
				}
				else if (curX < m_curScreen*m_spacing - m_spacing/5/2)
				{
					m_curScreen--;
                    
					if (curX < (m_curScreen - 1)*m_spacing + m_spacing*4/5/2)
					{
						m_curScreen--;
					}
                    
					bRefresh = true;
				}
				EaseMove = CCEaseIn::create(CCMoveTo::create(kEaseMoveTime, ccp(-m_curScreen*m_spacing, 0)), kEaseMoveRate);
			}
		}
		else
		{
			curX = -curX;
            
			if (curX > m_curScreen*m_spacing + m_spacing/5/2)
			{
				m_curScreen++;
                
				if (curX > (m_curScreen + 1)*m_spacing - m_spacing*4/5/2)
				{
					m_curScreen++;
				}
                
				bRefresh = true;
			}
			else if (curX < m_curScreen*m_spacing - m_spacing/5/2)
			{
				m_curScreen--;
                
                if (curX < (m_curScreen - 1)*m_spacing + m_spacing*4/5/2)
                {
					m_curScreen--;
                }
                
				bRefresh = true;
			}
			EaseMove = CCEaseIn::create(CCMoveTo::create(kEaseMoveTime, ccp(-m_curScreen*m_spacing, 0)), kEaseMoveRate);
		}
	}
    
	m_nodeBoss->runAction(CCSequence::createWithTwoActions(EaseMove, CCCallFunc::create(this, callfunc_selector(CCScreenLayer::moveEnd))));
    
}

void CCScreenLayer::moveEnd()
{
	if (m_pListener && m_pfnSelector)
	{
		(m_pListener->*m_pfnSelector)();
	}
    
	if (m_isLoop)
	{
		loopRearrange();
	}
}

void CCScreenLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touch = NULL;
}

void CCScreenLayer::visit()
{
	CCLayer::visit();
    
    
    
    // 	if (m_isVertical)
    // 	{
    // 		CCPoint* points = new CCPoint[m_totalScreens];
    // 		for (unsigned int i = 0; i < m_totalScreens; ++i)
    // 		{
    // 			CGFloat d = 6.0f; //< Distance between points.
    // 			float pX = m_winSize.width/15;
    // 			float pY = m_winSize.height/2 + d * ((float)i - 0.5f*(m_totalScreens-1.0f));
    // 			points[i] = ccp(pX, pY);
    // 		}
    //
    
    // 		// Set GL Values
    // 		glEnable(GL_POINT_SMOOTH);
    // 		GLboolean blendWasEnabled = glIsEnabled(GL_BLEND);
    // 		glEnable(GL_BLEND);
    //
    // 		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 		glPointSize(3.0f * CC_CONTENT_SCALE_FACTOR());
    //
    // 		// Draw Gray Points
    // 		glColor4ub(0x96,0x96,0x96,0xFF);
    // 		ccDrawPoints( points, m_totalScreens );
    //
    // 		// Draw White Point for Selected Page
    // 		glColor4ub(0xFF,0xFF,0xFF,0xFF);
    // 		ccDrawPoint(points[m_curScreen]);
    //
    // 		// Restore GL Values
    // 		glPointSize(1.0f);
    // 		glDisable(GL_POINT_SMOOTH);
    // 		if (! blendWasEnabled)
    // 			glDisable(GL_BLEND);
    //
    // 		delete [] points;
    // 	}
    // 	else
    // 	{
    // 		CCPoint* points = new CCPoint[m_totalScreens];
    // 		for (unsigned int i = 0; i < m_totalScreens; ++i)
    // 		{
    // 			CGFloat d = 6.0f; //< Distance between points.
    // 			float pX = m_winSize.width/2 + d * ((float)i - 0.5f*(m_totalScreens-1.0f));
    // 			float pY = m_winSize.height/15;
    // 			points[i] = ccp(pX, pY);
    // 		}
    //
    // 		// Set GL Values
    // 		glEnable(GL_POINT_SMOOTH);
    // 		GLboolean blendWasEnabled = glIsEnabled(GL_BLEND);
    // 		glEnable(GL_BLEND);
    //
    // 		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 		glPointSize(3.0f * CC_CONTENT_SCALE_FACTOR());
    //
    // 		// Draw Gray Points
    // 		glColor4ub(0x96,0x96,0x96,0xFF);
    // 		ccDrawPoints( points, m_totalScreens );
    //
    // 		// Draw White Point for Selected Page
    // 		glColor4ub(0xFF,0xFF,0xFF,0xFF);
    // 		ccDrawPoint(points[m_curScreen]);
    //
    // 		// Restore GL Values
    // 		glPointSize(1.0f);
    // 		glDisable(GL_POINT_SMOOTH);
    // 		if (! blendWasEnabled)
    // 			glDisable(GL_BLEND);
    //
    // 		delete [] points;
    // 	}
}

unsigned char CCScreenLayer::getPage()
{
	return getCurIndex();
}

void CCScreenLayer::setPage( int page )
{
	if (page >= m_totalScreens)
        page = m_totalScreens - 1;
    
	m_curScreen = page;
    
	m_nodeBoss->setPosition(ccp(-m_curScreen*m_spacing, 0));
    
	for (int i = 0; i < m_vecStaff.size(); i++)
	{
		CCLog("%f, %f\n", m_vecStaff[i]->getPositionX(), m_vecStaff[i]->getPositionY());
	}
  	if (m_isLoop)
  	{
  		loopRearrange();
  	}
    
	CCLog("Separator");
	for (int i = 0; i < m_vecStaff.size(); i++)
	{
		CCLog("%f, %f\n", m_vecStaff[i]->getPositionX(), m_vecStaff[i]->getPositionY());
	}
    
}

int CCScreenLayer::getCurIndex()
{
    int result = m_curScreen%((int)m_vecStaff.size());
    
    if (result < 0)
        result += m_vecStaff.size();
    
    return result;
}

void CCScreenLayer::loopRearrange()
{
    int curIndex = getCurIndex();
    
    CCNode *curNode = m_vecStaff[curIndex];
    
    int rightSide1 = curIndex + 1;
    if (rightSide1 >= m_vecStaff.size())
    {
        rightSide1 -= m_vecStaff.size();
    }
    m_vecStaff[rightSide1]->setPositionX(curNode->getPosition().x + m_spacing);
    
    int rightSide2 = curIndex + 2;
    if (rightSide2 >= m_vecStaff.size())
    {
        rightSide2 -= m_vecStaff.size();
    }
    m_vecStaff[rightSide2]->setPositionX(curNode->getPosition().x + m_spacing*2);
    
    int rightSide3 = curIndex + 3;
    if (rightSide3 >= m_vecStaff.size())
    {
        rightSide3 -= m_vecStaff.size();
    }
    m_vecStaff[rightSide3]->setPositionX(curNode->getPosition().x + m_spacing*3);
    
    int leftSide1 = curIndex - 1;
    if (leftSide1 < 0)
    {
        leftSide1 += m_vecStaff.size();
    }
    m_vecStaff[leftSide1]->setPositionX(curNode->getPosition().x - m_spacing);
    
    int leftSide2 = curIndex - 2;
    if (leftSide2 < 0)
    {
        leftSide2 += m_vecStaff.size();
    }
    m_vecStaff[leftSide2]->setPositionX(curNode->getPosition().x - m_spacing*2);
    
    int leftSide3 = curIndex - 3;
    if (leftSide3 < 0)
    {
        leftSide3 += m_vecStaff.size();
    }
    m_vecStaff[leftSide3]->setPositionX(curNode->getPosition().x - m_spacing*3);
    
}

void CCScreenLayer::update( float dt )
{
    
}