#include "GraphicsGrid.h"
#include "GraphicsBuffer.h"
#include "GraphicsSystem.h"
#include "GraphicsColor.h"
#include "..\GameLib\Grid.h"
#include "..\GameLib\Cell.h"

#include <climits>
#include <sstream>

const size_t CELL_BORDER_WIDTH = 2;
const GraphicsColor BORDER_COLOR = COLOR_BLACK;
const GraphicsColor UNREVEALED_COLOR = COLOR_WHITE;
const GraphicsColor MINE_COLOR = COLOR_RED;
const GraphicsColor REVEALED_COLOR = COLOR_BLACK;

GraphicsGrid::GraphicsGrid( Grid* pGrid, size_t pixelWidth, size_t pixelHeight, UINT xLoc, UINT yLoc )
	:mpGrid(pGrid)
	,mWidthInCells( pGrid->getWidth() )
	,mHeightInCells( pGrid->getHeight() )
	,CELL_WIDTH( pixelWidth/mWidthInCells )
	,CELL_HEIGHT( pixelHeight/mHeightInCells )
	,mWidthInPixels( pixelWidth )
	,mHeightInPixels( pixelHeight )
	,mULPixelLocX( xLoc )
	,mULPixelLocY( yLoc )
	,mpUnrevealedCellImage( NULL )
	,mpMineCellImage( NULL )
	,mpRevealedCellImage( NULL )
{
	size_t innerWidth = CELL_WIDTH - ( CELL_BORDER_WIDTH * 2 );
	size_t innerHeight = CELL_HEIGHT - ( CELL_BORDER_WIDTH * 2 );

	mpUnrevealedCellImage = new GraphicsBuffer( CELL_WIDTH, CELL_HEIGHT );
	mpUnrevealedCellImage->setToColor( BORDER_COLOR );
	GraphicsSystem::drawRectangle( CELL_BORDER_WIDTH, CELL_BORDER_WIDTH, innerWidth, innerHeight, UNREVEALED_COLOR, true, mpUnrevealedCellImage );

	mpMineCellImage = new GraphicsBuffer( CELL_WIDTH, CELL_HEIGHT );
	mpMineCellImage->setToColor( BORDER_COLOR );
	GraphicsSystem::drawRectangle( CELL_BORDER_WIDTH, CELL_BORDER_WIDTH, innerWidth, innerHeight, MINE_COLOR, true, mpMineCellImage );

	mpRevealedCellImage = new GraphicsBuffer( CELL_WIDTH, CELL_HEIGHT );
	mpRevealedCellImage->setToColor( BORDER_COLOR );
	GraphicsSystem::drawRectangle( CELL_BORDER_WIDTH, CELL_BORDER_WIDTH, innerWidth, innerHeight, REVEALED_COLOR, true, mpRevealedCellImage );
}

GraphicsGrid::~GraphicsGrid()
{
	delete mpUnrevealedCellImage;
	delete mpMineCellImage;
	delete mpRevealedCellImage;
}

UINT GraphicsGrid::getIndexFromPixelLoc( UINT xLoc, UINT yLoc ) const
{
	//check for off the grid
	if( xLoc < mULPixelLocX || yLoc < mULPixelLocY )
	{
		return UINT_MAX;
	}

	//adjust location due to UL offset
	xLoc -= mULPixelLocX;
	yLoc -= mULPixelLocY;

	//check for off the grid
	if( xLoc >= mWidthInPixels || yLoc >= mHeightInPixels )
	{
		return UINT_MAX;
	}

	//shift to cell coordinates from pixels
	xLoc /= CELL_WIDTH;
	yLoc /= CELL_HEIGHT;

	//multiply yLoc times cell width
	UINT index = yLoc * mWidthInCells;

	//add xLoc to index
	index += xLoc;

	return index;
}

void GraphicsGrid::draw() const
{
	UINT numCells = mWidthInCells * mHeightInCells;
	UINT xCenterOffset = CELL_WIDTH/2;
	UINT yCenterOffset = CELL_HEIGHT/2;

	for( UINT i=0; i<numCells; i++ )
	{
		UINT x,y;
		getPixelLocFromIndex( i, x, y );
		Cell* pCell = mpGrid->getCellAtIndex(i);
		if( pCell->isRevealed() )
		{
			if( pCell->hasMine() )
			{
				GraphicsSystem::drawEntireBuffer( mpMineCellImage, x, y );
				//GraphicsSystem::drawText( GraphicsSystem::getFont(), x + xCenterOffset, y + yCenterOffset, COLOR_RED, "*" );
			}
			else
			{
				std::stringstream sstream;
				sstream << mpGrid->getNumAdjacentMines( i );
				GraphicsSystem::drawText( GraphicsSystem::getFont(), x + xCenterOffset, y + yCenterOffset, COLOR_RED, sstream.str() );
			}
		}
		else
		{
			GraphicsSystem::drawEntireBuffer( mpUnrevealedCellImage, x, y );
		}
	}
}

void GraphicsGrid::getPixelLocFromIndex(UINT index, UINT& x, UINT& y) const
{
	UINT cellX = index%mWidthInCells;
	UINT cellY = index/mHeightInCells;

	x = ( cellX * CELL_WIDTH ) + mULPixelLocX;
	y = ( cellY * CELL_HEIGHT ) + mULPixelLocY;
}

