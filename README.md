vfmcpp
==============
vfmcpp is a C++ implementation of the vortex filament model of superfluids. 

To compile in Linux, you can use the makefile `make`; it is advised you use GCC 4.7+, as that is all that has been tested. 
Help with initial conditions can be found by running `run.sh` with the flag `-h`. 

The vague aim of this project is to model vortex ring dynamics in various configurations - including charged filaments immersed in trivial electric fields and open 'line' filaments. 

The code makes use of custom linked lists (with nasty bare pointers) to allow for reconnections to be modelled efficiently. Visualisation is currently performed by a 3D matplotlib script (requires latest matplotlib version), which leaves a lot to be desired. 

Aftermath of off-centre ring collision.

![alt text](http://charmedxi.co.uk/vfmcpp/img/double_rec.gif "Post double reconnection")
