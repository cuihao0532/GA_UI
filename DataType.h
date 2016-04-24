
#pragma once

typedef struct _tagArcNode
{
public:
	_tagArcNode(){ memset(this, 0, sizeof(*this)); }
	CPoint ptFrom;
	CPoint ptTo;

}ArcNode, *PArcNode;