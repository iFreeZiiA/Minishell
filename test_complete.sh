#!/bin/bash

echo "=== PREPARATION ==="
touch main.c test.c wildcard.c readme.txt notes.txt .hidden

echo -e "\n=== TESTS WILDCARDS SIMPLES ==="
./lexer_ut "*.c"
./lexer_ut "*.txt"
./lexer_ut "*test*"

echo -e "\n=== TESTS AVEC COMMANDES ==="
./lexer_ut "echo *.c"
./lexer_ut "ls *.txt"

echo -e "\n=== TESTS VARIABLES + WILDCARDS ==="
./lexer_ut "echo \$USER *.c"

echo -e "\n=== TESTS QUOTES (PAS D'EXPANSION) ==="
./lexer_ut "echo \"*.c\""
./lexer_ut "echo '*.txt'"

echo -e "\n=== TESTS MULTIPLES ==="
./lexer_ut "*.c *.txt"

echo -e "\n=== TESTS OPERATEURS ==="
./lexer_ut "echo *.c | grep main"

echo -e "\n=== NETTOYAGE ==="
rm -f main.c test.c wildcard.c readme.txt notes.txt .hidden
