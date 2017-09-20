#pragma once

#include "..\gamelib\trackable.h"
#include "..\GameLib\Card.h"
#include <wtypes.h>
#include <vector>

class GraphicsSprite;
class GraphicsBuffer;

class CardSpriteManager :	public Trackable
{
public:
	CardSpriteManager();
	~CardSpriteManager() { cleanup(); };

	void init( GraphicsBuffer* pBuffer );
	void cleanup();
	const GraphicsSprite& getSprite( const Card& theCard ) const;
	inline const GraphicsSprite& getCardBackSprite() const { return *mpCardBackSprite; };

private:
	std::vector<GraphicsSprite*> mSprites;
	GraphicsBuffer* mpCardBackBuffer;
	GraphicsSprite* mpCardBackSprite;

	UINT calcIndex( const Card& theCard ) const;
};

