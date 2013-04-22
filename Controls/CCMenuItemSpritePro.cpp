#include "CCMenuItemSpritePro.h"
#include "support/CCPointExtension.h"
#include "cocos2d.h"

const unsigned int kZoomActionTag = 0xc0c05002;

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
		m_fOriginalScale = 1.0f;
		setNormalImage(sprite);
		setContentSize(sprite->getContentSize());
	}

	return true;
}

void CCMenuItemSpritePro::selected()
{
	//setColor(ccc3(150,150,150));
	m_bIsSelected = true;

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
void CCMenuItemSpritePro::unselected()
{
	//setColor(ccc3(255,255,255));
	m_bIsSelected = false;

	this->stopActionByTag(kZoomActionTag);
	CCAction *zoomAction = CCScaleTo::create(0.1f, m_fOriginalScale);
	zoomAction->setTag(kZoomActionTag);
	this->runAction(zoomAction);
}