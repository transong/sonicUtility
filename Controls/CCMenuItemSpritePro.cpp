#include "CCMenuItemSpritePro.h"
#include "support/CCPointExtension.h"
#include "cocos2d.h"

const unsigned int kZoomActionTag = 0xc0c05002;

void CCMenuItemSpritePro::setColor(const ccColor3B& color)
{
	CCRGBAProtocol *pRGBAProtocol1 = dynamic_cast<CCRGBAProtocol*>(m_pNormalImage);
	if(pRGBAProtocol1)pRGBAProtocol1->setColor(color);
}

const ccColor3B& CCMenuItemSpritePro::getColor(void)
{
	CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(m_pNormalImage);
	if(pRGBAProtocol)
	{
		return pRGBAProtocol->getColor();
	}
	return ccc3(0,0,0);
}

GLubyte CCMenuItemSpritePro::getOpacity(void)
{
	CCSprite *pRGBAProtocol = dynamic_cast<CCSprite*>(m_pNormalImage);
	if(pRGBAProtocol)
	{
		return pRGBAProtocol->getOpacity();
	}
	return 255;
}

void CCMenuItemSpritePro::setOpacity(GLubyte opacity)
{
	CCSprite *pRGBAProtocol1 = dynamic_cast<CCSprite*>(m_pNormalImage);
	if(pRGBAProtocol1)pRGBAProtocol1->setOpacity(opacity);

}

CCMenuItemSpritePro* CCMenuItemSpritePro::itemFromNormalSprite(CCNode* sprite)
{
	return CCMenuItemSpritePro::itemFromNormalSprite(sprite, NULL, NULL);
}

CCMenuItemSpritePro* CCMenuItemSpritePro::itemFromNormalSprite(CCNode* sprite, CCObject* target, SEL_MenuHandler selector)
{
	CCMenuItemSpritePro *pRet = new CCMenuItemSpritePro();
	pRet->initFromNormalSprite(sprite, target, selector); 
	pRet->autorelease();
	return pRet;
}

bool CCMenuItemSpritePro::initFromNormalSprite(CCNode* sprite, CCObject* target, SEL_MenuHandler selector)
{
	assert(sprite != NULL);

	if( CCMenuItem::initWithTarget(target, selector) ) 
	{	
		m_selectEffectChangeColor = false;
		m_fOriginalScale = 1.0f;
		setNormalImage(sprite);
		setContentSize(sprite->getContentSize());
	}

	return true;
}

void CCMenuItemSpritePro::selected()
{

	m_bSelected = true;


	if (m_selectEffectChangeColor)
	{
		setColor(ccc3(150,150,150));
	}
	else
	{
		CCAction *action = getActionByTag(kZoomActionTag);
		if (action)
		{
			this->stopAction(action);
		}
		else
		{
			m_fOriginalScale = this->getScale();
		}

		CCAction *zoomAction = CCScaleTo::create(0.1f, m_fOriginalScale*1.25f);
		zoomAction->setTag(kZoomActionTag);
		this->runAction(zoomAction);
	}

}
void CCMenuItemSpritePro::unselected()
{
	m_bSelected = false;

	if (m_selectEffectChangeColor)
	{
	    setColor(ccc3(255,255,255));
	}
	else
	{
		this->stopActionByTag(kZoomActionTag);
		CCAction *zoomAction = CCScaleTo::create(0.1f, m_fOriginalScale);
		zoomAction->setTag(kZoomActionTag);
		this->runAction(zoomAction);
	}

}