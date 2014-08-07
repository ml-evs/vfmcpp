#!/bin/zsh

red='\e[1;37m'
blu='\e[1;34m'
yel='\e[1;33m'
gre='\e[0;36m'

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

start=0.08
inc=0.0025

for i in {0..4}
do
	sig=$(echo "scale = 4; $start+$i*$inc" |bc)
	sig="0$sig"
 	echo $sig
	if [ -d "$sig" ]; then
		echo "data directory exists, exiting...\n\n"
		exit 1
	fi
	
	mkdir $sig
	cd $sig
	echo "path data/sweep_0.9/$sig" >> $sig.in
	echo "time 1e-3" >> $sig.in
	echo "N_pt 100" >> $sig.in
	echo "ring 1e-6 0 0 5e-6" >> $sig.in
	echo "ring 0.9e-6 0 ${sig}e-6" >> $sig.in
	cd ..
	cd ../../bin
	./vfmcpp ../data/sweep_0.9/$sig/$sig.in
	cd ../data/sweep_0.9
done

echo "	${blu} !!!!!!!!!!! CORE 0 COMPLETE !!!!!!!!!!!"