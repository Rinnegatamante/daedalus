#include "stdafx.h"

#include <string.h>

#include "audiohle.h"
#include "AudioHLEProcessor.h"

#include "Core/Memory.h"
#include "Math/MathUtil.h"

#include "Debug/DBGConsole.h"


inline u16 Sample_Mask( u32 x )
{
	return (u16)( x & 0xffff );
}

void CLEARBUFF(AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "CLEARBUFF");
		#endif
  auto addr {command.Abi1ClearBuffer.Address};
	auto count {command.Abi1ClearBuffer.Count};

	gAudioHLEState.ClearBuffer( addr, count );
}

void CLEARBUFF2(AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "CLEARBUFF2");
		#endif
  auto addr {command.Abi2ClearBuffer.Address} ;
	auto count {command.Abi2ClearBuffer.Count};

	gAudioHLEState.ClearBuffer( addr, count );
}
void CLEARBUFF3(AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "CLEARBUFF3");
		#endif
  auto addr {(u16)(command.cmd0 &  0xffff)};
	auto count {(u16)(command.cmd1 & 0xffff)};
	memset(gAudioHLEState.Buffer+addr+0x4f0, 0, count);
}

void DMEMMOVE(AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "DMEMMOVE");
		#endif
  auto src {command.Abi1DmemMove.Src };
	auto dst {command.Abi1DmemMove.Dst };
	auto count {command.Abi1DmemMove.Count};

	gAudioHLEState.DmemMove( dst, src, count );

}

void DMEMMOVE2(AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "DMEMMOVE2");
		#endif
  auto src{ command.Abi2DmemMove.Src };
	auto dst{ command.Abi2DmemMove.Dst };
	auto count{ command.Abi2DmemMove.Count};

	gAudioHLEState.DmemMove( dst, src, count );
}

void DMEMMOVE3(AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "DMEMMOVE3");
		#endif
  auto src {command.Abi3DmemMove.Src};
	auto dst {command.Abi3DmemMove.Dst};
	auto count {command.Abi3DmemMove.Count};

	gAudioHLEState.DmemMove( dst + 0x4f0, src + 0x4f0, count );
}

void DUPLICATE2( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "DUPLICATE2");
		#endif
  auto Count {(command.cmd0 >> 16) & 0xff};
	auto In  {command.cmd0&0xffff};
	auto Out {command.cmd1>>16};

	u16 buff[64] {0};

	memmove(buff,gAudioHLEState.Buffer+In,128);

	while(Count)
	{
		memmove(gAudioHLEState.Buffer+Out,buff,128);
		Out+=128;
		Count--;
	}
}

void LOADBUFF( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "LOADBUFF");
		#endif
  auto addr {command.Abi1LoadBuffer.Address};// + gAudioHLEState.Segments[(command.cmd1>>24)&0xf];

	gAudioHLEState.LoadBuffer( addr );
}

void LOADBUFF2( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "LOADBUFF2");
		#endif
  // Needs accuracy verification...
	auto src {command.Abi2LoadBuffer.SrcAddr };// + gAudioHLEState.Segments[(command.cmd1>>24)&0xf];
	auto dst {command.Abi2LoadBuffer.DstAddr };
	auto count {command.Abi2LoadBuffer.Count };

	gAudioHLEState.LoadBuffer( dst, src, count );
}

void LOADBUFF3( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "LOADBUFF3");
		#endif

	auto cnt {(((command.cmd0 >> 0xC) + 3) &0xFFC)};
  auto	v0 {(command.cmd1 & 0xfffffc)};
	auto src  {(command.cmd0 &0xffc ) +0x4f0};
	memmove (gAudioHLEState.Buffer+src, rdram+v0, cnt);
}

void SAVEBUFF( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "SAVEBUFF");
		#endif
  auto addr {command.Abi1SaveBuffer.Address};// + gAudioHLEState.Segments[(command.cmd1>>24)&0xf];

	gAudioHLEState.SaveBuffer( addr );
}

void SAVEBUFF2( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "SAVEBUFF2");
		#endif
  // Needs accuracy verification...
	auto dst {command.Abi2SaveBuffer.DstAddr};// + gAudioHLEState.Segments[(command.cmd1>>24)&0xf];
	auto src {command.Abi2SaveBuffer.SrcAddr};
	auto count { command.Abi2SaveBuffer.Count};

	gAudioHLEState.SaveBuffer( dst, src, count );
}

void SAVEBUFF3( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "SAVEBUFF3");
		#endif
	auto cnt {(((command.cmd0 >> 0xC) + 3) &0xFFC)};
	auto v0 {(command.cmd1 & 0xfffffc)};
	auto src {(command.cmd0&0xffc)+0x4f0};
	memmove (rdram+v0, gAudioHLEState.Buffer+src, cnt);
}

void SEGMENT( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "SEGMENT - Not implemented");
		#endif
  /*u8	segment( command.Abi1SetSegment.Segment & 0xf );
	u32	address( command.Abi1SetSegment.Address );

	gAudioHLEState.SetSegment( segment, address );

  */
}

void SEGMENT2( AudioHLECommand command )
{

  if (isZeldaABI) {
		FILTER2( command );
		return;
	}
	if ((command.cmd0 & 0xffffff) == 0) {
		isMKABI = true;
		//gAudioHLEState.Segments[(command.cmd1>>24)&0xf] = (command.cmd1 & 0xffffff);
	} else {
		isMKABI = false;
		isZeldaABI = true;
		FILTER2( command );
	}
}

void SETBUFF( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "SETBUFF");
		#endif
  auto		flags {command.Abi1SetBuffer.Flags} ;
	auto	in {command.Abi1SetBuffer.In} ;
	auto		out {command.Abi1SetBuffer.Out} ;
	auto	count {command.Abi1SetBuffer.Count};

	gAudioHLEState.SetBuffer( flags, in, out, count );
}

void SETBUFF2( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "SETBUFF2");
		#endif
  auto		in {command.Abi2SetBuffer.In};
	auto		out {command.Abi2SetBuffer.Out};
	auto		count {command.Abi2SetBuffer.Count};

#ifdef DAEDALUS_ENABLE_ASSERTS
	auto		flags {command.Abi2SetBuffer.Flags};
	DAEDALUS_ASSERT( flags == 0, "SETBUFF flags set: %02x", flags );
#endif

	gAudioHLEState.SetBuffer( 0, in, out, count );
}

void SETLOOP( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "SETLOOP");
		#endif
  auto	loopval {command.Abi1SetLoop.LoopVal};// + gAudioHLEState.Segments[(command.cmd1>>24)&0xf];

	gAudioHLEState.SetLoop( loopval );
}

void SETLOOP2( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "SETLOOP2");
		#endif
  auto loopval {command.Abi2SetLoop.LoopVal};// + gAudioHLEState.Segments[(command.cmd1>>24)&0xf];

	gAudioHLEState.SetLoop( loopval );
}

void SETLOOP3( AudioHLECommand command )
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "SETLOOP3");
		#endif
  auto	loopval {command.Abi3SetLoop.LoopVal};// + gAudioHLEState.Segments[(command.cmd1>>24)&0xf];

	gAudioHLEState.SetLoop( loopval );
}
