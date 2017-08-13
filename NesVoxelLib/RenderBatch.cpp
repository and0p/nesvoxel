#include "stdafx.h"
#include "RenderBatch.hpp"

RenderBatch::RenderBatch(shared_ptr<GameData> gameData, shared_ptr<PpuSnapshot> snapshot, shared_ptr<VirtualPatternTable> vPatternTable)
	:	gameData(gameData), snapshot(snapshot), vPatternTable(vPatternTable)
{
	processOAM();
	processNametable();
}

void RenderBatch::processOAM()
{
	// Grab all (true) sprites from the snapshot
	for each (OamSprite s in snapshot->sprites)
	{
		int x = s.x;
		int y = s.y;
		int tile = s.tile;
		int palette = s.palette;
		// Make sure palette isn't out of bounds for any reason
		if (palette > 7)
			palette = 7;
		// Branch based on 16x8 mode
		if (snapshot->ctrl.spriteSize16x8)
		{
			// In 8x16, pattern table selection is specified by first bit.
			// Since you can only select even (in base 0) tiles it is free for this purpose.
			// Actual tile selection is bits 1-7.
			int patternSelection = tile & 1;
			// Nestopia tends to give tiles by absolute value, but seemingly only some of the time?
			// So we need to correct to be sure
			if (patternSelection && tile < 256)
				tile += 256;
			else if (!patternSelection && tile > 255)
				tile -= 256;
			if (patternSelection)
				tile--;
			// Flipped sprites in 8x16 also swap places on the Y axis
			if (s.vFlip)
				y += 8;
			// Get true tile #
			shared_ptr<VirtualSprite> vSprite = vPatternTable->getSprite(tile);
			shared_ptr<VirtualSprite> vSprite2 = vPatternTable->getSprite(tile + 1);
			// Render the first sprite
			computedSprites.push_back({ vSprite, nullptr, x, y, palette, s.hFlip, s.vFlip });
			//renderSprite(vSprite, x, y, s.palette, s.hFlip, s.vFlip);
			// Render the second sprite, which swaps place with vertical flip
			if (s.vFlip)
				computedSprites.push_back({ vSprite2, nullptr, x, y - 8, palette, s.hFlip, s.vFlip });
			else
				computedSprites.push_back({ vSprite2, nullptr, x, y + 8, palette, s.hFlip, s.vFlip });
		}
		else
		{
			// Select tile based on pattern table in CTRL register
			if (tile > 255 && !snapshot->getOAMSelectAtScanline(y))
			{
				tile -= 256;
			}
			else if (tile < 256 && snapshot->getOAMSelectAtScanline(y))
			{
				tile += 256;
			}
			shared_ptr<VirtualSprite> vSprite = vPatternTable->getSprite(tile);
			computedSprites.push_back({ vSprite, nullptr, x, y, palette, s.hFlip, s.vFlip });
		}
	}
}

void RenderBatch::processNametable()
{
}

void RenderBatch::processMeshes()
{
	// Process meshes (FOR NOW ASSUMING DEFAULT)
	for each(ComputedSprite s in computedSprites)
	{
		s.mesh = s.virtualSprite->defaultMesh;
	}
}

void RenderBatch::processOAMStencilGroups()
{
	int previousHueIndex = -1;
	for (int i = 0; i < computedSprites.size; i++)
	{
		if (gameData->oamGrouping == continous_samecolor)
		{
			// See if the hues are the same as the previous color

		}
	}
}

void RenderBatch::render(shared_ptr<Camera> camera)
{
}