# DaedalusX64

DaedalusX64 is a Nintendo 64 emulator for Linux and PSP, with plans to port to Windows, Mac, PS Vita among other platforms. The PSP port is noteworthy for being the fastest N64 emulator ever on the platform, achieving close to or even full speed in many titles.

## Features:

- Fast emulation
- High compatibility
- Support for PSP TV Mode with 480p output!
- Active support and updates
- Lots of experimental optimizations

And more!

## Usage

Download the latest release from the Releases page: https://github.com/DaedalusX64/daedalus/releases for platform of your choice.
All consoles require access to run homebrew, please consult our Discord or a PSP Scene / PSVita Scene Website.

PSP:
Copy the entire DaedalusX64 Folder to PSP/Game on your PSP Memory Stick.
Roms go into the ROM directory.

PSVita:
Copy the Daedalus.vpk file and install.
Roms go into Data/DaedalusX64/Roms

Others: Roms Directory

## Compilation
Note: No support is offered on compilation and should be referred to the appropriate channels on websites if applicable.

PSP - PSP Toolchain from https://github.com/top-sekret/psptoolchain, follow directions there, you will require libpng separately.

PSVita - VitaSDK from https://vitasdk.org - Follow instructions there.
 		Dependancies: https://github.com/Rinnegatamante/vitaGL, makefile needs to be modified to include -DTRANSPOSE_MATRICES under CFLAGS; make HAVE_SBRK=1 install
			      https://github.com/Rinnegatamante/math-neon standard make / install
            https://github.com/Rinnegatamante/imgui-vita standard make / install
macOS - Requires Xcode installed (Note: Does not work on macOS versions higher than High Sierra)
		Dependancies: zlib / libpng, obtainable via homebrew


General: Ensure build_daedalus.sh is executable (chmod +x build_daedalus.sh) then the following:
	 ./build_daedalus PLATFORM_RELEASE or PLATFORM_DEBUG

This will generate the relevant files / folders for your platform, do not delete any of these files / folders as the emulator may stop functioning.



## More Info

For information about compatibility, optimal settings and more about the emulator, visit the actively maintained GitHub wiki page: https://github.com/DaedalusX64/daedalus/wiki Feel free to submit reports for how well your favourite games run if they have not already been listed!

Join our Discord server to talk to other Daedalus users and the developers!

Invite link: https://discord.gg/FrVTpBV

## Credits

- kreationz, salvy6735, Corn, Chilly Willy: Original DaedalusX64 code
- Wally: Optimizations, improvements and ports
- z2442: Compilation improvements and updating, optimizations
- mrneo240: Optimizations, compilation help
- TheMrIron2: Optimizations, wiki maintenance
- Rinnegatamante: PSVita Port, optimisations.
