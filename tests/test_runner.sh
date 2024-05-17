#!/bin/bash

# Array of test files
tests=("test1.calc" "test2.calc" "test3.calc" "test4.calc", "test5.calc", "test6.calc")

# Path to the compiled interpreter
interpreter="../src/interpreter"

# Directory of expected output files
expected_output_dir="./output"

# Run each test
for test in "${tests[@]}"; do
    echo "Running test: $test"
    test_name=$(basename "$test" .calc)
    expected_output_file="$expected_output_dir/$test_name.out"
    
    # Run the interpreter with the test input file and capture the output
    actual_output=$($interpreter "$test")
    
    # Display the actual output
    echo "Actual Output:"
    echo "$actual_output"
    
    # Read the expected output
    if [ -f "$expected_output_file" ]; then
        expected_output=$(cat "$expected_output_file")
    else
        expected_output=""
        echo="nothing"
    fi
    
    # Display the expected output
    echo "Expected Output:"
    echo "$expected_output"
    
    # Compare the output with the expected output
    if [ "$actual_output" == "$expected_output" ]; then
        echo "Test $test passed!"
    else
        echo "Test $test failed!"
    fi
    
    echo ""
done
