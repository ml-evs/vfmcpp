vfmcpp
==============
vfmcpp is a C++ implementation of the vortex filament model of superfluids. 

The vague aim of this project is to model vortex ring dynamics in various configurations - including charged filaments immersed in trivial electric fields and open 'line' filaments. 

The code makes use of custom linked lists (with nasty bare pointers) to allow for reconnections to be modelled efficiently. Visualisation is currently performed by a 3D matplotlib script (requires latest matplotlib version), which leaves a lot to be desired. 

Aftermath of off-centre ring collision.

![alt text](http://charmedxi.co.uk/vfmcpp/img/double_rec.gif "Post double reconnection")


A simple off-centre leapfrog with no reconnection.


![alt text](http://charmedxi.co.uk/vfmcpp/img/leapfrog.gif "Simple leapfrog")
