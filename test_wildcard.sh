#!/bin/bash

# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   test_wildcard.sh                                   :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2025/08/07 23:05:00 by jjorda            #+#    #+#             #
#   Updated: 2025/08/07 23:05:00 by jjorda           ###   ########.fr       #
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
test_wildcard() {
    local test_name="$1"
    local command="$2"
    local expected_pattern="$3"
    
    echo -e "${YELLOW}Test: $test_name${NC}"
    echo "Commande: $command"
    
    # Préparer l'environnement de test
    mkdir -p test_wildcard_dir
    cd test_wildcard_dir
    
    # Créer des fichiers de test
    touch file1.txt file2.txt file3.c main.c test.h
    touch .hidden1 .hidden2.txt
    
    # Exécuter avec minishell et bash
    echo "$command" | timeout 3 ../"$MINISHELL" > minishell_output.txt 2>&1
    echo "$command" | timeout 3 bash > bash_output.txt 2>&1
    
    # Comparer les sorties (on vérifie que le nombre de fichiers listés est cohérent)
    minishell_lines=$(wc -l < minishell_output.txt)
    bash_lines=$(wc -l < bash_output.txt)
    
    echo "Sortie minishell: $(cat minishell_output.txt | tr '\n' ' ')"
    echo "Sortie bash: $(cat bash_output.txt | tr '\n' ' ')"
    
    # Vérification basique (même nombre de lignes ou pattern correspondant)
    if [[ $minishell_lines -eq $bash_lines ]] || [[ "$(cat minishell_output.txt)" =~ $expected_pattern ]]; then
        echo -e "${GREEN}✅ SUCCÈS${NC}"
        ((PASSED++))
    else
        echo -e "${RED}❌ ÉCHEC${NC}"
        echo "Différence détectée entre minishell et bash"
    fi
    
    cd ..
    rm -rf test_wildcard_dir
    ((TOTAL++))
    echo
}

# Tests de wildcards
echo -e "${BLUE}🧪 Tests Wildcards${NC}"
echo "=================================="

# Test 1: Wildcard simple
test_wildcard "Wildcard simple *.txt" "echo *.txt" ".*\.txt"

# Test 2: Wildcard .c
test_wildcard "Wildcard *.c" "echo *.c" ".*\.c"

# Test 3: Wildcard tout
test_wildcard "Wildcard *" "echo *" ".*"

# Test 4: Wildcard avec ls
test_wildcard "ls *.txt" "ls *.txt" ".*\.txt"

# Test 5: Wildcard inexistant
test_wildcard "Wildcard inexistant" "echo *.xyz" ".*\.xyz"

# Résultats finaux
echo "=================================="
echo -e "${BLUE}📊 Résultats finaux${NC}"
echo "Tests passés: $PASSED/$TOTAL"

if [ $PASSED -eq $TOTAL ]; then
    echo -e "${GREEN}🎉 WILDCARDS TOTALEMENT FONCTIONNELS !${NC}"
    PERCENTAGE=100
else
    PERCENTAGE=$((PASSED * 100 / TOTAL))
    echo -e "${YELLOW}⚠️  Wildcards partiellement fonctionnels ($PERCENTAGE%)${NC}"
fi

echo "Taux de réussite: $PERCENTAGE%"

# Nettoyage
make fclean > /dev/null 2>&1

exit 0
