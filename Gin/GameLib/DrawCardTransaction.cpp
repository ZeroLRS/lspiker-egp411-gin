#include "DrawCardTransaction.h"


DrawCardTransaction::DrawCardTransaction( DrawType drawType, UINT playerNum )
	:Transaction(DRAW_TRANSACTION)
	,mDrawType(drawType)
	,mPlayerNum(playerNum)
{
}

