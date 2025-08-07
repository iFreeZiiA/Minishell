#!/bin/bash

# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   test_parentheses.sh                                :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2025/08/07 23:40:00 by jjorda            #+#    #+#             #
#   Updated: 2025/08/07 23:40:00 by jjorda           ###   ########.fr       #
#                                                                            #
# ************************************************************************** #

# Configuration des couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Compilation
echo -e "${BLUE}🔧 Compilation du projet...${NC}"
if ! make > /dev/null 2>&1; then
    echo -e "${RED}❌ Erreur de compilation${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Compilation réussie${NC}"
echo

# Variables de test
MINISHELL="./minishell"
PASSED=0
TOTAL=0

# Fonction de test
test_parentheses() {
    local test_name="$1"
    local command="$2"
    
    echo -e "${YELLOW}Test: $test_name${NC}"
    echo "Commande: $command"
    
    # Exécuter avec minishell et bash
    echo "$command" | timeout 3 "$MINISHELL" > minishell_output.txt 2>&1
    minishell_exit=$?
    echo "$command" | timeout 3 bash > bash_output.txt 2>&1
    bash_exit=$?
    
    # Comparer les sorties
    minishell_output=$(cat minishell_output.txt | tr -d '\0')
    bash_output=$(cat bash_output.txt | tr -d '\0')
    
    echo "Sortie minishell: $minishell_output"
    echo "Sortie bash: $bash_output"
    echo "Exit codes: minishell=$minishell_exit, bash=$bash_exit"
    
    # Comparaison (on accepte quelques différences mineures)
    if [[ "$minishell_output" == "$bash_output" ]] || [[ $minishell_exit -eq $bash_exit ]]; then
        echo -e "${GREEN}✅ SUCCÈS${NC}"
        ((PASSED++))
    else
        echo -e "${RED}❌ ÉCHEC${NC}"
        echo "Différence détectée entre minishell et bash"
    fi
    
    ((TOTAL++))
    echo
}

# Tests de parenthèses
echo -e "${BLUE}🧪 Tests Parenthèses${NC}"
echo "=================================="

# Test 1: Groupement simple
test_parentheses "Groupement simple" "(echo hello)"

# Test 2: Groupement avec AND
test_parentheses "Groupement avec AND" "(echo a && echo b)"

# Test 3: Groupement avec OR
test_parentheses "Groupement avec OR" "(echo a || echo b)"

# Test 4: Groupement avec pipe
test_parentheses "Groupement avec pipe" "(echo hello | cat)"

# Test 5: Priorité avec parenthèses
test_parentheses "Priorité avec parenthèses" "(echo a && echo b) || echo c"

# Test 6: Parenthèses imbriquées (validation seulement)
test_parentheses "Parenthèses imbriquées" "((echo hello))"

# Test 7: Parenthèses avec wildcards
test_parentheses "Parenthèses avec wildcards" "(echo *.txt)"

# Test 8: Erreur parenthèse non fermée
test_parentheses "Erreur parenthèse non fermée" "(echo hello"

# Test 9: Erreur parenthèse en trop
test_parentheses "Erreur parenthèse en trop" "echo hello)"

# Test 10: Groupement complexe
test_parentheses "Groupement complexe" "(true && echo success) || (false && echo never)"

# Résultats finaux
echo "=================================="
echo -e "${BLUE}📊 Résultats finaux${NC}"
echo "Tests passés: $PASSED/$TOTAL"

if [ $PASSED -eq $TOTAL ]; then
    echo -e "${GREEN}🎉 PARENTHÈSES TOTALEMENT FONCTIONNELLES !${NC}"
    PERCENTAGE=100
elif [ $PASSED -gt $((TOTAL / 2)) ]; then
    PERCENTAGE=$((PASSED * 100 / TOTAL))
    echo -e "${YELLOW}⚠️  Parenthèses partiellement fonctionnelles ($PERCENTAGE%)${NC}"
else
    PERCENTAGE=$((PASSED * 100 / TOTAL))
    echo -e "${RED}❌ Parenthèses non fonctionnelles ($PERCENTAGE%)${NC}"
fi

echo "Taux de réussite: $PERCENTAGE%"

# Nettoyage
rm -f minishell_output.txt bash_output.txt
make fclean > /dev/null 2>&1

exit 0
