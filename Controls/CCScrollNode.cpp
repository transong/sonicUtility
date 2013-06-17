#include "CCScrollNode.h"
#include "Utility.h"

void CCScrollNode::visit()
{

	float systemRate = 1.0f;

#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
	systemRate = CCEGLView::sharedOpenGLView()->getFrameZoomFactor();
#endif

	glEnable(GL_SCISSOR_TEST);
    
	glScissor(m_rect.origin.x * Utility::getDeviceScreenRate().x * systemRate, m_rect.origin.y * Utility::getDeviceScreenRate().y * systemRate, m_rect.size.width * Utility::getDeviceScreenRate().x * systemRate, m_rect.size.height * Utility::getDeviceScreenRate().y * systemRate);
    
	CCNode::visit();
    
	glDisable(GL_SCISSOR_TEST);
}

CCScrollNode::CCScrollNode(CCRect rc)
{
	m_rect = rc;
}

CCScrollNode::~CCScrollNode()
{
    
}