
#include "DataType.h"

#pragma once

__interface IUIInterface
{
public:
	void Draw(POINT* pts, int nNum, ArcNode* pArcNodes, int nNumArc );
	void Finish();
};