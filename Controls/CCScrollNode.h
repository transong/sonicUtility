#pragma once

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class CCScrollNode : public CCNode
{
public:
	CCScrollNode(CCRect rc);
	~CCScrollNode();
	CCRect m_rect;
	void visit();
};