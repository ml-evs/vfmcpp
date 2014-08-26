#!/bin/zsh

usage() {
cat <<EOF
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

EOF
}

COMPILE=0
DEBUG=0

no=$#

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



red='\e[1;37m'
blu='\e[1;34m'
yel='\e[1;33m'
gre='\e[0;36m'



dir=$(grep "path" $in)
dir=${dir:5}

if [ ! -d "$dir" ]; then
	echo "data directory not found, creating...\n\n"
	mkdir "$dir"
fi

if [ -d "$dir" ]; then
	echo "data directory found, cleaning up...\n\n"
	cd "$dir"
	rm data*
	cd "../.."
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
