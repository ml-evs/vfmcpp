vfmcpp
==============
vfmcpp is a C++ implementation of the vortex filament model of superfluids. 

The vague aim of this project is to model vortex ring dynamics in various configurations - perhaps including charged filaments immersed in trivial electric fields. Current functionality is limited to closed filaments, with work being done on open filaments with various boundary conditions. Detailed documentation will be added inside `src/` shortly.

The code makes use of custom linked lists (with nasty bare pointers) to allow for reconnections to be modelled efficiently. Visualisation is currently performed by a 3D matplotlib script (requires latest matplotlib version), which leaves a lot to be desired. 

### To-do and possible directions

- Open filaments ('strings') with either dummy, curvature-preserving endpoints, image vortices or fixed ends.
- Charged filaments, initially with electrons fixed to particular mesh points, but hopefully eventually with the ability to move along the filament.
- Better visualisation, either using VTK or the like with Python, or by falling back to existing MATLAB work.
- Dynamically scaling mesh size during 'interesting' dynamics, i.e. perform reconnections at much higher resolution.
- If work moves onto larger systems, perhaps consider parallelising the code with MPI.
- Write better documentation!
- ...Probably many more.
