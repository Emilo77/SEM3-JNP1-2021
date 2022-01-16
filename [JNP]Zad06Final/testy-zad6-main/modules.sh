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

function run {
	rm -rf "src"
	mkdir "src"
	compile_file_as_module "$1" "$2"

	"./src/rover_example"
	code=$?
    
	if [ $code -eq 0 ]; then
		test_ok
		return 0
	else
		test_bad
		echo "Jakiś assert się wykrzaczył"
		exit 1
	fi
	rm -rf "src"
}

function run_all_tests {
    for cc_file in $(ls *.cc); do
		name=${cc_file%.cc}
		echo -n "$name "
		run "$1" "$name"
    done
}


function compile_file_as_module {
	cp -f ../*.cc ../*.cxx ../*akefile src/
	cp -f $2.cc src/rover_example.cc

	sed -i 's/\#include \"..\/rover.h\"/#include <iostream>\n import rover;/' src/rover_example.cc

	cd src
	if ! make modules; then
		printf "${RED}Makefile się popsuł${DEFAULT}"
		exit 1
	fi
	cd ..
}

# Check for compiler
if [ -z "$1" ]; then
	echo "Nie dano kompilatora, używam /opt/gcc-11.2/bin/g++-11.2"
	compiler="/opt/gcc-11.2/bin/g++-11.2"
else
	compiler=$1
fi

# redirecting unwanted error messages
run_all_tests "$compiler" 

