#!/bin/sh
for lz77 in einstein sources world_leaders cere
do
    for M in 2 3 4 5 10
    do
        echo "$lz77 $M 6t"
        echo ""
        ./locate patterns/$lz77/$M/patterns.txt $M 1000 his/$lz77/$lz77 6 $lz77
        echo ""
        echo "====================================================================================="
        echo "$lz77 $M 4t"
        echo ""
        ./locate patterns/$lz77/$M/patterns.txt $M 1000 his/$lz77/$lz77 4 $lz77
        echo ""
        echo "====================================================================================="
    done
done