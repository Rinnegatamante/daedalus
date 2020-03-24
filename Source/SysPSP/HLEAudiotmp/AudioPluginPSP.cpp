/*
Copyright (C) 2003 Azimer
Copyright (C) 2001,2006 StrmnNrmn

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
//	N.B. This source code is derived from Azimer's Audio plugin (v0.55?)
//	and modified by StrmnNrmn to work with Daedalus PSP. Thanks Azimer!
//	Drop me a line if you get chance :)
#include "stdafx.h"

#include <pspkernel.h>

#include "AudioPluginPSP.h"
#include "HLEAudio/audiohle.h"

#include "Config/ConfigOptions.h"
#include "Core/CPU.h"
#include "Core/Interrupt.h"
#include "Core/Memory.h"
#include "Core/ROM.h"
#include "Core/RSP_HLE.h"
#include "SysPSP/Utility/JobManager.h"

#define RSP_AUDIO_INTR_CYCLES     1
static const u32 kAudioBufferSize = 1024 * 2; // 1024 * 1024 is OSX

EAudioPluginMode gAudioPluginEnabled( APM_ENABLED_ASYNC ); // Vita Detection needs to go here
/* if (VITAwhatever)
		EAudioPluginMode gAudioPluginEnabled (APM_ENABLED_SYNC);
		*/
		idnt audioThread = sceKernelCreateThread("audioOutput", audioOutput, 0x15, 0x1800, PSP_THREAD_ATTR_USER, NULL);

class AudioPluginPSP : public CAudioPlugin
{
	AudioPluginPSP();
	virtual ~AudioPluginPSP();

	virtual bool StartEmulation();
	virtual void StopEmulation();

	virtual void DacrateChanged(int system_type);
	virtual void LenChanged();
	virtual u32 ReadLength() { return 0; }
	virtual EProcessResult ProcessAList();

	void AddBuffer(void * ptr, u32 length); // Upload new buffer and return status

	void StopAudio();
	void StartAudio();

private:
		CAudioBuffer mAudioBuffer;
		u32 mFrequency;
		volatile bool mKeepRunning;
		volatile u32 mBufferLenMs;
};

AudioPluginPSP::AudioPluginPSP()
: mAudioBuffer(kAudioBufferSize)
, mFrequency(44100)
, mKeepRunning (false)
, mBufferLenMs(0)
{}

	AudioPluginPSP::~AudioPluginPSP() {StopAudio};
	bool AudioPluginPSP::StartEmulation { return true };

	void AudioPluginPSP::StopEmulation() {StopAudio();}

	void AudioPluginPSP::DacrateChanged(int system_type)
	{
		u32 clock = (system_type == ST_NTSC) ? VI_NTSC_CLOCK : VI_PAL_CLOCK;
		u32 dacrate = (Memory_AI_GetRegister(AI_DACRATE_REG);
		u32 Frequency = clock / (dacrate + 1);
		DBGConsole_Msg(0, "Audio Frequency: %d", frequency);
		mFrequency = frequency;
			}

	void AudioPluginPSP::LenChanged()
	{
		if (gAudioPluginEnabled > APM_DISABLED)
		{
		u32 address = Memory_AI_GetRegister(AI_DRAM_ADDR_REG) & 0xFFFFFF;
		u32 length = Memory_AI_GetRegister(AI_LEN_REG);

		AddBuffer (g_pu8RamBase + address, length);
	}
	else
	{
		StopAudio();
	}
}

void AudioPluginPSP::ProcessAList()
	{
		Memory_SP_SetRegisterBits(SP_STATUS_REG, SP_STATUS_HALT);

		EProcessResult result = PR_NOT_STARTED;

		switch (gAudioPluginEnabled)
		{
		case APM_DISABLED;
				result = PR_COMPLETED;
				break;
		case APM_ENABLED_ASYNC:
			 	SHLEStartJob	job;
				gJobManager.AddJob( &job, sizeof( job ) );
			 	result = PR_STARTED;
				break;
		case APM_ENABLED_SYNC:
				AudioUcode();
				result = PR_COMPLETED;
				break;
		}
	return result;
 }

	void AudioPluginPSP::StopAudio()
	{
		mKeepRunning = false;

	}

	void AudioPluginPSP::StartAudio()
	{
		mKeepRunning = true;

	}
//bool gAdaptFrequency( false );


void AudioPluginPSP::AddBuffer(void * ptr, u32 length)
{
	if (length == 0)
		return;
if(!mAudioPlaying) // Probably neds to be more logical than this
	StartAudio();

		u32 num_samples = length / sizeof (Sample);

		mAudioBuffer.AddSamples( reinterpret_cast<const Sample *>(ptr), num_samples, mFrequencym kOutputFrequency);
		u32 remaining_samples = mAudioBuffer.GetNumBufferedSamples();
		mBufferLenMs = (1000 * remaining_samples) / kOutputFrequency;
		#ifdef DAEDALUS_DEBUG_CONSOLE
		float ms = (float)num_samples * 1000.f / (float)mFrequency;
		DPF_AUDIO("Queuing %d samples @%dhZ - %2fms - bufferlen now %d\n",
			num_samples, mFrequency, ms, mBufferLenMs);
		#endif
}

struct SHLEStartJob : public SJob
{
	SHLEStartJob()
	{
		 InitJob = NULL;
		 DoJob = &DoHLEStartStatic;
		 FiniJob = &DoHLEFinishedStatic;
	}

	static int DoHLEStartStatic( SJob * arg )
	{
		 SHLEStartJob *  job( static_cast< SHLEStartJob * >( arg ) );
		 return job->DoHLEStart();
	}

	static int DoHLEFinishedStatic( SJob * arg )
	{
		 SHLEStartJob *  job( static_cast< SHLEStartJob * >( arg ) );
		 return job->DoHLEFinish();
	}

	int DoHLEStart()
	{
		 Audio_Ucode();
		 return 0;
	}

	int DoHLEFinish()
	{
		 CPU_AddEvent(RSP_AUDIO_INTR_CYCLES, CPU_EVENT_AUDIO);
		 return 0;
	}
};




CAudioPlugin * CreateAudioPlugin()
{
		return new AudioPluginPSP();
}
