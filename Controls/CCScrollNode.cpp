#include "CCScrollNode.h"


void CCScrollNode::visit()
{
	glEnable(GL_SCISSOR_TEST);	

	glScissor(m_rect.origin.x * CC_CONTENT_SCALE_FACTOR(), m_rect.origin.y * CC_CONTENT_SCALE_FACTOR() * (CCEGLView::sharedOpenGLView()->getFrameSize().height/CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height), m_rect.size.width * CC_CONTENT_SCALE_FACTOR(), m_rect.size.height * CC_CONTENT_SCALE_FACTOR() * (CCEGLView::sharedOpenGLView()->getFrameSize().height/CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height));

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