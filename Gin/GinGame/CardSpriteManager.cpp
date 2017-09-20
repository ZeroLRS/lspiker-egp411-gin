#include "CardSpriteManager.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsSprite.h"
#include "GraphicsColor.h"
#include "CardLocator.h"
#include <assert.h>

using namespace std;

const UINT NUM_CARDS_X = 13;
const UINT NUM_CARDS_Y = 4;
const UINT NUM_CARD_SPRITES = NUM_CARDS_X * NUM_CARDS_Y;
const UINT PADDING_X = 1;//amount around the outside of image
const UINT PADDING_Y = 1;//amount around the outside of image
const UINT SPACING_X = 1;//amount of space between cards
const UINT SPACING_Y = 2;//amount of space between cards
const UINT CLUBS_ROW = 0;
const UINT SPADES_ROW = 1;
const UINT HEARTS_ROW = 2;
const UINT DIAMONDS_ROW = 3;


CardSpriteManager::CardSpriteManager()
	:mpCardBackBuffer(NULL)
{
	mSprites.resize( NUM_CARD_SPRITES, NULL );
}

void CardSpriteManager::init(GraphicsBuffer* pBuffer)
{
	UPoint2D pixelSize = CardLocator::getCardPixelSize();
	UINT cardPixelX = pixelSize.x;
	UINT cardPixelY = pixelSize.y;

	for( UINT y=0; y<NUM_CARDS_Y; y++ )
	{
		for( UINT x=0; x<NUM_CARDS_X; x++ )
		{
			UINT pixel_x = ( x * cardPixelX ) + PADDING_X + ( x * SPACING_X );
			UINT pixel_y = ( y * cardPixelY ) + PADDING_Y + ( y * SPACING_Y );
			GraphicsSprite* pSprite = new GraphicsSprite( pBuffer, pixel_x, pixel_y, cardPixelX, cardPixelY );
			UINT index = y * NUM_CARDS_X + x;
			mSprites[index] = pSprite;
		}
	}

	//create card back
	mpCardBackBuffer = GraphicsSystem::createGraphicsBuffer( cardPixelX, cardPixelY );
	GraphicsSystem::getGraphicsSystem()->fillRegion(*mpCardBackBuffer, Vector2D(3, 3), Vector2D((float)(cardPixelX - 6), (float)(cardPixelY - 6)), COLOR_RED);
	//GraphicsSystem::drawRectangle( 3, 3, cardPixelX - 6, cardPixelY - 6, COLOR_RED, true, mpCardBackBuffer );
	mpCardBackSprite = new GraphicsSprite(mpCardBackBuffer, 0, 0, cardPixelX, cardPixelY );
}

void CardSpriteManager::cleanup()
{
	for( vector<GraphicsSprite*>::iterator iter = mSprites.begin(); iter != mSprites.end(); ++iter )
	{
		delete *iter;
	}
	mSprites.clear();
	delete mpCardBackSprite;
	mpCardBackSprite = NULL;
	//delete mpCardBackBuffer;
	mpCardBackBuffer = NULL;
}

const GraphicsSprite& CardSpriteManager::getSprite(const Card& theCard) const
{
	UINT index = calcIndex( theCard );
	assert( index < NUM_CARD_SPRITES );
	return *(mSprites[index]);
}

UINT CardSpriteManager::calcIndex(const Card& theCard) const
{
	UINT row = UINT_MAX;

	switch( theCard.getSuit() )
	{
	case Card::CLUBS:
		row = CLUBS_ROW;
		break;
	case Card::SPADES:
		row = SPADES_ROW;
		break;
	case Card::HEARTS:
		row = HEARTS_ROW;
		break;
	case Card::DIAMONDS:
		row = DIAMONDS_ROW;
		break;
	}

	assert( row != UINT_MAX );

	UINT column = theCard.getValue();

	UINT index = row * NUM_CARDS_X + column;
	return index;
}
