#!/bin/sh
g++ -std=c++1z -O2 -Wall $1 -lstdc++fs
./a.out $2
#echo "TECHIO> terminal -i ./a.out"
#sleep 60
