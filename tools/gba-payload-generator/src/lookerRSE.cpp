
//{{BLOCK(lookerRSE)

//======================================================================
//
//	lookerRSE, 16x32@4, 
//	+ palette 16 entries, not compressed
//	+ 8 tiles lz77 compressed
//	Total size: 32 + 192 = 224
//
//	Time-stamp: 2026-07-17, 13:18:26
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#include "lookerRSE.h"

const unsigned int lookerRSETiles[48] __attribute__((aligned(4)))=
{
	0x00010010,0xF000003E,0xF001F001,0x2001F001,0x0000110D,0x00888100,0x00888210,0x22288100,
	0x81818810,0x001CC088,0x03001888,0x00000188,0x00188188,0x00121D11,0x11881100,0x47111017,
	0x14D000CC,0x1D00C4DF,0x0000F1F7,0x00FF77D0,0x00777DEE,0xDDEBEE00,0x5E99E9E0,0x18171700,
	0xC174CC00,0xFD4C000D,0x7F1F0D41,0xFF0000D1,0x77000D77,0x0000EED7,0x00EEBEDD,0x0E9E99E5,
	0xE9EEE000,0x9EE4D0AE,0xED0000AE,0x10003EBB,0x00026BBE,0x00166610,0x11651010,0x9F100140,
	0x0EEE9EEA,0x4E00E9EA,0xDEBBE30D,0x04EBB600,0x66610001,0x01A10001,0x10106000,0x0001008C,
};

const unsigned short lookerRSEPal[16] __attribute__((aligned(4)))=
{
	0x7680,0x0000,0x4A31,0x24AB,0x4B1F,0x7FBD,0x0863,0x3A7B,
	0x296B,0x2A14,0x3910,0x194C,0x5B5F,0x210F,0x0885,0x4F3F,
};

//}}BLOCK(lookerRSE)
