#!/bin/bash

# Script de test simple pour debug
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

MINISHELL="./minishell"
TEMP_DIR="./test_debug"
ORIGINAL_DIR=$(pwd)

setup_environment() {
    echo "Setting up environment..."
    mkdir -p "$TEMP_DIR" 2>/dev/null
    cd "$TEMP_DIR" 2>/dev/null
    echo "Test content" > test_file.txt
}

cleanup_environment() {
    cd "$ORIGINAL_DIR" 2>/dev/null
    rm -rf "$TEMP_DIR" 2>/dev/null
}

test_simple() {
    echo -e "${YELLOW}Testing: gdsfhsgfd && echo after${NC}"
    
    # Test avec bash
    echo "Testing with bash:"
    echo -e "gdsfhsgfd && echo after\necho EXIT_CODE:\$?\nexit" | timeout 5s bash > bash_out.txt 2> bash_err.txt
    echo "Bash output:"
    cat bash_out.txt
    echo "Bash error:"
    cat bash_err.txt
    echo "---"
    
    # Test avec minishell
    echo "Testing with minishell:"
    echo -e "gdsfhsgfd && echo after\necho EXIT_CODE:\$?\nexit" | timeout 5s "$ORIGINAL_DIR/$MINISHELL" > mini_out.txt 2> mini_err.txt
    echo "Minishell output:"
    cat mini_out.txt
    echo "Minishell error:"
    cat mini_err.txt
    
    rm -f bash_out.txt bash_err.txt mini_out.txt mini_err.txt
}

main() {
    if [ ! -f "$MINISHELL" ]; then
        echo "Minishell not found"
        exit 1
    fi
    
    setup_environment
    test_simple
    cleanup_environment
}

trap cleanup_environment EXIT
main "$@"
