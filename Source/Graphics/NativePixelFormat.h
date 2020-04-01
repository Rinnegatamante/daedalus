/*
Copyright (C) 2001 StrmnNrmn

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef GRAPHICS_NATIVEPIXELFORMAT_H_
#define GRAPHICS_NATIVEPIXELFORMAT_H_

#include "Utility/Macros.h"

struct NativePf5650
{
	u16	Bits;

	static u16 Make( u8 r, u8 g, u8 b, u8 a )
	{
		// Alpha is discarded

		#ifdef DAEDALUS_DEBUG_CONSOLE
		DAEDALUS_USE( a );
		#endif
		return ((r >> (8-BitsR)) << ShiftR) |
			   ((g >> (8-BitsG)) << ShiftG) |
			   ((b >> (8-BitsB)) << ShiftB);
	}


	NativePf5650()
	{
	}

	NativePf5650( u8 r, u8 g, u8 b, u8 a )
		:	Bits( Make( r,g,b,a ) )
	{
	}

	u8	GetR() const { u8 r( u8( ( Bits & MaskR ) >> ShiftR ) ); return (r << (8 - BitsR)) | (r >> (BitsR - (8 - BitsR))); }
	u8	GetG() const { u8 g( u8( ( Bits & MaskG ) >> ShiftG ) ); return (g << (8 - BitsG)) | (g >> (BitsG - (8 - BitsG))); }
	u8	GetB() const { u8 b( u8( ( Bits & MaskB ) >> ShiftB ) ); return (b << (8 - BitsB)) | (b >> (BitsB - (8 - BitsB))); }
	u8	GetA() const { return 255; }

	static const auto	MaskR {0x001f};
	static const auto	MaskG {0x07e0};
	static const auto	MaskB {0xf800};
	//static const u32	MaskA = 0x0000;

	static const auto	ShiftR {0};
	static const auto	ShiftG {5};
	static const auto	ShiftB {11};
	//static const u32	ShiftA = 16;

	static const auto	BitsR {5};
	static const auto	BitsG {6};
	static const auto	BitsB {5};
	//static const u32	BitsA = 0;
};
#ifdef DAEDALUS_ENABLE_ASSERTS
DAEDALUS_STATIC_ASSERT( sizeof( NativePf5650 ) == 2 );
#endif
struct NativePf5551
{
	u16	Bits;

	static u16 Make( u8 r, u8 g, u8 b, u8 a )
	{
		return ((r >> (8-BitsR)) << ShiftR) |
			   ((g >> (8-BitsG)) << ShiftG) |
			   ((b >> (8-BitsB)) << ShiftB) |
			   ((a >> (8-BitsA)) << ShiftA);		// Or could do 'a ? MaskA : 0'
	}


	NativePf5551()
	{
	}

	NativePf5551( u8 r, u8 g, u8 b, u8 a )
		:	Bits( Make( r,g,b,a ) )
	{
	}

	auto	GetR() const { u8 r( u8( ( Bits & MaskR ) >> ShiftR ) ); return (r << (8 - BitsR)) | (r >> (BitsR - (8 - BitsR))); }
	auto	GetG() const { u8 g( u8( ( Bits & MaskG ) >> ShiftG ) ); return (g << (8 - BitsG)) | (g >> (BitsG - (8 - BitsG))); }
	auto	GetB() const { u8 b( u8( ( Bits & MaskB ) >> ShiftB ) ); return (b << (8 - BitsB)) | (b >> (BitsB - (8 - BitsB))); }
	auto	GetA() const { return (Bits & MaskA) ? 255 : 0; }

	static const auto	MaskR {0x001f};
	static const auto	MaskG {0x03e0};
	static const auto	MaskB {0x7c00};
	static const auto	MaskA {0x8000};

	static const auto	ShiftR {0};
	static const auto	ShiftG {5};
	static const auto	ShiftB {10};
	static const auto	ShiftA {15};

	static const auto	BitsR {5};
	static const auto	BitsG {5};
	static const auto	BitsB {5};
	static const auto	BitsA {1};
};
DAEDALUS_STATIC_ASSERT( sizeof( NativePf5551 ) == 2 );

struct NativePf4444
{
	u16	Bits;

	static u16 Make( u8 r, u8 g, u8 b, u8 a )
	{
		return ((r >> (8-BitsR)) << ShiftR) |
			   ((g >> (8-BitsG)) << ShiftG) |
			   ((b >> (8-BitsB)) << ShiftB) |
			   ((a >> (8-BitsA)) << ShiftA);
	}

	NativePf4444()
	{
	}

	NativePf4444( u8 r, u8 g, u8 b, u8 a )
		:	Bits( Make( r,g,b,a ) )
	{
	}

	auto	GetR() const { u8 r( u8( ( Bits & MaskR ) >> ShiftR ) ); return (r << (8 - BitsR)) | (r >> (BitsR - (8 - BitsR))); }
	auto	GetG() const { u8 g( u8( ( Bits & MaskG ) >> ShiftG ) ); return (g << (8 - BitsG)) | (g >> (BitsG - (8 - BitsG))); }
	auto	GetB() const { u8 b( u8( ( Bits & MaskB ) >> ShiftB ) ); return (b << (8 - BitsB)) | (b >> (BitsB - (8 - BitsB))); }
	auto	GetA() const { u8 a( u8( ( Bits & MaskA ) >> ShiftA ) ); return (a << (8 - BitsA)) | (a >> (BitsA - (8 - BitsA))); }

	static const auto	MaskR {0x000f};
	static const auto	MaskG {0x00f0};
	static const auto	MaskB {0x0f00};
	static const auto	MaskA {0xf000};

	static const auto	ShiftR {0};
	static const auto	ShiftG {4};
	static const auto	ShiftB {8};
	static const auto	ShiftA {12};

	static const auto	BitsR {4};
	static const auto	BitsG {4};
	static const auto	BitsB {4};
	static const auto	BitsA {4};
};

#ifdef DAEDALUS_ENABLE_ASSERTS
DAEDALUS_STATIC_ASSERT( sizeof( NativePf4444 ) == 2 );
#endif

struct NativePf8888
{
	union
	{
		struct
		{
			u8		R;
			u8		G;
			u8		B;
			u8		A;
		};
		u32			Bits;
	};

	static u32 Make( u8 r, u8 g, u8 b, u8 a )
	{
		return (r << ShiftR) |
			   (g << ShiftG) |
			   (b << ShiftB) |
			   (a << ShiftA);
	}

	template< typename T >
	static NativePf8888 Make( T c )
	{
		return NativePf8888( c.GetR(), c.GetG(), c.GetB(), c.GetA() );
	}

	NativePf8888()
	{
	}

	// Would like to remove this
	explicit NativePf8888( u32 bits )
		:	Bits( bits )
	{
	}

	NativePf8888( u8 r, u8 g, u8 b, u8 a )
		:	Bits( Make( r,g,b,a ) )
	{
	}

	auto	GetR() const { return R; }
	auto	GetG() const { return G; }
	auto	GetB() const { return B; }
	auto	GetA() const { return A; }

	static const auto	MaskR {0x000000ff};
	static const auto	MaskG {0x0000ff00};
	static const auto	MaskB {0x00ff0000};
	static const auto	MaskA {0xff000000};

	static const auto	ShiftR {0};
	static const auto	ShiftG {8};
	static const auto	ShiftB {16};
	static const auto	ShiftA {24};

	static const auto	BitsR {8};
	static const auto	BitsG {8};
	static const auto	BitsB {8};
	static const auto	BitsA {8};
};

#ifdef DAEDALUS_ENABLE_ASSERTS
DAEDALUS_STATIC_ASSERT( sizeof( NativePf8888 ) == 4 );
#endif

struct NativePfCI44		// This represents 2 pixels
{
	u8	Bits;

	NativePfCI44()
	{
	}

	explicit NativePfCI44( u8 bits )
		:	Bits( bits )
	{
	}

	inline u8 GetIdxA() const		{ return (Bits >> 4) & 0xf; }
	inline u8 GetIdxB() const		{ return (Bits     ) & 0xf; }

	inline void SetIdxA(u8 value) 	{ Bits &= MaskPixelA; Bits |= (value << 4 )& MaskPixelB; }
	inline void SetIdxB(u8 value) 	{ Bits &= MaskPixelB; Bits |= value & MaskPixelA; }

	static const auto	MaskPixelA {0x0f};
	static const auto MaskPixelB {0xf0};

};
DAEDALUS_STATIC_ASSERT( sizeof( NativePfCI44 ) == 1 );

struct NativePfCI8
{
	u8	Bits;

	NativePfCI8()
	{
	}

	explicit NativePfCI8( u8 bits )
		:	Bits( bits )
	{
	}
};
#ifdef DAEDALUS_ENABLE_ASSERTS
DAEDALUS_STATIC_ASSERT( sizeof( NativePfCI8 ) == 1 );
#endif
#endif // GRAPHICS_NATIVEPIXELFORMAT_H_
