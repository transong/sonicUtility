#include "CCScrollLayer.h"
#include "CCScrollNode.h"

static const float kEaseMoveTime = 0.006f;
static const float kEaseMoveRate = 0.3f;

CCScrollLayer::CCScrollLayer()
:m_isVertical(false)
,m_isReverse(false)
,m_nodeBoss(NULL)
,m_scrollDiff(0)
,m_staffNumPerPage(8)
,m_isLessThanPage(false)
,m_priority(kCCMenuHandlerPriority)
,m_pListener(NULL)
,m_pfnSelector(NULL)
{
	m_winSize = CCDirector::sharedDirector()->getWinSize();
}

CCScrollLayer::~CCScrollLayer()
{

}

void CCScrollLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, m_priority, true);
}

CCScrollLayer* CCScrollLayer::nodeWithStaff(vector<CCNode*>* nodes, CCRect pageRect)
{
	CCScrollLayer* pRet = new CCScrollLayer();
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

bool CCScrollLayer::initWithNodes(vector<CCNode*>* nodes, CCRect pageRect)
{
	if (!CCLayer::init())
		return false;
	CC_ASSERT(nodes && nodes->size());

	setTouchEnabled(true);

	m_pageRect = pageRect;

	m_vecStaff = *nodes;

	return true;
}

void CCScrollLayer::moveToIndex(int index)
{
	int size = m_vecStaff.size();

	if (index < 0 || index >= size) {
		return;
	}

	if (index > size - m_staffNumPerPage) {
		index = size - m_staffNumPerPage;
	}

	CCPoint pos = CCPointZero;

	if (m_isVertical) {

		if (m_isReverse) {

           pos.y = -index*(m_pageRect.size.height/m_staffNumPerPage);
		}
		else
		{
			pos.y = index*(m_pageRect.size.height/m_staffNumPerPage);

		}
	}
	else
	{
		if (m_isReverse) {

           pos.x = -index*(m_pageRect.size.width/m_staffNumPerPage);

		}
		else
		{

            pos.x = index*(m_pageRect.size.width/m_staffNumPerPage);

		}
	}

	if (m_nodeBoss)
	{
		m_nodeBoss->setPosition(pos);
	}
	m_iniPos = pos;
}

void CCScrollLayer::moveToPosition(CCPoint pos)
{
	if (m_nodeBoss)
	{
		m_nodeBoss->setPosition(pos);
	}
	
	m_iniPos = pos;
}

void CCScrollLayer::onEnter()
{
	CCLayer::onEnter();
    
	m_nodeBoss = new CCScrollNode(m_pageRect);
	addChild(m_nodeBoss);
	m_nodeBoss->release();
    
	m_nodeBoss->setPosition(m_iniPos);

	for (int i = 0; i < m_vecStaff.size(); i++)
	{
		m_vecStaff[i]->retain();
	}
    
	m_isLessThanPage = m_vecStaff.size() <= m_staffNumPerPage;

	for (int i = 0; i < m_vecStaff.size(); i++)
	{
		if(m_isVertical)
		{
			float nY = 0;
			if(m_isReverse)
				nY = m_pageRect.origin.y + (i/m_staffNumPerPage) * m_pageRect.size.height + m_pageRect.size.height/(m_staffNumPerPage*2) + (i%m_staffNumPerPage) * m_pageRect.size.height/m_staffNumPerPage;
			else
			    nY = m_pageRect.origin.y + m_pageRect.size.height - (i/m_staffNumPerPage) * m_pageRect.size.height - m_pageRect.size.height/(m_staffNumPerPage*2) - (i%m_staffNumPerPage) * m_pageRect.size.height/m_staffNumPerPage;

			m_vecStaff[i]->setPosition(ccp(m_pageRect.origin.x + m_pageRect.size.width/2, nY));
		}
		else
		{
			float nX = 0;

			if(m_isReverse)
				nX = m_pageRect.origin.x + m_pageRect.size.width - (i/m_staffNumPerPage) * m_pageRect.size.width - m_pageRect.size.width/(m_staffNumPerPage*2) - (i%m_staffNumPerPage) * m_pageRect.size.width/m_staffNumPerPage;
			else
				nX = m_pageRect.origin.x + (i/m_staffNumPerPage) * m_pageRect.size.width + m_pageRect.size.width/(m_staffNumPerPage*2) + (i%m_staffNumPerPage) * m_pageRect.size.width/m_staffNumPerPage;

			m_vecStaff[i]->setPosition(ccp(nX,  m_pageRect.origin.y + m_pageRect.size.height/2));

		}

		m_nodeBoss->addChild(m_vecStaff[i]);
	}

}


void CCScrollLayer::onExit()
{
	CCLayer::onExit();

	for (int i = 0; i < m_vecStaff.size(); i++)
	{
		CC_SAFE_RELEASE(m_vecStaff[i]);
	}
}


bool CCScrollLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{   
	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}

	CCPoint a = pTouch->getLocation();
    
	CCRect realRc = m_pageRect;
	realRc.origin = ccpAdd(realRc.origin, getPosition());

	if (realRc.containsPoint(a))
	{
		m_nodeBoss->stopAllActions();
		m_scrollDiff = 0.0f;

		return true;
	}
	else
		return false;
}

void CCScrollLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{   
	// simple position update
	CCPoint a = pTouch->getPreviousLocation();
	CCPoint b = pTouch->getLocation();
	CCPoint nowPosition = m_nodeBoss->getPosition();

	float offset;

	if(m_isVertical)
	{
		float curY = m_nodeBoss->getPosition().y;

		offset = b.y - a.y;
        
		if (m_isLessThanPage)
		{
            offset /= 2;
		}
		else
		{
			if (m_isReverse)
			{
				if (curY > 0 || curY < -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.height/m_staffNumPerPage)))
					offset /= 2;
			}
			else
			{
				if (curY < 0 || curY > ((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.height/m_staffNumPerPage)))
					offset /= 2;
			}
		}

		nowPosition.y += offset;
	}
	else
	{
		float curX = m_nodeBoss->getPosition().x;

		offset = b.x - a.x;

		if (m_isLessThanPage)
		{
		    offset /= 2;
		}
		else
		{
			if (m_isReverse)
			{
				if (curX < 0 || curX > ((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage)))
					offset /= 2;
			}
			else
			{
				if (curX > 0 || curX < -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage)))
					offset /= 2;
				
			}
		}

		nowPosition.x += offset;
	}
    
	m_scrollDiff = offset*3;

	m_nodeBoss->setPosition(nowPosition);

}

void CCScrollLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint desPos = m_nodeBoss->getPosition();
	float distance = 0.0f;
	if (m_isVertical)
	{
      float curY = m_nodeBoss->getPosition().y;

	  if (m_isReverse)
	  {
		  if (curY > 0)
		  {
			  desPos = CCPointZero;
			  distance = 50;
		  }
		  else if (curY < -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.height/m_staffNumPerPage)))
		  {
			  desPos.y = -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.height/m_staffNumPerPage));
			  distance = 50;
		  }
		  else
		  {
			  desPos.y += m_scrollDiff;
			  distance = fabs(m_scrollDiff);

			  if (desPos.y > 0)
			  {
				  desPos = CCPointZero;
			  }
			  else if (desPos.y < -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.height/m_staffNumPerPage)))
			  {
				  desPos.y = -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.height/m_staffNumPerPage));
			  }
		  }
	  }
	  else
	  {
		  if (curY < 0)
		  {
			  desPos = CCPointZero;
			  distance = 50;
		  }
		  else if (curY > ((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.height/m_staffNumPerPage)))
		  {
			  desPos.y = ((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.height/m_staffNumPerPage));
			  distance = 50;
		  }
		  else
		  {
			  desPos.y += m_scrollDiff;
			  distance = fabs(m_scrollDiff);

              if (desPos.y < 0)
              {
				  desPos = CCPointZero;
              }
			  else if (desPos.y > ((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.height/m_staffNumPerPage)))
			  {
				  desPos.y = ((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.height/m_staffNumPerPage));
			  }
		  }
	  }
	}
	else
	{
		float curX = m_nodeBoss->getPosition().x;

		if (m_isReverse)
		{
			if (curX < 0)
			{
				desPos = CCPointZero;
				distance = 50;
			}
			else if (curX > ((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage)))
			{
				desPos.x = ((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage));
				distance = 50;
			}
			else
			{
				desPos.x += m_scrollDiff;
				distance = fabs(m_scrollDiff);

				if (desPos.x < 0)
				{
					desPos = CCPointZero;
				}
				else if (desPos.x > ((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage)))
				{
					desPos.x = ((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage));
				}
			}
		}
		else
		{
			if (curX > 0)
			{
				desPos = CCPointZero;
				distance = 50;
			}
			else if(curX < -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage)))
			{
				desPos.x = -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage));
				distance = 50;
			}
			else
			{
				desPos.x += m_scrollDiff;
				distance = fabs(m_scrollDiff);

				if (desPos.x > 0)
				{
					desPos = CCPointZero;
				}
				else if (desPos.x < -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage)))
				{
					desPos.x = -((m_vecStaff.size() - m_staffNumPerPage)*(m_pageRect.size.width/m_staffNumPerPage));
                    
				}
			}
		}
	}
    
	if(m_isLessThanPage)
	{
		desPos = CCPointZero;
		distance = 50;
	}

	m_nodeBoss->runAction(CCSequence::createWithTwoActions(CCEaseIn::create(CCMoveTo::create(kEaseMoveTime*distance, desPos), kEaseMoveRate), CCCallFunc::create(this, callfunc_selector(CCScrollLayer::moveEnd))));

}

void CCScrollLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}

void CCScrollLayer::moveEnd()
{
	if (m_pListener && m_pfnSelector)
	{
		(m_pListener->*m_pfnSelector)();
	}
}