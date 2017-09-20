#include <SDL.h>
#include <SDL_image.h>
#include "..\gamelib\Vector2D.h"
#include <cassert>

#include "GraphicsBuffer.h"
#include "GraphicsSystem.h"
#include "Renderer.h"
#include "GraphicsColor.h"

using namespace std;

BufferID GraphicsBuffer::msNextID = 0;
BufferContainer GraphicsBuffer::msBuffers;

//used to create the back buffer
GraphicsBuffer::GraphicsBuffer(SDL_Renderer* pRenderer)
	:mID(msNextID++)

{
	mpTexture = SDL_GetRenderTarget(pRenderer);
	if (mpTexture != NULL)
	{
		mValid = true;
	}

	//add buffer to container if it doesn't exist yet
	assert(!bufferExists(mID));
	msBuffers[mID] = this;
}

GraphicsBuffer::GraphicsBuffer(SDL_Texture* pTexture)
	:mpTexture(pTexture)
	,mID(msNextID++)
{
	if (pTexture != NULL)
	{
		mValid = true;
	}

	//add buffer to container if it doesn't exist yet
	assert(!bufferExists(mID));
	msBuffers[mID] = this;
}

GraphicsBuffer::~GraphicsBuffer()
{
	if (mValid)
	{
		SDL_DestroyTexture(mpTexture);
	}

	//remove from container
	msBuffers.erase(mID);
}

void GraphicsBuffer::getDimensions(int& width, int& height) const
{
	if (mValid)
	{
		SDL_QueryTexture(mpTexture, NULL, NULL, &width, &height);
	}
	else
	{
		width = INVALID_DIMENSION;
		height = INVALID_DIMENSION;
	}
}

int GraphicsBuffer::getWidth() const
{
	int width, height;

	getDimensions(width, height);

	return width;
}

int GraphicsBuffer::getHeight() const
{
	int width, height;

	getDimensions(width, height);

	return height;
}

bool GraphicsBuffer::bufferExists(const BufferID& id)
{
	BufferContainer::const_iterator iter = msBuffers.find(id);
	if (iter == msBuffers.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

GraphicsBuffer* GraphicsBuffer::getBuffer(const BufferID& id)
{
	if (bufferExists(id))
	{
		return msBuffers[id];
	}
	else
	{
		return NULL;
	}
}

void GraphicsBuffer::deleteAllGraphicsBuffers()
{
	while (msBuffers.size() > 0)
	{
		BufferContainer::iterator iter = msBuffers.begin();
		delete iter->second;
	}
}

