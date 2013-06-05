#include "CCInput.h"
#include "GameConst.h"
#include "Utility.h"

#define FONT_NAME                       "Thonburi"
#define FONT_SIZE                       9

CCInput::CCInput(CCRect& rc)
:m_responseRect(rc)
,m_pListener(NULL)            
,m_pfnSelector(NULL)
{
	
}

CCInput::~CCInput()
{

}

CCInput *CCInput::create(CCRect rc)
{
	CCInput *pRet = new CCInput(rc);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void CCInput::setString(const char *text)
{
   m_pTextField->setString(text);
}

CCString* CCInput::getString()
{
	return CCString::create(m_pTextField->getString());
}

void CCInput::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void CCInput::onEnter()
{
	setTouchEnabled(true);
	CCLayer::onEnter();

	m_nCharLimit = 120;

	m_pTextFieldAction = CCRepeatForever::create(
		(CCActionInterval*)CCSequence::create(
		CCFadeOut::create(0.25),
		CCFadeIn::create(0.25),
		0
		));
	m_pTextFieldAction->retain();
	m_bAction = false;

	// add CCTextFieldTTF
	CCSize s = CCDirector::sharedDirector()->getWinSize();

	m_pTextField = CCTextFieldTTF::textFieldWithPlaceHolder("<click>",
		FONT_NAME,
		FONT_SIZE*macro_padFactor);
	addChild(m_pTextField);
	//m_pTextField->setColor(ccc3(255, 0, 0));

	m_pTextField->setDelegate(this);
    
	m_pTextField->setPosition(ccp(m_responseRect.origin.x + m_responseRect.size.width/2, m_responseRect.origin.y + m_responseRect.size.height/2));

	CCLayerColor* border = CCLayerColor::create(ccc4(255, 255, 255, 80), m_responseRect.size.width, m_responseRect.size.height);
	border->setPosition(ccp(m_responseRect.origin.x, m_responseRect.origin.y));
    addChild(border, -1);
}

void CCInput::onExit()
{
	CCLayer::onExit();

    m_pTextFieldAction->release();
}

// CCLayer
bool CCInput::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}
    
    CCPoint a = pTouch->getLocation();
    
	if (m_responseRect.containsPoint(a))
	{
		return true;
	}
	else
	{
		return false;
	}

}

void CCInput::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_pTextField->attachWithIME();
}

bool CCInput::onTextFieldAttachWithIME(CCTextFieldTTF * pSender)
{
	if (! m_bAction)
	{
		m_pTextField->runAction(m_pTextFieldAction);
		m_bAction = true;
	}
	return false;
}

bool CCInput::onTextFieldDetachWithIME(CCTextFieldTTF * pSender)
{
	if (m_bAction)
	{
		m_pTextField->stopAction(m_pTextFieldAction);
		m_pTextField->setOpacity(255);
		m_bAction = false;
        
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
	}
	return false;
}

bool CCInput::onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen)
{
	// if insert enter, treat as default to detach with ime
	if ('\n' == *text)
	{
		return false;
	}

	// if the textfield's char count more than m_nCharLimit, doesn't insert text anymore.
	if (pSender->getCharCount() >= m_nCharLimit)
	{
		return true;
	}

	// create a insert text sprite and do some action
	CCLabelTTF * label = CCLabelTTF::create(text, FONT_NAME, FONT_SIZE);
	this->addChild(label);
//	ccColor3B color = { 226, 121, 7};
//	label->setColor(color);

	// move the sprite from top to position
	CCPoint endPos = pSender->getPosition();
	if (pSender->getCharCount())
	{
		endPos.x += pSender->getContentSize().width / 2;
	}
	CCSize  inputTextSize = label->getContentSize();
	CCPoint beginPos(endPos.x, CCDirector::sharedDirector()->getWinSize().height - inputTextSize.height * 2); 

	float duration = 0.5;
	label->setPosition(beginPos);
	label->setScale(8);

	CCAction * seq = CCSequence::create(
		CCSpawn::create(
		CCMoveTo::create(duration, endPos),
		CCScaleTo::create(duration, 1),
		CCFadeOut::create(duration),
		0),
		CCCallFuncN::create(this, callfuncN_selector(CCInput::callbackRemoveNodeWhenDidAction)),
		0);
	label->runAction(seq);
	return false;
}

bool CCInput::onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen)
{
	// create a delete text sprite and do some action
	CCLabelTTF * label = CCLabelTTF::create(delText, FONT_NAME, FONT_SIZE);
	this->addChild(label);

	// move the sprite to fly out
	CCPoint beginPos = pSender->getPosition();
	CCSize textfieldSize = pSender->getContentSize();
	CCSize labelSize = label->getContentSize();
	beginPos.x += (textfieldSize.width - labelSize.width) / 2.0f;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint endPos(- winSize.width / 4.0f, winSize.height * (0.5 + (float)rand() / (2.0f * RAND_MAX)));

	float duration = 1;
	float rotateDuration = 0.2f;
	int repeatTime = 5; 
	label->setPosition(beginPos);

	CCAction * seq = CCSequence::create(
		CCSpawn::create(
		CCMoveTo::create(duration, endPos),
		CCRepeat::create(
		CCRotateBy::create(rotateDuration, (rand()%2) ? 360 : -360),
		repeatTime),
		CCFadeOut::create(duration),
		0),
		CCCallFuncN::create(this, callfuncN_selector(CCInput::callbackRemoveNodeWhenDidAction)),
		0);
	label->runAction(seq);
	return false;
}

bool CCInput::onDraw(CCTextFieldTTF * pSender)
{
	return false;
}

void CCInput::callbackRemoveNodeWhenDidAction(CCNode * pNode)
{
	this->removeChild(pNode, true);
}

void CCInput::setTarget(CCObject *rec, SEL_CallFuncN selector)
{
	m_pListener = rec;
	m_pfnSelector = selector;
}