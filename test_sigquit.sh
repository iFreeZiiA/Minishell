#!/bin/bash

echo "Test du comportement de SIGQUIT (Ctrl+\\)"
echo "=========================================="
echo ""

echo "1. Test avec bash :"
echo "Tapez 'sleep 5' puis Ctrl+\\ dans bash"
echo "bash"

echo ""
echo "2. Test avec minishell :"
echo "Tapez 'sleep 5' puis Ctrl+\\ dans minishell"
echo "./minishell"

echo ""
echo "3. Pour test automatique, utilisez :"
echo "timeout 2s bash -c 'sleep 10' || echo 'Interrupted'"
echo "timeout 2s ./minishell -c 'sleep 10' || echo 'Interrupted'"
