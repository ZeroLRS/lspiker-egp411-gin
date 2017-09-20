#include "DiscardCardTransaction.h"

DiscardCardTransaction::DiscardCardTransaction( const Card& card, UINT playerNum, bool knock /*= false */ )
	:Transaction(DISCARD_TRANSACTION)
	,mCard(card)
	,mPlayerNum( playerNum )
	,mKnocking(knock)
{
}
