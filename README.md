vfmcpp
==============
vfmcpp is a C++ implementation of the vortex filament model of superfluids. 

To compile in Linux, you can use the makefile `make`; it is advised you use GCC 4.7+, as that is all that has been tested. 
Help with initial conditions can be found by running `run.sh` with the flag `-h`. 

The vague aim of this project is to model vortex ring dynamics in various configurations - including charged filaments immersed in trivial electric fields and open 'line' filaments. 

The code makes use of custom linked lists (with nasty bare pointers) to allow for reconnections to be modelled efficiently. Visualisation is currently performed by a 3D matplotlib script (requires latest matplotlib version), which leaves a lot to be desired. 

Aftermath of off-centre ring collision.

![alt text](http://www.charmedxi.co.uk/vfmcpp/img/offset_collision.gif "Post double reconnection")

![alt text](http://giant.gfycat.com/AmbitiousPlushBetafish.gif "4 ring reconnection")

![alt text](http://giant.gfycat.com/ScratchyCorruptIbizanhound.gif "Distorted string colliding with ring")

![alt text](http://giant.gfycat.com/ZigzagDelightfulBuzzard.gif "Highly distorted string colliding with ring")


run.sh
---------

DESCRIPTION:

Run the vfmcpp code.

USAGE:

	run.sh [OPTIONS] [FILENAME]

OPTIONS:        

	-c
		Recompile the code.
	-d
		Run the code inside the gdb debugger.
	-h
		Show usage information.

	Flags cannot be combined.

FILENAME:

	The initial condition file can be specified; defaults to "run.in".

INITIAL CONDITIONS: 

	path 
		data folder path relative to vfmcpp root folder (will be created if doesn't exist or emptied if exists)
	
	time	
		total length of time to simulate
	
	resl	
		specify the resolution of the simulation in metres, filaments will be created, default=6.28e-8 (1um radius ring w/ 100 pts) 

	ring [radius, x, y, z]
		make a closed filament, requires 4 arguments as above

	line [length, x, y, z]
		make an open filament, requires 4 arugments as above (easy to extend to any direction if we want to)

	Eext [strength, duration, direction]
		include an external electric field, requires 3 arguments where direction is either 0, 1 or 2 for x, y or z.

	q_pt [filament, point, magnitude]
		add a charge to a filament, requires filaments to be specified eariler in file, 
		filament is the relative position of the filament in the .in file, (first filament = 0), 
		point is the index of the desired charged point on the filament, and magnitude is the size of the charge.

	#
		indicates a comment, which will be ignored by vfmcpp

EXAMPLE FILE:
	
	path data/init_example
	time 1e-3
	resl 6.28e-8
	ring 1e-6 0 0 5e-6
	ring 9e-7 0 0.025e-6 0
	Eext 10000 1e-3 0 
	q_pt 0 50 1.6e-19 
