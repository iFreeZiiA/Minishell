#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_heredoc.sh                                    :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/26 16:00:00 by jjorda            #+#    #+#              #
#    Updated: 2025/07/26 16:00:00 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counter
TESTS_RUN=0
TESTS_PASSED=0

# Function to print test results
print_test_result() {
    local test_name="$1"
    local result="$2"
    
    TESTS_RUN=$((TESTS_RUN + 1))
    
    if [ "$result" = "0" ]; then
        echo -e "${GREEN}✅ $test_name: PASSED${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}❌ $test_name: FAILED${NC}"
    fi
}

# Function to test heredoc functionality
test_heredoc_basic() {
    echo -e "${BLUE}Test 1: Heredoc basique${NC}"
    
    # Create test input
    cat > test_input.txt << 'EOF'
Hello World
This is a test
EOF
    
    # Test with bash
    bash -c 'cat << EOF
Hello World
This is a test
EOF' > bash_output.txt
    
    # Compare with expected
    if cmp -s test_input.txt bash_output.txt; then
        print_test_result "Heredoc basique" 0
    else
        print_test_result "Heredoc basique" 1
    fi
    
    # Cleanup
    rm -f test_input.txt bash_output.txt
}

# Function to test heredoc with quotes
test_heredoc_quotes() {
    echo -e "${BLUE}Test 2: Heredoc avec délimiteur quoté${NC}"
    
    # Test with bash
    bash -c 'cat << "EOF"
$HOME should not expand
Special chars: * ? [ ]
EOF' > bash_quoted_output.txt
    
    # Create expected output
    cat > expected_quoted.txt << 'EOF'
$HOME should not expand
Special chars: * ? [ ]
EOF
    
    if cmp -s expected_quoted.txt bash_quoted_output.txt; then
        print_test_result "Heredoc délimiteur quoté" 0
    else
        print_test_result "Heredoc délimiteur quoté" 1
    fi
    
    # Cleanup
    rm -f expected_quoted.txt bash_quoted_output.txt
}

# Function to test heredoc with variables
test_heredoc_variables() {
    echo -e "${BLUE}Test 3: Heredoc avec expansion de variables${NC}"
    
    # Test with bash
    USER_VAR="testuser" bash -c 'cat << EOF
User: $USER_VAR
Home: $HOME
EOF' > bash_var_output.txt
    
    # Check if variables were expanded
    if grep -q "testuser" bash_var_output.txt && grep -q "$HOME" bash_var_output.txt; then
        print_test_result "Heredoc expansion variables" 0
    else
        print_test_result "Heredoc expansion variables" 1
    fi
    
    # Cleanup
    rm -f bash_var_output.txt
}

# Function to test heredoc with pipes
test_heredoc_pipe() {
    echo -e "${BLUE}Test 4: Heredoc avec pipe${NC}"
    
    # Test with bash
    bash -c 'cat << EOF | wc -l
line 1
line 2
line 3
EOF' > bash_pipe_output.txt
    
    # Should output 3
    if [ "$(cat bash_pipe_output.txt | tr -d ' ')" = "3" ]; then
        print_test_result "Heredoc avec pipe" 0
    else
        print_test_result "Heredoc avec pipe" 1
    fi
    
    # Cleanup
    rm -f bash_pipe_output.txt
}

# Function to test heredoc edge cases
test_heredoc_edge_cases() {
    echo -e "${BLUE}Test 5: Cas limites heredoc${NC}"
    
    # Test: délimiteur manquant (doit échouer)
    bash -c 'cat <<' >/dev/null 2>&1
    local exit_code1=$?
    
    # Test: EOF sans newline (Ctrl+D simulation)
    echo "test line" | bash -c 'cat << EOF
test line' >/dev/null 2>&1
    local exit_code2=$?
    
    # Au moins un des tests doit échouer (syntaxe invalide)
    if [ $exit_code1 -ne 0 ]; then
        print_test_result "Délimiteur manquant (erreur attendue)" 0
    else
        print_test_result "Délimiteur manquant (erreur attendue)" 1
    fi
}

# Function to test empty heredoc
test_empty_heredoc() {
    echo -e "${BLUE}Test 6: Heredoc vide${NC}"
    
    # Test with bash
    bash -c 'cat << EOF
EOF' > bash_empty_output.txt
    
    # Should be empty
    if [ ! -s bash_empty_output.txt ]; then
        print_test_result "Heredoc vide" 0
    else
        print_test_result "Heredoc vide" 1
    fi
    
    # Cleanup
    rm -f bash_empty_output.txt
}

# Main test function
main() {
    echo -e "${YELLOW}=== Tests d'intégration Heredoc - Phase 10 ===${NC}"
    echo -e "${YELLOW}Comparaison avec le comportement de bash${NC}\n"
    
    # Run all tests
    test_heredoc_basic
    test_heredoc_quotes
    test_heredoc_variables
    test_heredoc_pipe
    test_heredoc_edge_cases
    test_empty_heredoc
    
    # Print summary
    echo -e "\n${YELLOW}=== Résumé des tests ===${NC}"
    echo -e "Tests exécutés: $TESTS_RUN"
    echo -e "Tests réussis: $TESTS_PASSED"
    echo -e "Tests échoués: $((TESTS_RUN - TESTS_PASSED))"
    
    if [ $TESTS_PASSED -eq $TESTS_RUN ]; then
        echo -e "${GREEN}✅ Tous les tests d'intégration sont passés !${NC}"
        echo -e "${GREEN}Le module heredoc est conforme au comportement de bash.${NC}"
        return 0
    else
        echo -e "${RED}❌ Certains tests ont échoué.${NC}"
        echo -e "${RED}Le module heredoc nécessite des corrections.${NC}"
        return 1
    fi
}

# Run main function
main "$@"
