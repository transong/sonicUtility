#include "CCMenuAdvance.h"
#include "CCDirector.h"
#include "support/CCPointExtension.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouch.h"
#include "Utility.h"

//Menu
CCMenuAdvance::CCMenuAdvance()
:m_menuType(MenuType_Block)
,m_priority(kCCMenuHandlerPriority)
,m_isLarger(true)
,m_largerIndent(8)
,m_isEnabled(true)
{
	m_responseRect = CCRectMake(0, 0, CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height);

	//m_tapEnd = "menu_sel.wav";
}

void CCMenuAdvance::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, m_priority, (MenuType_Ghost == m_menuType) ? false : true);
}

CCMenuAdvance* CCMenuAdvance::menuWithItems(CCMenuItem* item, ...)
{
	va_list args;
	va_start(args,item);
	CCMenuAdvance *pRet = new CCMenuAdvance();
	if (pRet && pRet->initWithItems(item, args))
	{
		pRet->setPosition(CCPointZero);
		pRet->autorelease();
		va_end(args);
		return pRet;
	}
	va_end(args);
	CC_SAFE_DELETE(pRet);
		return NULL;
}

CCMenuAdvance* CCMenuAdvance::menuWithItem(CCMenuItem* item)
{
	return menuWithItems(item, NULL);
}

CCMenuItem* CCMenuAdvance::itemForTouch(CCTouch *touch)
{
	CCPoint touchLocation = touch->locationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);

	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = dynamic_cast<CCNode*>(pObject);
			if (pChild && pChild->isVisible() && ((CCMenuItem*)pChild)->isEnabled())
			{
				CCPoint local = pChild->convertToNodeSpace(touchLocation);
				CCRect r = ((CCMenuItem*)pChild)->rect();
				r.origin = CCPointZero;
                
				if (m_isLarger)
				{
					r.origin =  ccp(-m_largerIndent, -m_largerIndent);
					r.size = CCSizeMake(r.size.width + 2*m_largerIndent, r.size.height + 2*m_largerIndent);
				}
				

				if (CCRect::CCRectContainsPoint(r, local))
				{
					return (CCMenuItem*)pChild;
				}
			}
		}

	}

	return NULL;
}

bool CCMenuAdvance::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint a = CCDirector::sharedDirector()->convertToGL(touch->locationInView());

	if (!CCRect::CCRectContainsPoint(m_responseRect, a))
	{
		return true;
	}

	if (m_eState != kCCMenuStateWaiting || ! m_bIsVisible)
	{
		return false;
	}

	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}

	m_pSelectedItem = this->itemForTouch(touch);
	if (m_pSelectedItem)
	{
		
		m_eState = kCCMenuStateTrackingTouch;
		m_pSelectedItem->selected();
        
		if (MenuType_PreAct == m_menuType)
		{
			m_pSelectedItem->activate();

			if(!m_tapEnd.empty())
				Utility::playSFX(m_tapEnd.c_str());
		}
		
		return true;
	}

	return false;
	
}

void CCMenuAdvance::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if (MenuType_Ghost == m_menuType || MenuType_PreAct == m_menuType)
	{
		if (m_pSelectedItem)
		{   
			m_pSelectedItem->unselected();
			m_eState = kCCMenuStateWaiting;
		}
	}
	else if(MenuType_Block == m_menuType)
	{
		CCMenuItem *currentItem = this->itemForTouch(touch);
		if (currentItem != m_pSelectedItem  )
		{
			if (m_pSelectedItem)
			{
				m_pSelectedItem->unselected();
			}
			m_pSelectedItem = currentItem;
			if (m_pSelectedItem)
			{
				m_pSelectedItem->selected();
			}
		}
	}
}

void CCMenuAdvance::ccTouchEnded(CCTouch* touch, CCEvent* event)
{

	if (MenuType_Ghost == m_menuType)
	{
		CC_UNUSED_PARAM(touch);
		CC_UNUSED_PARAM(event);
		if (m_eState == kCCMenuStateTrackingTouch && m_pSelectedItem)
		{
			if (m_isEnabled)
			{
				m_pSelectedItem->unselected();
				m_pSelectedItem->activate();
			}

			if(!m_tapEnd.empty())
			  Utility::playSFX(m_tapEnd.c_str());
		}
		m_eState = kCCMenuStateWaiting;
	}
	else if (MenuType_PreAct == m_menuType)
	{
		CC_UNUSED_PARAM(touch);
		CC_UNUSED_PARAM(event);
		if (m_eState == kCCMenuStateTrackingTouch && m_pSelectedItem)
		{
			if (m_isEnabled)
			   m_pSelectedItem->unselected();
		}
		m_eState = kCCMenuStateWaiting;
	}
	else if(MenuType_Block == m_menuType)
	{
		if (m_pSelectedItem && m_pSelectedItem->isEnabled())
		{
			if (m_isEnabled)
			{
				m_pSelectedItem->unselected();
				m_pSelectedItem->activate();
			}

			if(!m_tapEnd.empty())
			   Utility::playSFX(m_tapEnd.c_str());
		}
		m_eState = kCCMenuStateWaiting;
	}

}

void CCMenuAdvance::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
    m_eState = kCCMenuStateTrackingTouch;
    
    CCMenu::ccTouchCancelled(touch, event);
}

void CCMenuAdvance::setEnabled( bool value )
{
	m_isEnabled = value;
	CCArray* pChildren = getChildren();
	CCObject* pObject = NULL;
	CCARRAY_FOREACH(pChildren, pObject)
	{
		CCMenuItem* item = (CCMenuItem*)pObject;
		item->setEnabled(value);
	}
}