#include "MouseEvent.h"


MouseEvent::MouseEvent(UINT x, UINT y, MouseButton theButton, UINT playerNum)
	:Event(MOUSE_CLICKED_EVENT, playerNum)
	,mX(x)
	,mY(y)
	,mButton(theButton)
{

}
