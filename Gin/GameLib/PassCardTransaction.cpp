#include "PassCardTransaction.h"

PassCardTransaction::PassCardTransaction(const Card& card, UINT playerNum)
	:Transaction(PASS_CARD_TRANSACTION)
	,mCard(card)
	,mPlayerNum(playerNum)
{

}