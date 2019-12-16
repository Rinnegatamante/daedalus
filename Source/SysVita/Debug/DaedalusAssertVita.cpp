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

#include "stdafx.h"
#include "Debug/DaedalusAssert.h"

#ifdef DAEDALUS_ENABLE_ASSERTS

#include <stdarg.h>
#include <stdio.h>
#include <psp2/kernel/clib.h> 


#define PSP_LINE_SIZE 512
#define PSP_PIXEL_FORMAT 3

DaedalusAssertHook gAssertHook = NULL;

//
//	Return -1 to ignore once, 0 to ignore permenantly, 1 to break
//
EAssertResult DaedalusAssert( const char * expression, const char * file, unsigned int line, const char * msg, ... )
{
	char buffer[ 1024 ];
	va_list va;
	va_start(va, msg);
	vsnprintf( buffer, 1024, msg, va );
	buffer[1023] = 0;
	va_end(va);

	//
	//	Enter the debug menu as soon as select is newly pressed
	//

	EAssertResult	result( AR_IGNORE );

	printf( "************************************************************\n" );
	printf( "Assert Failed: %s\n", expression );
	//printf( "MemFree: Total - %d, Max - %d\n", sceKernelTotalFreeMemSize(), sceKernelMaxFreeMemSize() );
	printf( "Location: %s(%d)\n", file, line );
	printf( "\n" );
	printf( "%s\n", buffer );
	printf( "\n" );
	printf( "Press X to ignore once, [] to ignore forever, O to break\n" );
	printf( "************************************************************\n" );


	
	return result;
}
#endif //DAEDALUS_ENABLE_ASSERTS
