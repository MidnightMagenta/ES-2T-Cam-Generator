# ES 2-stroke Camshaft Generator

## Note

Windows Defender has a tendency to flag unsigned binaries as "trojan" or "serious threats"

Because this project is open source, not commercial, and I make $0 from it, I cannot justify purchasing or maintaining a code signing certificate. 

To avoid giving users a false sense of insecurity and damaging my reputation, as well as the reputation of the project, I will not be publishing prebuilt binaries

If you want to use this software, please see the [How to build](#how-to-build) section for instructions on building from source.

## Description

A small CLI tool for generating camshafts used to replicate the physical characteristics of 2 stroke engines, in AngeTheGreat's Engine Simulator.

## How to use

The program may be used in two ways. CLI, or Interactive.

### Using the program in CLI mode

To use the program in this mode, it must be extracted into a folder that is contained in the PATH enviromental variable, or the full path to the program must be specified when calling it.

To generate camshafts, use `escamgen generate`. `escamgen generate --help` may be used to view available options.

### Using the program in interactive mode

To use the program in this mode, either call `escamgen generate --interactive` from the command line. The program will ask you to specify the following parameters:

 - Stroke - engine stroke in milimeters
 - Rod length - the length of the connecting rod in milimeters
 - Exhaust port height - the height of the exhaust port in milimeters
 - Exhaust port offset - the offset of the exhaust port in milimeters, relative to the top of the cylinder
 - Transfer port height - the height of the transfer port in milimeters
 - Transfer port offset - the offset of the transfer port in milimeters, reelative to the bottom of the cylinder
 - Step count - the number of steps to calculate. The number of generated samples per camshaft lobe will be (360 / stepCount) * 2 + 1
 - Output path - a path to a regular file (e.g. `path/to/file.txt`). If a file already exists, the program will ask whether you want to overwrite it.

 ## How to build

 To build the program, you must have [cmake](https://cmake.org/download/) installed. You must also have a compiler installed (for example, [MSVC](https://visualstudio.microsoft.com/downloads/) for Windows)
 
 From any directory, call `git clone https://github.com/MidnightMagenta/ES-2T-Cam-Generator.git`
 
 From the directory into which the repository was cloned, call 

 ```
 mkdir build
 cd build
 cmake ..
 cmake --build .

 ```
