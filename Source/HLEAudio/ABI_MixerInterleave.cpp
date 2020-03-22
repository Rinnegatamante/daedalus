#include "stdafx.h"

#include <string.h>

#include "audiohle.h"
#include "AudioHLEProcessor.h"

#include "Core/Memory.h"
#include "Math/MathUtil.h"

#include "Debug/DBGConsole.h"


void ADDMIXER( AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
    DBGConsole_Msg(0, "ADDMIXER");
    #endif
//  DAEDALUS_ERROR( "ADDMIXER - broken?" );
	u32 Count     = (command.cmd0 >> 12) & 0x0ff0;
	u32 InBuffer  = (command.cmd1 >> 16);
	u32 OutBuffer = command.cmd1 & 0xffff;

	s16 *inp  = (s16 *)(gAudioHLEState.Buffer + InBuffer);
	s16 *outp = (s16 *)(gAudioHLEState.Buffer + OutBuffer);
	s32 temp {};
	for (s16 cntr = 0; cntr < Count; cntr+=2)
	{
		temp = *outp + *inp;
		*outp = Saturate<s16>(temp);
		//s32 temp = Saturate<s16>( *outp + *inp );
		// *outp = temp;		// Added this - correct??

		outp++;	inp++;
	}
}

void HILOGAIN( AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "HILOAGAIN");
		#endif
  u16 count = command.cmd0 & 0xffff;
	u16 out = (command.cmd1 >> 16) & 0xffff;
	s16 hi  = (s16)((command.cmd0 >> 4) & 0xf000);
	u16 lo  = (command.cmd0 >> 20) & 0xf;
	s16 *src = (s16 *)(gAudioHLEState.Buffer+out);

	s32 tmp {}, val {};
	while( count )
	{
		val = (s32)*src;
		tmp = ((val * (s32)hi) >> 16) + (u32)(val * lo);
		*src++ = Saturate<s16>( tmp );
		src++;
		count -= 2;
	}
}

void INTERLEAVE( AudioHLECommand command)
{

  u16 inL( command.Abi1Interleave.LAddr );
  u16 inR( command.Abi1Interleave.RAddr );

  	gAudioHLEState.Interleave( inL, inR );
}


void INTERLEAVE2( AudioHLECommand command)
{

	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "INTERLEAVE2");
		#endif
  u16	inR( command.Abi2Interleave.RAddr );
	u16	inL( command.Abi2Interleave.LAddr);
	u16 out( command.Abi2Interleave.OutAddr );
	u16 count( command.Abi2Interleave.Count );

	if (count != 0)
	{
		gAudioHLEState.Interleave( out, inL, inR, count );
	}
	else
	{
		gAudioHLEState.Interleave( inL, inR );
	}
}

void INTERLEAVE3( AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "INTERLEAVE3");
		#endif
  // Needs accuracy verification...
	//inR = command.cmd1 & 0xFFFF;
	//inL = (command.cmd1 >> 16) & 0xFFFF;

	gAudioHLEState.Interleave( 0x4f0, 0x9d0, 0xb40, 0x170 );
}

void DEINTERLEAVE2( AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "DEINTERLEAVE2");
		#endif
  u16 count( command.Abi2Deinterleave.Count );
	u16 out( command.Abi2Deinterleave.Out );
	u16 in( command.Abi2Deinterleave.In );

	gAudioHLEState.Deinterleave( out, in, count );
}


void MIXER( AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "MIXER");
		#endif
  u16 dmemin( command.Abi1Mixer.DmemIn );
  	u16 dmemout( command.Abi1Mixer.DmemOut );
  	s32 gain( command.Abi1Mixer.Gain );

  	gAudioHLEState.Mixer( dmemout, dmemin, gain );
}

void MIXER2( AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "MIXER2");
		#endif
// Needs accuracy verification
u16 dmemin( command.Abi2Mixer.DmemIn );
u16 dmemout( command.Abi2Mixer.DmemOut );
s32 gain( command.Abi2Mixer.Gain );
u16	count( command.Abi2Mixer.Count * 16 );

//printf( "Mixer: i:%04x o:%04x g:%08x (%d) c:%04x - %08x%08x\n", dmemin, dmemout, gain, s16(gain), count, command.cmd0, command.cmd1 );

gAudioHLEState.Mixer( dmemout, dmemin, gain, count );		// NB - did mult gain by 2 above, then shifted by 16 inside mixer.
}

void MIXER3( AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "MIXER3");
		#endif
  // Needs accuracy verification...
	u16 dmemin  = (u16)(command.cmd1 >> 0x10)  + 0x4f0;
	u16 dmemout = (u16)(command.cmd1 & 0xFFFF) + 0x4f0;
	//u8  flags   = (u8)((command.cmd0 >> 16) & 0xff);
	s32 gain    = (s16)(command.cmd0 & 0xFFFF);

	gAudioHLEState.Mixer( dmemout, dmemin, gain, 0x170 );		// NB - did mult gain by 2 above, then shifted by 16 inside mixer.
}
