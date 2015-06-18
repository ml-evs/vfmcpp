vfmcpp : src/filament.h src/tangle.h src/point.h
	mkdir -p bin
	g++ -std=c++11 -pg -O3 src/main.cpp src/init.cpp src/vel.cpp src/mesh.cpp src/dummy.cpp src/field.cpp src/tdt.cpp src/velnl.cpp src/mesh_adjust.cpp src/reconnect.cpp src/fromfile.cpp src/loopkiller.cpp -o bin/vfmcpp
