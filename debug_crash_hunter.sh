#!/bin/bash

# Script de debug pour identifier les crashes spécifiques du crash hunter

MINISHELL="./minishell"
TIMEOUT=2

# Test des cas problématiques spécifiques du crash hunter
debug_tests=(
    # Tests de stress avec longs arguments
    "echo $(seq -s' echo ' 1 1000)" 
    
    # Tests d'imbrication extrême
    "$(printf '(' | head -c 1000)echo hello$(printf ')' | head -c 1000)"
    
    # Tests de variables circulaires
    "export A=\$B && export B=\$A && echo \$A"
    
    # Tests de heredoc extrêmes
    "cat << $(printf 'A' | head -c 1000)"
    
    # Tests de wildcards extrêmes 
    "echo $(printf '*/' | head -c 500)"
    
    # Tests de signaux/interruptions
    "sleep 10 &"
    
    # Tests spécifiques qui pourraient causer des problèmes
    "< < <"
    "| | |"
    "( ||| )"
    "( && || )"
    "echo > > > > >"
    "ls ||||||| grep"
    "&& && &&"
    "|| || ||"
    ">>> <<<"
    "echo | | | | |"
    
    # Tests de limites système
    "echo \$(seq 1 10000)"
    "ls $(printf '/' | head -c 100)"
)

test_count=0
crash_count=0

echo "🔍 DEBUG CRASH HUNTER - Tests spécifiques"
echo "========================================"

for test_cmd in "${debug_tests[@]}"; do
    test_count=$((test_count + 1))
    echo -n "Test $test_count: $(echo "$test_cmd" | head -c 50)..."
    
    # Test avec timeout
    output=$(timeout $TIMEOUT bash -c "echo '$test_cmd' | $MINISHELL" 2>&1)
    exit_code=$?
    
    if [[ $exit_code -eq 139 || $exit_code -eq 134 ]]; then
        echo -e " 💀 CRASH (exit: $exit_code)"
        echo "   Command: $test_cmd"
        crash_count=$((crash_count + 1))
    elif [[ $exit_code -eq 124 ]]; then
        echo -e " ⏰ TIMEOUT"
        crash_count=$((crash_count + 1))
    else
        echo -e " ✅ OK (exit: $exit_code)"
    fi
done

echo ""
echo "📊 RÉSULTATS DEBUG:"
echo "   Total tests: $test_count"
echo "   Crashes: $crash_count"
echo "   Succès: $((test_count - crash_count))"

if [ $crash_count -gt 0 ]; then
    echo "🚨 $crash_count vulnérabilités détectées!"
else
    echo "🎉 Aucune vulnérabilité détectée!"
fi
