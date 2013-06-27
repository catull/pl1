#!/bin/bash

bin/plcmp prog/${1}.pli && bin/asmcmp prog/${1}.ass && bin/absload prog/${1}.mod
