#include "Hand.h"
#include <algorithm>
#include <wtypes.h>
#include <cassert>

using namespace std;

void Hand::addCard(const Card& card)
{
	mCards.push_back( card );
}

bool Hand::removeCard(const Card& card)
{
	vector<Card>::iterator iter;
	for( iter = mCards.begin(); iter != mCards.end(); ++iter )
	{
		if( (*iter) == card )
		{
			mCards.erase( iter );
			return true;
		}
	}
	return false;
}

void Hand::sortOnSuitAndValue()
{
	sort( mCards.begin(), mCards.end(), Card::sortOrderSuitAndValue );
}

void Hand::sortOnValue()
{
	sort( mCards.begin(), mCards.end(), Card::sortOrderValue );

}

void Hand::reset()
{
	mCards.clear();
}

Hand Hand::clone() const
{
	Hand newHand;

	for( vector<Card>::const_iterator iter = mCards.cbegin(); iter != mCards.cend(); ++iter )
	{
		newHand.addCard( *iter );
	}

	return newHand;
}

const Card& Hand::operator[]( UINT index )
{
	assert( index < mCards.size() );
	return mCards[index];
}

UINT Hand::getHandScore() const
{
	Hand theHand = this->clone();
	//logContents();

	vector<Card> theMatches1 = theHand.removeMatches( false );
	vector<Card> theMatches3 = theHand.removeMatches( true );
	UINT score1 = theHand.scoreExistingCards();

	Hand theHand2 = this->clone();
	vector<Card> theMatches2 = theHand2.removeMatches( true );
	vector<Card> theMatches4 = theHand2.removeMatches( false );
	UINT score2 = theHand2.scoreExistingCards();

	if( score1 < score2 )
	{
		return score1;
	}
	else
	{
		return score2;
	}
}

void Hand::logContents() const
{
	cout << endl;

	for(UINT i=0; i<mCards.size(); i++ )
	{
		const Card& theCard = mCards[i];
		cout << theCard.getValue() << theCard.getSuitName() << "\t";
	}
}

std::vector<Card> Hand::removeMatches(bool shouldSortOnValue)
{
	vector<Card> matches;
	if( getNumCards() == 0 )
	{
		return matches;
	}

	if( shouldSortOnValue )
	{
		sortOnValue();
	}
	else
	{
		sortOnSuitAndValue();
	}

	for( UINT i=0; i<getNumCards()-1; i++ )
	{
		int runLength = 1;
		for( UINT j=i+1; j<getNumCards(); j++ )
		{
			if( shouldSortOnValue )
			{
				if( mCards[i].getValue() == mCards[j].getValue() )
				{
					runLength++;
				}
				else
				{
					break;
				}
			}
			else
			{
				if( mCards[i].getSuit() == mCards[j].getSuit() && mCards[j-1].getValue() == mCards[j].getValue()-1 )
				{
					runLength++;
				}
				else
				{
					break;
				}
			}
		}
		if( runLength >=3 )
		{
			//copy matches to matches vector
			for( vector<Card>::iterator iter =  mCards.begin()+i; iter < mCards.begin()+runLength+i; ++iter )
			{
				matches.push_back( *iter );
			}

			//remove matches from hand
			mCards.erase( mCards.begin()+i, mCards.begin()+runLength+i);

			//recursively continue removing matches and capturing the matches in a temp vector
			vector<Card> temp = removeMatches( shouldSortOnValue );

			//copy temp vector to the matches vector
			for( UINT i=0; i<temp.size(); i++ )
			{
				matches.push_back( temp[i] );
			}
			break;
		}
	}
	return matches;
}

UINT Hand::scoreExistingCards() const
{
	UINT score = 0;

	for( UINT i=0; i<mCards.size(); i++ )
	{
		score += mCards[i].getPointValue();
	}
	return score;
}

Card Hand::getCard(UINT index) const
{
	assert( index < mCards.size() );
	return mCards[index];
}


