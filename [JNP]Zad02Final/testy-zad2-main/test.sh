#!/bin/bash

runtime_errors=()
wrong_err_errors=()


GREEN="\e[1;32m"
RED="\e[1;31m"
DEFAULT="\e[0m"

function test_ok {
    printf " ${GREEN}OK${DEFAULT}\n"
}

function test_bad {
    printf " ${RED}WRONG${DEFAULT}\n"
}

echo compiling maptel.o
if ! g++ -c -Wall -Wextra -O2 -std=c++17 ../maptel.cc -o maptel.o; then
    echo -ne ": compilation failed"
    test_bad
    exit 1
fi
echo compilation finished

function link {
	if ! g++ maptel.o "$1.o" -o "$1"; then
        echo -ne ": linking error"
        test_bad
        exit 1
    fi
}

function run {
    "./$1" 2>m.err
    code=$?
    
    # exit code of a failed assertion is 134 I think
	if [ $code -eq 134 ]; then
        # tests from the rte family are intended to fail
        if echo "$1" | grep -q -E '^rte[0-9]*$'; then
            test_ok
        else
            echo -n ": runtime error, assertion failed"
            test_bad
            runtime_errors+=("$1")
        fi
	elif echo "$1" | grep -q -E '^rte[0-9]*$'; then
        if [ $code -eq 0 ]; then
            # should get an assertion failure but did not get one
            echo -n ": assertion should have failed"
        else
            # code is neither 0 nor 134 -- some other runtime error
            echo -n ": runtime error, expected an assertion"
        fi
        test_bad
        runtime_errors+=("$1")
    elif [ ! $code -eq 0 ]; then
        echo -n ": runtime error"
        test_bad
        runtime_errors+=("$1")
    else
		if [ ! -f "$1.err" ]; then
			cp m.err "$1.err"
			sed -i -E 's/0x[a-f0-9]{12}/ADDR/g' "$1.err"
		fi
		sed -i -E 's/0x[a-f0-9]{12}/ADDR/g' m.err
		if ! diff m.err "$1.err" > /dev/null; then
			echo -n ": wrong .err"
            test_bad
            wrong_err_errors+=("$1")
		else
            test_ok
        fi
	fi
}

function cleanup {
	rm "$1.o"
	rm "$1"
	rm "m.err"
}

function compilation_error {
    echo -ne ": failed to compile $1"
    test_bad
    exit 1
}

function run_c_test {
	gcc -c -Wall -Wextra -O2 -std=c11 "$1.c" -o "$1.o" ||
        compilation_error "$1.o"
    link "$1"
	run "$1"
	cleanup "$1"
}

function run_cc_test {
    g++ -c -Wall -Wextra -O2 -std=c++17 "$1.cc" -o "$1.o" ||
        compilation_error "$1.o"
	link "$1"
	run "$1"
	cleanup "$1"
}


function run_all_tests {
    for c_file in $(ls *.c -r); do
	name=${c_file%.c}
	echo -n "$name"
	run_c_test "$name"
    done
    
    for cc_file in *.cc; do
	    name=${cc_file%.cc}
	    echo -n "$name"
	run_cc_test "$name"
    done
    
    g++ -c -Wall -Wextra -O2 -std=c++17 przykladowy2.cc -o przykladowy2.o
    g++ przykladowy2.o maptel.o -o przykladowy2
    run przykladowy2
    cleanup przykladowy2
}

# redirecting unwanted error messages
run_all_tests 2>/dev/null

rm maptel.o

if [ ! "${#runtime_errors[@]}" -eq 0 ]; then
    echo
    echo -n All runtime errors:
    test_bad
    for err in "${runtime_errors[@]}"; do
        printf "\t%s\n" "${err}"
    done
else
    echo
    echo -n no runtime errors!
    test_ok
fi

if [ ! "${#wrong_err_errors[@]}" -eq 0 ]; then
    echo
    echo -n All \"wrong err\" errors:
    test_bad
    for err in "${wrong_err_errors[@]}"; do
        printf "\t%s\n" "${err}"
    done
else
    echo
    echo -n No \"wrong err\" errors!
    test_ok
fi


