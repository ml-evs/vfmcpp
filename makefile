# VFMCPP makefile - several versions for different compilers/architectures

# no enforced vectorisation
g++ : src/filament.h src/tangle.h src/point.h
	mkdir -p bin
	g++ -Wall -std=c++11 -pg -O3 src/main.cpp src/init.cpp src/vel.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/velnl.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/vfmcpp

# vectorisation enforced in velnlcpp
g++vec : src/filament.h src/tangle.h src/point.h
	mkdir -p bin/chmarking/gpp_autovec
	g++ -Wall -std=c++11 -O3 -pg src/main.cpp src/init.cpp src/vel.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/velnl_AVX.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/chmarking/gpp_autovec/vfmcpp

# default intel compilation with i7-4770 compatible optimisations but no enforced vectorisation
icpc : src/filament.h src/tangle.h src/point.h
	mkdir -p bin
	/opt/intel/bin/icpc -xCORE-AVX2 -std=c++11 -O3 src/main.cpp src/init.cpp src/vel.cpp src/velnl.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/vfmcpp

# auto-vectorisation on, profiling on, vectorisation reports on
icpc-autovec : src/filament.h src/tangle.h src/point.h
	mkdir -p bin/chmarking/icpc-autovec
	/opt/intel/bin/icpc -xCORE-AVX2 -std=c++11 -O3 src/main.cpp src/init.cpp src/vel.cpp src/velnl_AVX.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/chmarking/icpc-autovec/vfmcpp

# profiler icpc for benchmarking
icpcprof : src/filament.h src/tangle.h src/point.h
	mkdir -p bin/chmarking/icpc
	/opt/intel/bin/icpc -xCORE-AVX2 -pg -std=c++11 -O3 src/main.cpp src/init.cpp src/vel.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/velnl.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/chmarking/icpc/vfmcpp

g++prof : src/filament.h src/tangle.h src/point.h
	mkdir -p bin/chmarking/gpp
	g++ -Wall -std=c++11 -O3 src/main.cpp src/init.cpp src/vel.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/velnl.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/chmarking/gpp/vfmcpp
