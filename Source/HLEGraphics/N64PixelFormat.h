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

#ifndef HLEGRAPHICS_N64PIXELFORMAT_H_
#define HLEGRAPHICS_N64PIXELFORMAT_H_

template< typename PixelFormatA, typename PixelFormatB >
inline PixelFormatA ConvertPixelFormat( PixelFormatB colour )
{
	return PixelFormatA( colour.GetR(), colour.GetG(), colour.GetB(), colour.GetA() );
}

struct N64Pf5551
{
	u16	Bits;

	static u16 Make( u8 r, u8 g, u8 b, u8 a )
	{
		return ((r >> (8-BitsR)) << ShiftR) |
			   ((g >> (8-BitsG)) << ShiftG) |
			   ((b >> (8-BitsB)) << ShiftB) |
			   ((a >> (8-BitsA)) << ShiftA);		// Or could do 'a ? MaskA : 0'
	}

	N64Pf5551()
	{
	}

	// Would like to remove this
	explicit N64Pf5551( u16 bits )
		:	Bits( bits )
	{
	}

	N64Pf5551( u8 r, u8 g, u8 b, u8 a )
		:	Bits( Make( r,g,b,a ) )
	{
	}

	auto GetR() const { u8 r( u8( ( Bits & MaskR ) >> ShiftR ) ); return (r << (8 - BitsR)) | (r >> (BitsR - (8 - BitsR))); }
	auto GetG() const { u8 g( u8( ( Bits & MaskG ) >> ShiftG ) ); return (g << (8 - BitsG)) | (g >> (BitsG - (8 - BitsG))); }
	auto GetB() const { u8 b( u8( ( Bits & MaskB ) >> ShiftB ) ); return (b << (8 - BitsB)) | (b >> (BitsB - (8 - BitsB))); }
	auto GetA() const { return (Bits & MaskA) ? 255 : 0; }

	static const auto	MaskR {0xf800};
	static const auto	MaskG {0x07c0};
	static const auto	MaskB {0x003e};
	static const auto	MaskA {0x0001};

	static const auto	ShiftR {11};
	static const auto	ShiftG {6};
	static const auto	ShiftB {1};
	static const auto	ShiftA {0};

	static const auto	BitsR {5};
	static const auto	BitsG {5};
	static const auto	BitsB {5};
	static const auto	BitsA {1};
};
#ifdef DAEDALUS_ENABLE_ASSERTS
DAEDALUS_STATIC_ASSERT( sizeof( N64Pf5551 ) == 2 );
#endif
struct N64Pf8888
{
	union
	{
		struct
		{
			u8				A;
			u8				B;
			u8				G;
			u8				R;
		};
		u32					Bits;
	};

	static u32 Make( u8 r, u8 g, u8 b, u8 a )
	{
		return (r << ShiftR) |
			   (g << ShiftG) |
			   (b << ShiftB) |
			   (a << ShiftA);
	}

	template< typename T >
	static N64Pf8888 Make( T c )
	{
		return N64Pf8888( c.GetR(), c.GetG(), c.GetB(), c.GetA() );
	}

	N64Pf8888()
	{
	}

	explicit N64Pf8888( u32 bits )
		:	Bits( bits )
	{
	}

	N64Pf8888( u8 r, u8 g, u8 b, u8 a )
		:	Bits( Make( r,g,b,a ) )
	{
	}

	auto	GetR() const { return R; }
	auto	GetG() const { return G; }
	auto	GetB() const { return B; }
	auto	GetA() const { return A; }

	static const auto	MaskR {0xff000000};
	static const auto	MaskG {0x00ff0000};
	static const auto	MaskB {0x0000ff00};
	static const auto	MaskA {0x000000ff};

	static const auto	ShiftR {24};
	static const auto	ShiftG {16};
	static const auto	ShiftB {8};
	static const auto	ShiftA {0};

	static const auto	BitsR {8};
	static const auto	BitsG {8};
	static const auto	BitsB {8};
	static const auto	BitsA {8};
};
#ifdef DAEDALUS_ENABLE_ASSERTS
DAEDALUS_STATIC_ASSERT( sizeof( N64Pf8888 ) == 4 );
#endif
struct N64PfIA8
{
	u8	Bits;

	N64PfIA8()
	{
	}

	auto	GetR() const { return GetI(); }
	auto	GetG() const { return GetI(); }
	auto	GetB() const { return GetI(); }

	u8	GetI() const { auto i( ( Bits & MaskI ) >> ShiftI ); return (i << (8 - BitsI)) | (i >> (BitsI - (8 - BitsI))); }
	u8	GetA() const { auto a( ( Bits & MaskA ) >> ShiftA ); return (a << (8 - BitsA)) | (a >> (BitsA - (8 - BitsA))); }

	static const auto	MaskI {0xf0}, MaskA {0x0f};
	static const auto	ShiftI {4}, ShiftA {0};
	static const auto	BitsI {4}, BitsA {4};
};
#ifdef DAEDALUS_ENABLE_ASSERTS
DAEDALUS_STATIC_ASSERT( sizeof( N64PfIA8 ) == 1 );
#endif
struct N64PfIA16
{
	union
	{
		struct
		{
			u8		A;
			u8		I;
		};
		u16			Bits;
	};

	N64PfIA16()
	{
	}

	auto	GetR() const { return I; }
	auto	GetG() const { return I; }
	auto	GetB() const { return I; }
	u8	GetA() const { return A; }

	static const auto	MaskI {0xff00};
	static const auto	MaskA {0x00ff};

	static const auto	ShiftI {8};
	static const auto	ShiftA {0};

	static const auto	BitsI {8};
	static const auto	BitsA {8};
};
#ifdef DAEDALUS_ENABLE_ASSERTS
DAEDALUS_STATIC_ASSERT( sizeof( N64PfIA16 ) == 2 );
#endif

struct N64PfI8
{
	union
	{
		struct
		{
			u8		I;
		};
		u8			Bits;
	};

	N64PfI8()
	{
	}

	auto	GetR() const { return I; }
	auto	GetG() const { return I; }
	auto	GetB() const { return I; }
	auto	GetA() const { return I; }		// Always I and not 255?

	static const auto	MaskI {0xff}, ShiftI {0}, BitsI {0};

};
#ifdef DAEDALUS_ENABLE_ASSERTS
DAEDALUS_STATIC_ASSERT( sizeof( N64PfI8 ) == 1 );
#endif

#endif // HLEGRAPHICS_N64PIXELFORMAT_H_
