#!/bin/bash

GREEN="\e[1;32m"
RED="\e[1;31m"
DEFAULT="\e[0m"

function test_ok {
    printf "${GREEN}OK${DEFAULT}\n"
}

function test_bad {
    printf "${RED}WRONG${DEFAULT}\n"
}

function remove_minus_zeroes_from_textfile {
	sed -i -E -r ':a;N;$!ba;s/-0([^.])/0\1/g' "$1"
}

function run {
	g++ -Wall -Wextra -O2 -std=c++20 -lm $1.cc
	if [ ! $? == 0 ]; then
		exit 1
	fi
    "./a.out" > m.out
    
	if [ ! -f "$1.out" ]; then
		cp m.out "$1.out"
		remove_minus_zeroes_from_textfile "$1.out"
	fi

	remove_minus_zeroes_from_textfile "m.out"
	if ! diff m.out "$1.out" > /dev/null; then
		echo -n "wrong .out "
		test_bad
	else
		test_ok
	fi
}

function cleanup {
	rm "a.out"
	rm "m.out"
}

function run_all_tests {
    for cc_file in *.cc; do
	    name=${cc_file%.cc}
	    echo -n "$name: "
		run "$name"
		cleanup
    done
}

run_all_tests

