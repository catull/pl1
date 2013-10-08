#!/usr/local/bin/bash

plcmp/bin/plcmp prog/${1}.pli && 
asmcmp/bin/asmcmp prog/${1}.ass && 
printf "prog/${1}.tex" > prog/${1}.mod && 
absload/bin/absload prog/${1}.mod
