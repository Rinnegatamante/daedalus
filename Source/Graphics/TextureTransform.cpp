#include "stdafx.h"
#include "TextureTransform.h"

#include <string.h>

#include "ColourValue.h"
#include "NativePixelFormat.h"
#include "TextureFormat.h"

template< typename T >
T * AddByteOffset( T * p, s32 offset )
{
	return reinterpret_cast< T * >( reinterpret_cast< u8 * >( p ) + offset );
}
template< typename T >
const T * AddByteOffset( const T * p, s32 offset )
{
	return reinterpret_cast< const T * >( reinterpret_cast< const u8 * >( p ) + offset );
}


// This is intended for use with swizzled and unswizzled textures, the
// assumption being that 2 and 4 byte pixels are swizzled around in
// such a way that their bytes remain in the same order in memory.
template< typename T >
static void RecolourTexture( void * p_data, u32 width, u32 height, u32 stride, c32 c )
{
	auto		r {c.GetR()};
	auto	g {c.GetG()};
	auto 	b {c.GetB()};

	T *data {reinterpret_cast< T * >( p_data )};

	for( auto y {0}; y < height; ++y )
	{
		for( auto x {0}; x < width; ++x )
		{
			data[x] = T( r, g, b, data[x].GetA() );
		}

		data = AddByteOffset( data, stride );
	}
}

template< typename T >
static void RecolourPalette( void * p_data, u32 num_entries, c32 c )
{
	auto		r {c.GetR()};
	auto		g {c.GetG()};
	auto		b {c.GetB()};

	T *data {reinterpret_cast< T * >( p_data )};

	for( auto x {0}; x < num_entries; ++x )
	{
		data[x] = T( r, g, b, data[x].GetA() );
	}
}

void Recolour( void * data, void * palette, u32 width, u32 height, u32 stride, ETextureFormat texture_format, c32 colour )
{
	switch( texture_format )
	{
	case TexFmt_5650:		RecolourTexture< NativePf5650 >( data, width, height, stride, colour );	return;
	case TexFmt_5551:		RecolourTexture< NativePf5551 >( data, width, height, stride, colour );	return;
	case TexFmt_4444:		RecolourTexture< NativePf4444 >( data, width, height, stride, colour );	return;
	case TexFmt_8888:		RecolourTexture< NativePf8888 >( data, width, height, stride, colour );	return;
	case TexFmt_CI4_8888:	RecolourPalette< NativePf8888 >( palette, 16, colour );					return;
	case TexFmt_CI8_8888:	RecolourPalette< NativePf8888 >( palette, 256, colour );				return;
	}
	#ifdef DAEDALUS_DEBUG_CONSOLE
	DAEDALUS_ERROR( "Unhandled texture format" );
	#endif
}


template< typename T >
static void ClampTexels( void * texels, u32 n64_width, u32 n64_height, u32 native_width, u32 native_height, u32 native_stride )
{
	#ifdef DAEDALUS_ENABLE_ASSERTS
	DAEDALUS_ASSERT( native_stride >= native_width * sizeof( T ), "Native stride isn't big enough" );
	DAEDALUS_ASSERT( n64_width <= native_width, "n64 width greater than native width?" );
	DAEDALUS_ASSERT( n64_height <= native_height, "n64 height greater than native height?" );
	#endif

	T * data {reinterpret_cast< T * >( texels )};

	//
	//	If any of the rows are short, we need to duplicate the last pixel on the row
	//	Stick this in an outer predicate incase they match
	//
	if( native_width > n64_width )
	{
		for( auto y {0}; y < n64_height; ++y )
		{
			T	colour( data[ n64_width - 1 ] );

			for( auto x {n64_width}; x < native_width; ++x )
			{
				data[ x ] = colour;
			}

			data = AddByteOffset( data, native_stride );
		}
	}
	else
	{
		data = AddByteOffset( data, n64_height * native_stride );
	}

	//
	//	At this point all the rows up to the n64 height have been padded out.
	//	We need to duplicate the last row for every additional native row.
	//
	if( native_height > n64_height )
	{
		const void *last_row {AddByteOffset( texels, ( n64_height - 1 ) * native_stride )};

		for( auto y {n64_height}; y < native_height; ++y )
		{
			memcpy( data, last_row, native_stride );

			data = AddByteOffset( data, native_stride );
		}
	}
}

