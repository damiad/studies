#!/bin/bash

no_color='\033[0m' # No Color
green='\033[0;92m' # Green color for successful examples
red='\033[0;91m'   # Red color for failed examples

timeout=2 # Timeout duration in seconds

run_examples() {
    echo -e "--------------------------- ${4}${1} examples (expected ${2}to yield an error)${no_color} ---------------------------"
    for file in examples/${3}/*.hml; do
        echo -e "Running $file...${4}"
        timeout $timeout ./interpreter "$file"
        [ $? -eq 124 ] && echo "timeout!" # Checking for timeout
        echo -e "${no_color}"
    done
}

run_examples "Good" "NOT " "good" "$green" # Run good examples
run_examples "BAD" "" "bad" "$red" # Run bad examples
