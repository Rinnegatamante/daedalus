/*
Copyright (C) 2012 StrmnNrmn

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

#include "Core/CPU.h"
#include "Debug/DBGConsole.h"
#include "Interface/RomDB.h"
#include "System/Paths.h"
#include "System/System.h"
#include "Test/BatchTest.h"
#include "Utility/IO.h"

#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/ctrl.h>
#include <stdio.h>
#include <string.h>
#include <psp2/kernel/clib.h> 

int main()
{
	int result = 0;

	strcpy(gDaedalusExePath, "ux0:/");

	if (!System_Init())
		return false;

	System_Open( "ux0:/rom.z64");
	CPU_Run();
	System_Close();



	//
	// Write current config out to the registry
	//
	//WriteConfiguration();

	System_Finalize();

	return result;
}

//FIXME: All this stuff needs tidying

void Dynarec_ClearedCPUStuffToDo()
{
}

void Dynarec_SetCPUStuffToDo()
{
}


extern "C" {
void _EnterDynaRec()
{
	DAEDALUS_ASSERT(false, "Unimplemented");
}
}
