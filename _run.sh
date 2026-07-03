#!/bin/bash
cd "$(dirname "$0")" || exit
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
./nidus.out -i init.txt --serve 5200
