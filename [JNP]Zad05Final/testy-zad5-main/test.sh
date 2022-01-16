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

function is_substring_in_filename {
	echo "$1" | grep -q -E "*$2*"
}

function run {
	if ! g++ -std=c++20 -Wall -Wextra -O2 $1.cc -o $1; then
		if is_substring_in_filename $1 "ce"; then
			test_ok
			return 0
		else
			test_bad
			echo "Miało się skompilować, a się nie skompilowało"
			exit 1
		fi
	fi

	if is_substring_in_filename $1 "ce"; then
		test_bad
		echo "Miało się nie skompilować, a się skompilowało"
		exit 1
	fi

    "./$1"
    code=$?
    
	if [ $code -eq 0 ]; then
		test_ok
		return 0
	else
		test_bad
		echo "Jakiś assert się wykrzaczył"
		exit 1
	fi
}

function run_all_tests {
    for cc_file in $(ls *.cc); do
		name=${cc_file%.cc}
		echo -n "$name "
		run "$name"
		rm "$name"
    done
}

# redirecting unwanted error messages
run_all_tests 2>/dev/null

