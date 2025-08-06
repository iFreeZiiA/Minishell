#!/bin/bash

# Test validation lexer extension
echo "=== TEST VALIDATION LEXER EXTENSION ==="

# Test 1: Commande simple (référence)
echo "Test 1: echo test"
echo "echo test" | ./minishell 2>&1 | grep "DEBUG: .* tokens"

# Test 2: Avec &&
echo -e "\nTest 2: echo a && echo b"
echo "echo a && echo b" | ./minishell 2>&1 | grep "DEBUG: .* tokens"

# Test 3: Avec ||
echo -e "\nTest 3: echo a || echo b"
echo "echo a || echo b" | ./minishell 2>&1 | grep "DEBUG: .* tokens"

# Test 4: Avec parenthèses
echo -e "\nTest 4: (echo test)"
echo "(echo test)" | ./minishell 2>&1 | grep "DEBUG: .* tokens"

# Test 5: Combiné
echo -e "\nTest 5: (echo a && echo b) || echo c"
echo "(echo a && echo b) || echo c" | ./minishell 2>&1 | grep "DEBUG: .* tokens"

echo -e "\n=== INTERPRÉTATION DES RÉSULTATS ==="
echo "- echo test : attendu 2 tokens"
echo "- echo a && echo b : attendu 5 tokens (echo, a, &, &, echo, b si && décomposé)"
echo "- Parenthèses ajoutent 2 tokens supplémentaires"
echo "- Plus de tokens = meilleure reconnaissance"
