#include "CCMenuItemCheckBox.h"
#include "support/CCPointExtension.h"
#include "sprite_nodes/CCSprite.h"

CCMenuItemCheckBox* CCMenuItemCheckBox::itemFromNormalSprite(CCNode* frameSprite, CCNode* checkSprite)
{
   return CCMenuItemCheckBox::itemFromNormalSprite(frameSprite, checkSprite, NULL, NULL);
}

CCMenuItemCheckBox* CCMenuItemCheckBox::itemFromNormalSprite(CCNode* frameSprite, CCNode* checkSprite, CCObject* target, SEL_MenuHandler selector)
{
	CCMenuItemCheckBox *pRet = new CCMenuItemCheckBox();
	pRet->initFromNormalSprite(frameSprite, checkSprite, target, selector); 
	pRet->autorelease();
	return pRet;
}

bool CCMenuItemCheckBox::initFromNormalSprite(CCNode* frameSprite, CCNode* checkSprite, CCObject* target, SEL_MenuHandler selector)
{
	assert(frameSprite != NULL && checkSprite != NULL);

	if( CCMenuItem::initWithTarget(target, selector) ) 
	{
		m_frameImage    = frameSprite;
		m_checkImage	= checkSprite;
        
		m_isHide = false;
		m_isChecked = false;

		m_frameImage->setAnchorPoint(ccp(0, 0));
        m_checkImage->setPosition(ccp(m_frameImage->getContentSize().width/2, m_frameImage->getContentSize().height/2));

		setContentSize(m_frameImage->getContentSize());
		addChild(m_frameImage, 1);
		addChild(m_checkImage, 2);

		m_checkImage->setVisible(false);
	}

	return true;
}

void CCMenuItemCheckBox::setColor(const ccColor3B& color)
{
	CCRGBAProtocol *pRGBAProtocol1 = dynamic_cast<CCRGBAProtocol*>(m_checkImage);
	if(pRGBAProtocol1)pRGBAProtocol1->setColor(color);
	CCRGBAProtocol *pRGBAProtocol2 = dynamic_cast<CCRGBAProtocol*>(m_frameImage);
	if(pRGBAProtocol2)pRGBAProtocol2->setColor(color);
}

const ccColor3B& CCMenuItemCheckBox::getColor(void)
{
	CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(m_frameImage);
	if(pRGBAProtocol)
	{
		return pRGBAProtocol->getColor();
	}
	return ccc3(0,0,0);
}

GLubyte CCMenuItemCheckBox::getOpacity(void)
{
	CCSprite *pRGBAProtocol = dynamic_cast<CCSprite*>(m_frameImage);
	if(pRGBAProtocol)
	{
		return pRGBAProtocol->getOpacity();
	}
	return 255;
}

void CCMenuItemCheckBox::setOpacity(GLubyte opacity)
{
	CCSprite *pRGBAProtocol1 = dynamic_cast<CCSprite*>(m_checkImage);
	if(pRGBAProtocol1)pRGBAProtocol1->setOpacity(opacity);
	CCSprite *pRGBAProtocol2 = dynamic_cast<CCSprite*>(m_frameImage);
	if(pRGBAProtocol2)pRGBAProtocol2->setOpacity(opacity);

}

void CCMenuItemCheckBox::setIsChecked(bool bCheck)
{
	m_isChecked = bCheck;

	if (m_isHide)
	{
		if (bCheck)
		{
			m_checkImage->setVisible(true);
			m_frameImage->setVisible(false);
		}
		else
		{
			m_checkImage->setVisible(false);
			m_frameImage->setVisible(true);
		}
	}
	else
	{
		if (bCheck)
		{
			m_checkImage->setVisible(true);
		}
		else
		{
			m_checkImage->setVisible(false);
		}
	}
}

void CCMenuItemCheckBox::activate()
{
   setIsChecked(!m_isChecked);
   CCMenuItem::activate();
}

void CCMenuItemCheckBox::setOpacityModifyRGB( bool bValue )
{

}

bool CCMenuItemCheckBox::isOpacityModifyRGB( void )
{
	return false;
}
