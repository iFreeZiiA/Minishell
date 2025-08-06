#!/bin/bash

# Test spécifique pour la commande problématique mentionnée par l'utilisateur

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}Test spécifique: gdsfhsgfd && echo \$?${NC}"

# Test avec bash
echo "=== BASH ==="
echo -e "gdsfhsgfd && echo \$?\necho FINAL_EXIT=\$?\nexit" | timeout 5s bash > bash_test.out 2> bash_test.err
echo "STDOUT:"
cat bash_test.out
echo "STDERR:"
cat bash_test.err
bash_code=$(grep -o "FINAL_EXIT=[0-9]*" bash_test.out | cut -d= -f2)
echo "Code final bash: $bash_code"

echo ""
echo "=== MINISHELL ==="
echo -e "gdsfhsgfd && echo \$?\necho FINAL_EXIT=\$?\nexit" | timeout 5s ./minishell > mini_test.out 2> mini_test.err
echo "STDOUT:"
cat mini_test.out
echo "STDERR:"
cat mini_test.err
mini_code=$(grep -o "FINAL_EXIT=[0-9]*" mini_test.out | cut -d= -f2)
echo "Code final minishell: $mini_code"

echo ""
echo "=== COMPARAISON ==="
if [ "$bash_code" = "$mini_code" ]; then
    echo -e "${GREEN}✅ CODES IDENTIQUES: $bash_code${NC}"
else
    echo -e "${RED}❌ CODES DIFFÉRENTS: bash=$bash_code, mini=$mini_code${NC}"
fi

# Nettoyer
rm -f bash_test.out bash_test.err mini_test.out mini_test.err
