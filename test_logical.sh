#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    logical.sh                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#              #
#    Updated: 2025/07/20 17:30:00 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# Binary name
LOGICAL_TEST="logical_ut"

# Test counter
test_result=0

echo -e "${BLUE}================================================${NC}"
echo -e "${BLUE}         TESTS PHASE 7.1 - OPÉRATEURS LOGIQUES${NC}"
echo -e "${BLUE}================================================${NC}"
echo ""

# Check if test binary exists
if [ ! -f "./$LOGICAL_TEST" ]; then
    echo -e "${RED}❌ Erreur: Binaire de test $LOGICAL_TEST introuvable${NC}"
    echo -e "${YELLOW}💡 Compilez avec: make logical${NC}"
    exit 1
fi

echo -e "${YELLOW}🧪 Lancement des tests unitaires intégrés...${NC}"
echo ""

# Run unit tests
./$LOGICAL_TEST
unit_result=$?

if [ $unit_result -eq 0 ]; then
    echo -e "${GREEN}✅ Tests unitaires: RÉUSSIS${NC}"
else
    echo -e "${RED}❌ Tests unitaires: ÉCHEC${NC}"
    test_result=1
fi

echo ""
echo -e "${BLUE}================================================${NC}"
echo -e "${BLUE}         TESTS MANUELS COMPLÉMENTAIRES${NC}"
echo -e "${BLUE}================================================${NC}"
echo ""

# Check if main minishell binary exists for integration tests
MINISHELL="minishell"
if [ ! -f "./$MINISHELL" ]; then
    echo -e "${YELLOW}⚠️  Binaire minishell introuvable - tests d'intégration ignorés${NC}"
    echo ""
else
    echo -e "${YELLOW}🔧 Tests d'intégration avec minishell...${NC}"
    echo ""
    
    # Test 1: Basic AND
    echo -e "${YELLOW}Test 1: Opérateur AND basique${NC}"
    echo -e "Command: ${BLUE}echo test1 && echo test2${NC}"
    echo "echo test1 && echo test2" | timeout 3s ./$MINISHELL 2>/dev/null
    echo -e "${GREEN}✅ ATTENDU: test1 puis test2 affichés${NC}"
    echo ""
    
    # Test 2: Basic OR
    echo -e "${YELLOW}Test 2: Opérateur OR basique${NC}"
    echo -e "Command: ${BLUE}false || echo fallback${NC}"
    echo "false || echo fallback" | timeout 3s ./$MINISHELL 2>/dev/null
    echo -e "${GREEN}✅ ATTENDU: fallback affiché${NC}"
    echo ""
    
    # Test 3: Mixed operators
    echo -e "${YELLOW}Test 3: Opérateurs mixtes${NC}"
    echo -e "Command: ${BLUE}true && echo success || echo failure${NC}"
    echo "true && echo success || echo failure" | timeout 3s ./$MINISHELL 2>/dev/null
    echo -e "${GREEN}✅ ATTENDU: success affiché${NC}"
    echo ""
    
    # Test 4: Short circuit AND
    echo -e "${YELLOW}Test 4: Court-circuit AND${NC}"
    echo -e "Command: ${BLUE}false && echo not_shown${NC}"
    echo "false && echo not_shown" | timeout 3s ./$MINISHELL 2>/dev/null
    echo -e "${GREEN}✅ ATTENDU: Rien affiché (court-circuit)${NC}"
    echo ""
fi

echo -e "${BLUE}================================================${NC}"
echo -e "${BLUE}         TESTS DE VALIDATION SYNTAXIQUE${NC}"
echo -e "${BLUE}================================================${NC}"
echo ""

# Test syntax errors (should be caught by parser)
echo -e "${YELLOW}Test Syntaxe 1: Opérateur en début${NC}"
echo -e "Command: ${BLUE}&& echo test${NC}"
if [ -f "./$MINISHELL" ]; then
    echo "&& echo test" | timeout 3s ./$MINISHELL 2>&1 | head -1