template<>
void ClampTexels< NativePfCI44 >( void * texels, u32 n64_width, u32 n64_height, u32 native_width, u32 native_height, u32 native_stride )
{
	NativePfCI44  *data {reinterpret_cast<  NativePfCI44  * >( texels )};

	//
	//	If any of the rows are short, we need to duplicate the last pixel on the row
	//	Stick this in an outer predicate incase they match
	//
	if( native_width > n64_width )
	{
		for( auto y {0}; y < n64_height; ++y )
		{
			NativePfCI44	colour0  {data[ (n64_width - 1)]};
			auto	colour {0};

			if (n64_width & 1)
			{
				// even
				colour = colour0.GetIdxB();
			}
			else
			{
				colour = colour0.GetIdxA();
			}

			for( auto x {n64_width}; x < native_width; ++x )
			{
				if (x & 1)
					data[ x >> 1 ].SetIdxB(colour);
				else
					data[ x >> 1 ].SetIdxA(colour);
			}

			data = AddByteOffset( data, native_stride );
		}
	}
	else
	{
		data = AddByteOffset( data, n64_height * native_stride );
	}

	//
	//	At this point all the rows up to the n64 height have been padded out.
	//	We need to duplicate the last row for every additional native row.
	//
	if( native_height > n64_height )
	{
		const void *last_row {AddByteOffset( texels, ( n64_height - 1 ) * native_stride)};

		for( auto y {n64_height}; y < native_height; ++y )
		{
			memcpy( data, last_row, native_stride );

			data = AddByteOffset( data, native_stride );
		}
	}
}

void ClampTexels( void * texels, u32 n64_width, u32 n64_height, u32 native_width, u32 native_height, u32 native_stride, ETextureFormat texture_format )
{
	switch( texture_format )
	{
	case TexFmt_5650:		ClampTexels< NativePf5650 >( texels, n64_width, n64_height, native_width, native_height, native_stride ); return;
	case TexFmt_5551:		ClampTexels< NativePf5551 >( texels, n64_width, n64_height, native_width, native_height, native_stride ); return;
	case TexFmt_4444:		ClampTexels< NativePf4444 >( texels, n64_width, n64_height, native_width, native_height, native_stride ); return;
	case TexFmt_8888:		ClampTexels< NativePf8888 >( texels, n64_width, n64_height, native_width, native_height, native_stride ); return;
	case TexFmt_CI4_8888:	ClampTexels< NativePfCI44 >( texels, n64_width, n64_height, native_width, native_height, native_stride ); return;
	case TexFmt_CI8_8888:	ClampTexels< NativePfCI8 > ( texels, n64_width, n64_height, native_width, native_height, native_stride ); return;
	}
	#ifdef DAEDALUS_DEBUG_CONSOLE
	DAEDALUS_ERROR( "Unhandled texture format" );
	#endif
}


template< typename T >
static void CopyRow( T * dst, const T * src, u32 pixels )
{
	memcpy( dst, src, pixels * sizeof( T ) );
}

template<>
void CopyRow( NativePfCI44 * dst, const NativePfCI44 * src, u32 pixels )
{
	for( auto i {0}; i+1 < pixels; i += 2 )
	{
		dst[ i/2 ] = src[ i/2 ];
	}

	// Handle odd pixel..
	if( pixels & 1 )
	{
		auto	s {src[ pixels / 2 ].Bits};

		dst[ pixels/2 ].Bits &=     ~NativePfCI44::MaskPixelA;
		dst[ pixels/2 ].Bits |= (s & NativePfCI44::MaskPixelA );
	}
}

template< typename T >
static void CopyRowReverse( T * dst, const T * src, u32 pixels )
{
	auto last_pixel {pixels * 2 - 1};

	for( auto i {0}; i < pixels; ++i )
	{
		dst[ last_pixel - i ] = src[ i ];
	}
}

