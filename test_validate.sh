#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_validate.sh                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: student <student@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/26 00:00:00 by student           #+#    #+#              #
#    Updated: 2025/01/26 00:00:00 by student          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colours for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_result="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}Test $TOTAL_TESTS: $test_name${NC}"
    
    # Run the command and capture output
    if eval "$command" >/dev/null 2>&1; then
        result="success"
    else
        result="failure"
    fi
    
    # Check if result matches expected
    if [ "$result" = "$expected_result" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAIL (expected $expected_result, got $result)${NC}"
    fi
    echo
}

# Function to test syntax validation
test_syntax_validation() {
    echo -e "${YELLOW}=== Testing Syntax Validation ===${NC}\n"
    
    # Valid syntax tests
    run_test "Valid pipe command" "echo 'echo hello | grep hello' | ./minishell -c" "success"
    run_test "Valid logical AND" "echo 'echo hello && echo world' | ./minishell -c" "success"
    run_test "Valid logical OR" "echo 'echo hello || echo world' | ./minishell -c" "success"
    run_test "Valid parentheses" "echo '(echo hello && echo world) | cat' | ./minishell -c" "success"
    
    # Invalid syntax tests
    run_test "Invalid trailing AND" "echo 'echo hello &&' | ./minishell -c" "failure"
    run_test "Invalid leading pipe" "echo '| echo hello' | ./minishell -c" "failure"
    run_test "Invalid trailing OR" "echo 'echo hello ||' | ./minishell -c" "failure"
    run_test "Unmatched parentheses" "echo '(echo hello' | ./minishell -c" "failure"
}

# Function to test complex commands
test_complex_commands() {
    echo -e "${YELLOW}=== Testing Complex Commands ===${NC}\n"
    
    run_test "Pipe with logical AND" "echo 'echo hello | grep hello && echo found' | ./minishell -c" "success"
    run_test "Parentheses with pipe" "echo '(echo hello && echo world) | wc -l' | ./minishell -c" "success"
    run_test "Multiple pipes" "echo 'echo hello | cat | wc -l' | ./minishell -c" "success"
    run_test "Complex precedence" "echo 'echo a | grep a && echo found || echo notfound' | ./minishell -c" "success"
}

# Function to test precedence validation
test_precedence() {
    echo -e "${YELLOW}=== Testing Precedence Validation ===${NC}\n"
    
    # Precedence: () > | > && > ||
    run_test "Pipe before AND" "echo 'echo hello | grep hello && echo found' | ./minishell -c" "success"
    run_test "Parentheses override" "echo '(echo hello && echo world) | cat' | ./minishell -c" "success"
    run_test "OR lowest precedence" "echo 'echo a && echo b || echo c' | ./minishell -c" "success"
}

# Function to test error handling
test_error_handling() {
    echo -e "${YELLOW}=== Testing Error Handling ===${NC}\n"
    
    run_test "Empty command" "echo '' | ./minishell -c" "success"
    run_test "Only spaces" "echo '   ' | ./minishell -c" "success"
    run_test "Multiple operators" "echo 'echo test && && echo fail' | ./minishell -c" "failure"
    run_test "Malformed pipe" "echo 'echo test |||| echo fail' | ./minishell -c" "failure"
}

# Function to compare with bash
compare_with_bash() {
    local command="$1"
    local temp_file_minishell="/tmp/minishell_output"
    local temp_file_bash="/tmp/bash_output"
    
    # Run with minishell
    echo "$command" | ./minishell > "$temp_file_minishell" 2>&1
    minishell_exit=$?
    
    # Run with bash
    echo "$command" | bash > "$temp_file_bash" 2>&1
    bash_exit=$?
    
    # Compare outputs
    if diff "$temp_file_minishell" "$temp_file_bash" >/dev/null 2>&1 && [ $minishell_exit -eq $bash_exit ]; then
        return 0
    else
        return 1
    fi
}

# Function to test bash compatibility
test_bash_compatibility() {
    echo -e "${YELLOW}=== Testing Bash Compatibility ===${NC}\n"
    
    # Test basic commands
    if compare_with_bash "echo hello"; then
        echo -e "${GREEN}✓ Basic echo compatibility${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ Basic echo compatibility${NC}"
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Test pipe commands
    if compare_with_bash "echo hello | cat"; then
        echo -e "${GREEN}✓ Pipe compatibility${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ Pipe compatibility${NC}"
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Test logical operators
    if compare_with_bash "true && echo success"; then
        echo -e "${GREEN}✓ Logical AND compatibility${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ Logical AND compatibility${NC}"
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
}

# Main execution
main() {
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}         MINISHELL VALIDATION TESTS             ${NC}"
    echo -e "${BLUE}               Phase 11.4                      ${NC}"
    echo -e "${BLUE}================================================${NC}\n"
    
    # Check if minishell executable exists
    if [ ! -f "./minishell" ]; then
        echo -e "${RED}Error: minishell executable not found!${NC}"
        echo -e "${YELLOW}Please compile minishell first with 'make'${NC}"
        exit 1
    fi
    
    # Run all test suites
    test_syntax_validation
    test_complex_commands
    test_precedence
    test_error_handling
    test_bash_compatibility
    
    # Final results
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}                 FINAL RESULTS                 ${NC}"
    echo -e "${BLUE}================================================${NC}"
    echo -e "Total tests: $TOTAL_TESTS"
    echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}"
    echo -e "Failed: ${RED}$((TOTAL_TESTS - PASSED_TESTS))${NC}"
    
    if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
        echo -e "\n${GREEN}🎉 ALL TESTS PASSED! Phase 11.4 validation complete!${NC}"
        exit 0
    else
        echo -e "\n${RED}❌ Some tests failed. Please review the implementation.${NC}"
        exit 1
    fi
}

# Run main function
main "$@"