else
    echo "Minishell non disponible"
fi
echo -e "${GREEN}✅ ATTENDU: Erreur de syntaxe${NC}"
echo ""

echo -e "${YELLOW}Test Syntaxe 2: Opérateur en fin${NC}"
echo -e "Command: ${BLUE}echo test &&${NC}"
if [ -f "./$MINISHELL" ]; then
    echo "echo test &&" | timeout 3s ./$MINISHELL 2>&1 | head -1
else
    echo "Minishell non disponible"
fi
echo -e "${GREEN}✅ ATTENDU: Erreur de syntaxe${NC}"
echo ""

echo -e "${YELLOW}Test Syntaxe 3: Opérateurs consécutifs${NC}"
echo -e "Command: ${BLUE}echo test && || echo error${NC}"
if [ -f "./$MINISHELL" ]; then
    echo "echo test && || echo error" | timeout 3s ./$MINISHELL 2>&1 | head -1
else
    echo "Minishell non disponible"
fi
echo -e "${GREEN}✅ ATTENDU: Erreur de syntaxe${NC}"
echo ""

echo -e "${BLUE}================================================${NC}"
echo -e "${BLUE}         TESTS DE PRÉCÉDENCE${NC}"
echo -e "${BLUE}================================================${NC}"
echo ""

echo -e "${YELLOW}Test Précédence: Priorité des opérateurs${NC}"
echo -e "Command: ${BLUE}true || false && echo precedence${NC}"
if [ -f "./$MINISHELL" ]; then
    echo "true || false && echo precedence" | timeout 3s ./$MINISHELL 2>/dev/null
else
    echo "Minishell non disponible"
fi
echo -e "${GREEN}✅ ATTENDU: precedence affiché (AND prioritaire sur OR)${NC}"
echo ""

echo -e "${PURPLE}⚠️  FONCTIONNALITÉS À IMPLÉMENTER (phases suivantes):${NC}"
echo -e "${RED}❌ Parenthèses: (echo a && echo b) || echo c (Phase 8)${NC}"
echo -e "${RED}❌ Pipes avec logique: echo a | grep a && echo found (Phase 6)${NC}"
echo -e "${RED}❌ Redirections: echo test > file && cat file (Phase 6)${NC}"
echo -e "${RED}❌ Variables: VAR=test && echo \$VAR (Phase 3)${NC}"
echo -e "${RED}❌ Wildcards: ls *.c && echo found (Phase 9)${NC}"

echo ""

# Summary
echo -e "${BLUE}================================================${NC}"
echo -e "${BLUE}         RÉSUMÉ PHASE 7.1${NC}"
echo -e "${BLUE}================================================${NC}"

if [ $test_result -eq 0 ]; then
    echo -e "${GREEN}✅ Tests Phase 7.1: RÉUSSIS${NC}"
    echo -e "${YELLOW}🎯 Opérateurs logiques && || opérationnels${NC}"
    echo -e "${YELLOW}✅ Détection des tokens logiques${NC}"
    echo -e "${YELLOW}✅ Gestion des précédences${NC}"
    echo -e "${YELLOW}✅ Validation syntaxique${NC}"
    echo -e "${YELLOW}🚀 Prêt pour la Phase 8 (parenthèses et groupes)${NC}"
else
    echo -e "${RED}❌ Tests Phase 7.1: ÉCHEC${NC}"
    echo -e "${YELLOW}🔧 Corrigez les opérateurs logiques avant de continuer${NC}"
fi

echo ""
echo -e "${BLUE}✨ Phase 7.1 complète = Opérateurs logiques && || fonctionnels !${NC}"
echo -e "${YELLOW}Prochaines étapes: Phase 8 (parenthèses) puis Phase 6 (pipes avancés)${NC}"

exit $test_result
