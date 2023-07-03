#!/bin/bash

if [ "$#" != 2 ]; then
	echo "Użycie: $0 <binarka> <folder>"
	exit 1
fi

BINARKA="$1"
FOLDER="$2"

PROG_OUT="$(mktemp)"
PROG_ERR="$(mktemp)"

for test_in in $FOLDER/*.in; do
	test_nazwa=${test_in%".in"}
	test_out="$test_nazwa.out"
	test_err="$test_nazwa.err"

	valgrind -q --error-exitcode=123 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
		"./$BINARKA" < "$test_in" > "$PROG_OUT" 2> "$PROG_ERR"
	# "./$BINARKA" < "$test_in" > "$PROG_OUT" 2> "$PROG_ERR"

	return_value=$?
	if [[ "$return_value" == 123 ]]; then
		echo "$test_nazwa: wyciek pamięci"
	elif [[ "$return_value" != 0 && "$return_value" != 1 ]]; then
		echo "$test_nazwa: segmentation fault"
	elif diff "$test_out" "$PROG_OUT" > /dev/null && diff "$test_err" "$PROG_ERR" > /dev/null; then
		echo "$test_out: OK"
	else
		echo "$test_nazwa: zła odpowiedź"
		exit 1
	fi
done

rm "$PROG_OUT"
rm "$PROG_ERR"
