#!/bin/bash

# Script de débogage pour identifier les problèmes du test strict

# Test quelques commandes problématiques suspectées
test_commands=(
    "echo test"
    "true"
    "false"
    "ls *.nonexistent 2>/dev/null"
    "export TEST=value"
    "echo \$TEST"
)

for cmd in "${test_commands[@]}"; do
    echo "========================================="
    echo "Testing: $cmd"
    echo "========================================="
    
    # Test avec bash
    echo "BASH:"
    echo "$cmd" | bash 2>bash_error.tmp >bash_out.tmp
    bash_exit=$?
    echo "  Output: $(cat bash_out.tmp)"
    echo "  Error: $(cat bash_error.tmp)"
    echo "  Exit code: $bash_exit"
    echo "  Error file size: $(wc -c < bash_error.tmp)"
    
    echo ""
    echo "MINISHELL:"
    echo "$cmd" | ./minishell 2>mini_error.tmp >mini_out.tmp
    mini_exit=$?
    echo "  Raw output: $(cat mini_out.tmp)"
    echo "  Error: $(cat mini_error.tmp)"
    echo "  Exit code: $mini_exit"
    echo "  Error file size: $(wc -c < mini_error.tmp)"
    
    # Nettoyer la sortie minishell comme dans le script strict
    cat mini_out.tmp | sed 's/^minishell\$ //g' | sed '/^$/d' | grep -v "^exit$" > mini_cleaned.tmp
    echo "  Cleaned output: $(cat mini_cleaned.tmp)"
    echo ""
done

# Nettoyer
rm -f bash_error.tmp bash_out.tmp mini_error.tmp mini_out.tmp mini_cleaned.tmp
