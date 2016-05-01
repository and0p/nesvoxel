#include "stdafx.h"
#include "VoxelPPUSnapshot.h"

VoxelPPUSnapshot::VoxelPPUSnapshot(const void *vram)
{
	unsigned char *oamStart = (unsigned char*)vram;
	for (int i = 0; i < 64; i++) {
		unsigned char *spritePtr = oamStart + (i*4);
		sprites[i] = buildSprite(spritePtr);
	}
}

OamSprite *VoxelPPUSnapshot::buildSprite(unsigned char* ptr)
{
	OamSprite *sprite = new OamSprite();
	sprite->y = *ptr;
	ptr += 2;
	sprite->x = *ptr;
	return sprite;
}