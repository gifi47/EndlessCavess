#ifndef BLOCKS_H
#define BLOCKS_H

enum class BlockType;

#ifdef BLOCK_STORY

enum class BlockType{
	GrassDirt = 0x01,
	Grass = 0x10,
	Dirt = 0x20,
	Stone = 0x04,
	Sand = 0x03,
	Snow = 0x07,
	Wood = 0x11,
	Leaf = 0x29,
	Water = 0xA1
};

#else

enum class BlockType{
	GrassDirt = 0xFC,
	Grass = 0xD7,
	Dirt = 0xFD,
	Stone = 0xFE,
	Sand = 0xED,
	Snow = 0xBD,
	Wood = 0xEB,
	Leaf = 0xCB,
	Water = 0x30
};

#endif

#endif //BLOCKS_H