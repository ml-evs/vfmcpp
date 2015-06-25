g++ : src/filament.h src/tangle.h src/point.h
	mkdir -p bin
	g++ -Wall -std=c++11 -O3 src/main.cpp src/init.cpp src/vel.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/velnl.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/vfmcpp

icpcprof : src/filament.h src/tangle.h src/point.h
	mkdir -p bin/chmarking/icpc
	/opt/intel/bin/icpc -xAVX -std=c++11 -pg -O3 src/main.cpp src/init.cpp src/vel.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/velnl.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/chmarking/icpc/vfmcpp

icpc : src/filament.h src/tangle.h src/point.h
	mkdir -p bin
	/opt/intel/bin/icpc -xAVX -std=c++11 -O3 src/main.cpp src/init.cpp src/vel.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/velnl.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/vfmcpp

g++prof : src/filament.h src/tangle.h src/point.h
	mkdir -p bin/chmarking/gpp
	g++ -Wall -std=c++11 -pg -O3 src/main.cpp src/init.cpp src/vel.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/velnl.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/chmarking/gpp/vfmcpp

icpc-autovec : src/filament.h src/tangle.h src/point.h
	mkdir -p bin/chmarking/icpc-autovec
	/opt/intel/bin/icpc  -xAVX -std=c++11 -O3 -qopt-report src/main.cpp src/init.cpp src/vel.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/velnl.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/chmarking/icpc-autovec/vfmcpp