#!/bin/zsh

red='\e[1;37m'
blu='\e[1;34m'
yel='\e[1;33m'
gre='\e[0;36m'


usage() {
cat <<EOF

#########################################################################
#                                                                       #
#                                                                       #
#            ██╗   ██╗███████╗███╗   ███╗ ██████╗██████╗ ██████╗        #
#            ██║   ██║██╔════╝████╗ ████║██╔════╝██╔══██╗██╔══██╗       #
#            ██║   ██║█████╗  ██╔████╔██║██║     ██████╔╝██████╔╝       #
#            ╚██╗ ██╔╝██╔══╝  ██║╚██╔╝██║██║     ██╔═══╝ ██╔═══╝        #
#             ╚████╔╝ ██║     ██║ ╚═╝ ██║╚██████╗██║     ██║            #
#              ╚═══╝  ╚═╝     ╚═╝     ╚═╝ ╚═════╝╚═╝     ╚═╝            #
#                                                                       #
#               * * *            vfmcpp            * * *                #
#                                                                       #
#                         vortex filament code                          #
#                      Rory Brown & Matthew Evans                       #
#                                                                       #
#########################################################################

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


EOF
}

COMPILE=0
DEBUG=0

no=$#

cwd=$(pwd)

while true
do
	case "$3" in
		-c|--compile) COMPILE=1; shift;;
		-h|--help) usage; shift;;
		-d|--debug) DEBUG=1; shift;;
		-*) echo "invalid flag, showing help"; usage; shift;; 
		*) in=$3; break;;	
	esac
done

while true
do
	case "$2" in
		-c|--compile) COMPILE=1; shift;;
		-h|--help) usage; shift;;
		-d|--debug) DEBUG=1; shift;;
		-*) echo "invalid flag, showing help"; usage; shift;; 
		*) in=$2; break;;	
	esac
done

while true
do
	case "$1" in
		-c|--compile) COMPILE=1; shift;;
		-h|--help) usage; shift;;
		-d|--debug) DEBUG=1; shift;;
		-*) echo "invalid flag, showing help"; usage; shift;; 
		*) in=$1; break;;	
	esac
done

if [ ! -f "$in" ]; then
	echo "given input file does not exist, exiting..."
	exit 1
fi


	dir=$(grep -v '^#' $in | grep 'path')
	dir=${dir:5}
	echo $dir
	
if [ -d "$dir" ]; then
	echo "data directory found, cleaning up...\n\n"
	cd "$dir"
	rm '*.dat'
	cd $cwd
fi

if [ ! -d "$dir" ]; then
	echo "data directory not found, creating...\n\n"
	mkdir "$dir"
fi



if [ $COMPILE -eq 1 ]; then
	cd "src"
	echo " ${blu} -c flag specified, recompiling source...${red}"
	make
	echo " ${blu} success!\n\n"
	cd ".."
fi



cd "bin"

echo "  ${gre}#########################################################################"
echo "  ${gre}#                                                                       #"
echo "  ${gre}#                                                                       #"
echo "  ${gre}#         ${blu} ██╗   ██╗███████╗███╗   ███╗ ██████╗██████╗ ██████╗          ${gre}#"
echo "  ${gre}#         ${blu} ██║   ██║██╔════╝████╗ ████║██╔════╝██╔══██╗██╔══██╗         ${gre}#"
echo "  ${gre}#         ${blu} ██║   ██║█████╗  ██╔████╔██║██║     ██████╔╝██████╔╝         ${gre}#"
echo "  ${gre}#         ${blu} ╚██╗ ██╔╝██╔══╝  ██║╚██╔╝██║██║     ██╔═══╝ ██╔═══╝          ${gre}#"
echo "  ${gre}#         ${blu}  ╚████╔╝ ██║     ██║ ╚═╝ ██║╚██████╗██║     ██║              ${gre}#"
echo "  ${gre}#         ${blu}   ╚═══╝  ╚═╝     ╚═╝     ╚═╝ ╚═════╝╚═╝     ╚═╝              ${gre}#"
echo "  ${gre}#                                                                       #"
echo "  ${gre}#               ${yel}* * *            vfmcpp            * * *         ${gre}       #"
echo "  ${gre}#                                                                       ${gre}#"
echo "  ${gre}#                 ${yel}        vortex filament code                          ${gre}#"
echo "  ${gre}#                 ${yel}     Rory Brown & Matthew Evans                       ${gre}#"
echo "  ${gre}#                                                                       ${gre}#"
echo "  ${gre}#########################################################################\n${red}"


if [ $DEBUG -eq 1 ]; then
	gdb -ex run --args vfmcpp "../$in"
fi

if [ ! $DEBUG -eq 1 ]; then
	./vfmcpp "../$in"
fi
