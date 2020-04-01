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
	auto Count     {(command.cmd0 >> 12) & 0x0ff0};
	auto InBuffer  {command.cmd1 >> 16};
	auto OutBuffer {command.cmd1 & 0xffff};

	auto *inp  {(s16 *)(gAudioHLEState.Buffer + InBuffer)};
	auto *outp {(s16 *)(gAudioHLEState.Buffer + OutBuffer)};
	auto temp {0};
	for (auto cntr {0}; cntr < Count; cntr+=2)
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
  auto count {command.cmd0 & 0xffff};
	auto out {(command.cmd1 >> 16) & 0xffff};
	auto hi  {(s16)((command.cmd0 >> 4) & 0xf000)};
	auto lo  {(command.cmd0 >> 20) & 0xf};
	auto *src {(s16 *)(gAudioHLEState.Buffer+out)};

	auto tmp {0}, val {0};
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

	auto inR {command.Abi1Interleave.RAddr};
  auto inL {command.Abi1Interleave.LAddr};

  	gAudioHLEState.Interleave( inL, inR );
}


void INTERLEAVE2( AudioHLECommand command)
{

	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "INTERLEAVE2");
		#endif
  auto inR {command.Abi2Interleave.RAddr};
	u16	inL {command.Abi2Interleave.LAddr};
	u16 out {command.Abi2Interleave.OutAddr};
	u16 count {command.Abi2Interleave.Count};

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
  auto count {command.Abi2Deinterleave.Count};
	auto out {command.Abi2Deinterleave.Out};
	auto in {command.Abi2Deinterleave.In};

	gAudioHLEState.Deinterleave( out, in, count );
}


void MIXER( AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "MIXER");
		#endif
  auto dmemin {command.Abi1Mixer.DmemIn};
  auto dmemout {command.Abi1Mixer.DmemOut};
	auto gain {command.Abi1Mixer.Gain};

  	gAudioHLEState.Mixer( dmemout, dmemin, gain );
}

void MIXER2( AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "MIXER2");
		#endif
// Needs accuracy verification
auto dmemin {command.Abi2Mixer.DmemIn};
auto dmemout {command.Abi2Mixer.DmemOut};
auto gain {command.Abi2Mixer.Gain};
auto	count {command.Abi2Mixer.Count * 16};

//printf( "Mixer: i:%04x o:%04x g:%08x (%d) c:%04x - %08x%08x\n", dmemin, dmemout, gain, s16(gain), count, command.cmd0, command.cmd1 );

gAudioHLEState.Mixer( dmemout, dmemin, gain, count );		// NB - did mult gain by 2 above, then shifted by 16 inside mixer.
}

void MIXER3( AudioHLECommand command)
{
	#ifdef DEBUG_AUDIO
		DBGConsole_Msg(0, "MIXER3");
		#endif
  // Needs accuracy verification...
	auto dmemin  {(u16)(command.cmd1 >> 0x10)  + 0x4f0};
	auto dmemout {(u16)(command.cmd1 & 0xFFFF) + 0x4f0};
	//u8  flags   = (u8)((command.cmd0 >> 16) & 0xff);
	auto gain   {(s16)(command.cmd0 & 0xFFFF)};

	gAudioHLEState.Mixer( dmemout, dmemin, gain, 0x170 );		// NB - did mult gain by 2 above, then shifted by 16 inside mixer.
}
