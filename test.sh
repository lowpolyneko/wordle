#!/bin/bash
make -s
OUTPUT=$(cat << EOF
34 adapt
34 leant
33 clapt
32 trait
31 adept
27 cleft
24 abuts
16 clear
14 tried
12 darns
7 sours
Top scoring words:
adapt 34
leant 34

EOF
)

echo "3 answersTiny.txt guessesTiny.txt 1" | ./main

diff -w <(echo "3 answersTiny.txt guessesTiny.txt 1" | ./main) <(printf "%s" "$OUTPUT")