template<>
void CopyRowReverse( NativePfCI44 * dst, const NativePfCI44 * src, u32 pixels )
{
		#ifdef DAEDALUS_DEBUG_CONSOLE
	if( pixels & 1 )
	{
		// Odd

		DAEDALUS_ERROR( "MirrorS unsupported for odd-width CI4 textures" );

	}
	else
	{
				#endif
		// Even number of pixels

		const auto first_pair_idx {0};
		const auto last_pair_idx {pixels * 2 - 2};

		for( auto i {0}; i < pixels; i += 2 )
		{
			auto s {src[ (first_pair_idx + i) / 2 ].Bits};
			auto d {(s>>4) | (s<<4)};		// Swap

			dst[ (last_pair_idx - i) / 2 ].Bits = d;
		}
#ifdef DAEDALUS_DEBUG_CONSOLE
	}
	#endif
}


// Assumes width p_dst = 2*width p_src and height p_dst = 2*height p_src
template< typename T, bool MirrorS, bool MirrorT >
static void MirrorTexelsST( void * dst, u32 dst_stride, const void * src, u32 src_stride, u32 width, u32 height )
{
	T *p_dst {reinterpret_cast< T * >( dst )};
	const T *	p_src {reinterpret_cast< const T * >( src )};

	for( auto y {0}; y < height; ++y )
	{
		// Copy regular pixels
		CopyRow< T >( p_dst, p_src, width );

		if( MirrorS )
		{
			CopyRowReverse< T >( p_dst, p_src, width );
		}

		p_dst = AddByteOffset< T >( p_dst, dst_stride );
		p_src = AddByteOffset< T >( p_src, src_stride );
	}

	if( MirrorT )
	{
		// Copy remaining rows in reverse order
		for( auto y {0}; y < height; ++y )
		{
			p_src = AddByteOffset( p_src, -s32(src_stride) );

			// Copy regular pixels
			CopyRow< T >( p_dst, p_src, width );

			if( MirrorS )
			{
				CopyRowReverse< T >( p_dst, p_src, width );
			}

			p_dst = AddByteOffset< T >( p_dst, dst_stride );
		}
	}
}

template< bool MirrorS, bool MirrorT >
static void MirrorTexels( void * dst, u32 dst_stride, const void * src, u32 src_stride, ETextureFormat tex_fmt, u32 width, u32 height )
{
		#ifdef DAEDALUS_ENABLE_ASSERTS
	bool handled {false};

	switch(tex_fmt)
	{
	case TexFmt_5650:		MirrorTexelsST< NativePf5650, MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); handled = true; break;
	case TexFmt_5551:		MirrorTexelsST< NativePf5551, MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); handled = true; break;
	case TexFmt_4444:		MirrorTexelsST< NativePf4444, MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); handled = true; break;
	case TexFmt_8888:		MirrorTexelsST< NativePf8888, MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); handled = true; break;

	case TexFmt_CI4_8888:	MirrorTexelsST< NativePfCI44, MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); handled = true; break;
	case TexFmt_CI8_8888:	MirrorTexelsST< NativePfCI8 , MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); handled = true; break;
	}
#endif

switch(tex_fmt)
{
case TexFmt_5650:		MirrorTexelsST< NativePf5650, MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); break;
case TexFmt_5551:		MirrorTexelsST< NativePf5551, MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); break;
case TexFmt_4444:		MirrorTexelsST< NativePf4444, MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); break;
case TexFmt_8888:		MirrorTexelsST< NativePf8888, MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); break;

case TexFmt_CI4_8888:	MirrorTexelsST< NativePfCI44, MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); break;
case TexFmt_CI8_8888:	MirrorTexelsST< NativePfCI8 , MirrorS, MirrorT >( dst, dst_stride, src, src_stride, width, height ); break;
}

	#ifdef DAEDALUS_ENABLE_ASSERTS
	DAEDALUS_ASSERT( handled, "Unhandled format" );
	#endif
}


void MirrorTexels( bool mirror_s, bool mirror_t,
				   void *dst, u32 dst_stride,
				   const void *src, u32 src_stride,
				   ETextureFormat tex_fmt,
				   u32 width, u32 height )
{
	if( mirror_s && mirror_t )
	{
		MirrorTexels< true, true >( dst, dst_stride, src, src_stride, tex_fmt, width, height );
	}
	else if( mirror_s )
	{
		MirrorTexels< true, false >( dst, dst_stride, src, src_stride, tex_fmt, width, height );
	}
	else if( mirror_t )
	{
		MirrorTexels< false, true >( dst, dst_stride, src, src_stride, tex_fmt, width, height );
	}
}
