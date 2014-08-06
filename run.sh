#!/bin/zsh



COMPILE=0

while true
do
	case "$2" in
		-c|--compile) COMPILE=1; shift;;
		*) in=$2; break;;
	esac
done

while true
do
	case "$1" in
		-c|--compile) COMPILE=1; shift;;
		*) in=$1; break;;
	esac
done


echo $in


if [ $# -eq 0 ]; then
	echo "no arguments provided, input file defaulting to run.in"
	in="run.in"
fi

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

if [ ! -d "bin/$dir" ]; then
	echo "data directory not found, creating...\n\n"
	mkdir "bin/$dir"
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

./vfmcpp "../$in"