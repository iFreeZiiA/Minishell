#!/bin/bash

# Test rapide pour vérifier l'extraction des codes de sortie dans test_all_commands.sh

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}Test extraction codes de sortie - test_all_commands.sh${NC}"

command="echo hello"
bash_output="bash_quick.out"
mini_output="mini_quick.out"

# Test avec bash
echo -e "$command\necho BASH_CODE:\$?\nexit" | timeout 5s bash > "$bash_output" 2> /dev/null

# Test avec minishell
echo -e "$command\necho MINI_CODE:\$?\nexit" | timeout 5s ./minishell > "$mini_output" 2> /dev/null

# Nettoyer minishell
grep -v "minishell\$" "$mini_output" | grep -v "^exit$" > "${mini_output}.clean" 2>/dev/null
mv "${mini_output}.clean" "$mini_output" 2>/dev/null

echo "Sortie bash:"
cat "$bash_output"
echo "---"
echo "Sortie minishell:"
cat "$mini_output"
echo "---"

# Extraction avec la nouvelle méthode
bash_code=$(grep -o "BASH_CODE:[0-9]*" "$bash_output" 2>/dev/null | cut -d: -f2 | tail -1)
mini_code=$(grep -o "MINI_CODE:[0-9]*" "$mini_output" 2>/dev/null | cut -d: -f2 | tail -1)

echo "Code bash extrait: '$bash_code'"
echo "Code mini extrait: '$mini_code'"

if [ "$bash_code" = "$mini_code" ]; then
    echo -e "${GREEN}✅ EXTRACTION RÉUSSIE: codes identiques ($bash_code)${NC}"
else
    echo -e "${RED}❌ EXTRACTION ÉCHOUÉE: bash='$bash_code', mini='$mini_code'${NC}"
fi

# Nettoyer
rm -f "$bash_output" "$mini_output" 2>/dev/null
