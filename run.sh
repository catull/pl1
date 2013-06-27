#!/bin/bash

bin/plcmp prog/${1}.pli && 
bin/asmcmp prog/${1}.ass && 
printf "prog/${1}.tex" > prog/${1}.mod && 
bin/absload prog/${1}.mod
