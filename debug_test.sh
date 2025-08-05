#!/bin/bash

echo "=== DEBUG TEST ==="

# Test 1: sans timeout
echo "Test 1: Sans timeout"
echo -e "echo hello && echo world\nexit" | ./minishell
echo "Exit code: $?"

echo ""

# Test 2: avec timeout
echo "Test 2: Avec timeout"
echo -e "echo hello && echo world\nexit" | timeout 5s ./minishell
echo "Exit code: $?"

echo ""

# Test 3: avec redirection
echo "Test 3: Avec redirection"
echo -e "echo hello && echo world\nexit" | timeout 5s ./minishell >/dev/null 2>&1
echo "Exit code: $?"

echo ""

# Test 4: format exact du script
echo "Test 4: Format exact du script"
command="echo hello && echo world"
echo -e "$command\nexit" | timeout 5s ./minishell >/dev/null 2>&1
echo "Exit code: $?"
