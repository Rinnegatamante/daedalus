/*
Copyright (C) 2009 StrmnNrmn

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

#ifndef HLEGRAPHICS_UCODES_UCODE_GBI0_H_
#define HLEGRAPHICS_UCODES_UCODE_GBI0_H_

//*****************************************************************************
//
//*****************************************************************************
void DLParser_GBI0_Vtx( MicroCodeCommand command )
{
	auto addr {RDPSegAddr(command.vtx0.addr)};
	auto v0   {command.vtx0.v0};
	auto n    {command.vtx0.n + 1};
		#ifdef DAEDALUS_DEBUG_DISPLAYLIST
	DL_PF("    Address[0x%08x] v0[%d] Num[%d] Len[0x%04x]", addr, v0, n, command.vtx0.len);
#endif
#ifdef DAEDALUS_ENABLE_ASSERTS
	// Never seen or should happen, but just in case..
	DAEDALUS_ASSERT( (v0 + n) < 80, "Warning, attempting to load into invalid vertex positions");
	DAEDALUS_ASSERT( (addr + (n*16)) < MAX_RAM_ADDRESS, "Address out of range (0x%08x)", addr );
	#endif
	gRenderer->SetNewVertexInfo( addr, v0, n );

#ifdef DAEDALUS_DEBUG_DISPLAYLIST
	gNumVertices += n;
	DLParser_DumpVtxInfo( addr, v0, n );
#endif

}
//*****************************************************************************
//
//*****************************************************************************
// GE and PD have alot of empty tris...
// Maybe not the best idea to inline nor have an extra jump or and extra branch in Addtri..
inline bool AddTri4( u32 v0, u32 v1, u32 v2 )
{
	if( v0 == v1 )
	{
				#ifdef DAEDALUS_DEBUG_DISPLAYLIST
		DL_PF("    Tri: %d,%d,%d (Culled -> Empty?)", v0, v1, v2);
		#endif
		return false;
	}
	else
	{
		return gRenderer->AddTri(v0, v1, v2);
	}
}

//*****************************************************************************
// It's used by Golden Eye and Perfect Dark
//*****************************************************************************
void DLParser_GBI0_Tri4( MicroCodeCommand command )
{
	// While the next command pair is Tri2, add vertices
	auto pc {gDlistStack.address[gDlistStackPointer]};

	bool tris_added {false};

	do
	{
		//DL_PF("    0x%08x: %08x %08x Flag: 0x%02x %-10s", pc-8, command.inst.cmd0, command.inst.cmd1, (command.inst.cmd0 >> 16) & 0xFF, "G_GBI1_TRI4");

		//Tri #1
		auto v0 {command.tri4.v0};
		auto v1 {command.tri4.v1};
		auto v2 {command.tri4.v2};

		tris_added |= gRenderer->AddTri(v0, v1, v2);

		//Tri #2
		auto v3 {command.tri4.v3};
		auto v4 {command.tri4.v4};
		auto v5 {command.tri4.v5};

		tris_added |= AddTri4(v3, v4, v5);

		//Tri #3
		auto v6 {command.tri4.v6};
		auto v7 {command.tri4.v7};
		auto v8 {command.tri4.v8};

		tris_added |= AddTri4(v6, v7, v8);

		//Tri #4
		auto v9  {command.tri4.v9};
		auto v10 {command.tri4.v10};
		auto v11 {command.tri4.v11};

		tris_added |= AddTri4(v9, v10, v11);

		command.inst.cmd0 = *(u32 *)(g_pu8RamBase + pc+0);
		command.inst.cmd1 = *(u32 *)(g_pu8RamBase + pc+4);
		pc += 8;
	} while ( command.inst.cmd == G_GBI1_TRI2 );

	gDlistStack.address[gDlistStackPointer] = pc-8;

	if (tris_added)
	{
		gRenderer->FlushTris();
	}
}

//*****************************************************************************
// Actually line3d, not supported I think.
//*****************************************************************************
/*
void DLParser_GBI0_Quad( MicroCodeCommand command )
{
	DAEDALUS_ERROR("GBI0_Quad : Not supported in ucode0 ? ( Ignored )");
}
*/

#endif // HLEGRAPHICS_UCODES_UCODE_GBI0_H_
