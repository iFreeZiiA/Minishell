#!/bin/bash

# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   test_lexer.sh                                      :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: unit_test <unit_test@student.42.fr>        +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2025/07/20 10:00:00 by unit_test         #+#    #+#             #
#   Updated: 2025/07/20 10:00:00 by unit_test        ###   ########.fr       #
#                                                                            #
# ************************************************************************** #

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# Test executable name
TEST_EXEC="./lexer_ut"

echo -e "${BLUE}================================================${NC}"
echo -e "${BLUE}  TESTS PHASES 2 & 3 - LEXER + QUOTES & EXPANSION${NC}"
echo -e "${BLUE}================================================${NC}"
echo -e "${YELLOW}📋 PHASE 2: Tokenisation de base${NC}"
echo -e "${GREEN}   ✅ TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIR_IN, TOKEN_REDIR_OUT${NC}"
echo -e "${GREEN}   ✅ Gestion des espaces multiples${NC}"
echo -e "${YELLOW}📋 PHASE 3: Quotes et expansion${NC}"
echo -e "${GREEN}   ✅ Quotes simples: 'text' (pas d'expansion)${NC}"
echo -e "${GREEN}   ✅ Quotes doubles: \"text\" (expansion possible)${NC}"
echo -e "${GREEN}   ✅ Variables: \$VAR${NC}"
echo -e "${GREEN}   ✅ Exit code: \$?${NC}"
echo -e "${GREEN}   ✅ Expansion dans dquotes: \"Hello \$USER\"${NC}"
echo -e "${RED}   ❌ PAS de wildcards, opérateurs avancés, parsing${NC}\n"

# Check if test executable exists
if [ ! -f "$TEST_EXEC" ]; then
    echo -e "${RED}❌ Executable de test non trouvé: $TEST_EXEC${NC}"
    echo -e "${YELLOW}💡 Compilez d'abord avec: make lexing${NC}"
    exit 1
fi

# Check if the lexer utility exists for token display
if [ ! -f "./lexer_ut" ]; then
    echo -e "${RED}❌ Utilitaire lexer_ut non trouvé${NC}"
    echo -e "${YELLOW}💡 Compilez d'abord avec: make lexing${NC}"
    exit 1
fi

echo -e "${PURPLE}🔍 Lancement des tests automatisés Phases 2 & 3...${NC}\n"

# Run the main test executable
$TEST_EXEC
test_result=$?

echo ""

# Manual tests for both phases
echo -e "${PURPLE}🔧 Tests manuels - Phases 2 & 3...${NC}\n"

# Set up test environment variables
export TEST_MANUAL="manual_test_value"

echo -e "${BLUE}--- TESTS PHASE 2: TOKENISATION DE BASE ---${NC}"

# Test 1: Basic tokens
echo -e "${YELLOW}Test 1: Tokens de base${NC}"
echo -e "Input: ${BLUE}echo hello | grep test > output${NC}"
echo "Sortie du lexer:"
./lexer_ut "echo hello | grep test > output" 2>&1 | grep -E ": [A-Z_]+" || echo "Aucune sortie token visible"
echo -e "${GREEN}✅ ATTENDU: 6 tokens (echo, hello, |, grep, test, >, output)${NC}"
echo ""

# Test 2: Spaces handling
echo -e "${YELLOW}Test 2: Gestion espaces multiples${NC}"
echo -e "Input: ${BLUE}echo    hello    world${NC}"
echo "Sortie du lexer:"
./lexer_ut "echo    hello    world" 2>&1 | grep -E ": [A-Z_]+" || echo "Aucune sortie token visible"
echo -e "${GREEN}✅ ATTENDU: 3 tokens WORD (espaces ignorés)${NC}"
echo ""

echo -e "${BLUE}--- TESTS PHASE 3: QUOTES & EXPANSION ---${NC}"

# Test 3: Simple quotes (no expansion)
echo -e "${YELLOW}Test 3: Quotes simples (pas d'expansion)${NC}"
echo -e "Input: ${BLUE}echo 'Hello \$TEST_MANUAL world'${NC}"
echo "Sortie du lexer:"
./lexer_ut "echo 'Hello \$TEST_MANUAL world'" 2>&1 | grep -E ": [A-Z_]+" || echo "Aucune sortie token visible"
echo -e "${GREEN}✅ ATTENDU: 'Hello \$TEST_MANUAL world' (littéral, pas d'expansion)${NC}"
echo ""

# Test 4: Double quotes (with expansion)
echo -e "${YELLOW}Test 4: Quotes doubles (avec expansion)${NC}"
echo -e "Input: ${BLUE}echo \"Hello \$TEST_MANUAL\"${NC}"
echo "Sortie du lexer:"
./lexer_ut "echo \"Hello \$TEST_MANUAL\"" 2>&1 | grep -E ": [A-Z_]+" || echo "Aucune sortie token visible"
echo -e "${GREEN}✅ ATTENDU: 'Hello manual_test_value' (expansion dans dquotes)${NC}"
echo ""

# Test 5: Variable expansion
echo -e "${YELLOW}Test 5: Expansion variable${NC}"
echo -e "Input: ${BLUE}echo \$TEST_MANUAL${NC}"
echo "Sortie du lexer:"
./lexer_ut "echo \$TEST_MANUAL" 2>&1 | grep -E ": [A-Z_]+" || echo "Aucune sortie token visible"
echo -e "${GREEN}✅ ATTENDU: 'manual_test_value' (variable élargie)${NC}"
echo ""

# Test 6: Exit code
echo -e "${YELLOW}Test 6: Code de sortie${NC}"
echo -e "Input: ${BLUE}echo \$?${NC}"
echo "Sortie du lexer:"
./lexer_ut "echo \$?" 2>&1 | grep -E ": [A-Z_]+" || echo "Aucune sortie token visible"
echo -e "${GREEN}✅ ATTENDU: Code de sortie numérique (ex: '0')${NC}"
echo ""

# Test 7: Complex combination
echo -e "${YELLOW}Test 7: Combinaison complexe${NC}"
echo -e "Input: ${BLUE}echo \"User: \$TEST_MANUAL\" | grep value > 'output file'${NC}"
echo "Sortie du lexer:"
./lexer_ut "echo \"User: \$TEST_MANUAL\" | grep value > 'output file'" 2>&1 | grep -E ": [A-Z_]+" || echo "Aucune sortie token visible"
echo -e "${GREEN}✅ ATTENDU: Expansion dans dquotes + tokens + quotes simples${NC}"
echo ""

echo -e "${PURPLE}⚠️  FONCTIONNALITÉS NON TESTÉES (phases suivantes):${NC}"
echo -e "${RED}❌ Wildcards: echo *.txt (Phase 9)${NC}"
echo -e "${RED}❌ Opérateurs logiques: echo a && echo b (Phase 7)${NC}"
echo -e "${RED}❌ Redirections avancées: echo a >> file (Phase 4)${NC}"
echo -e "${RED}❌ Heredoc: cat << EOF (Phase 10)${NC}"
echo -e "${RED}❌ Parenthèses: (echo a) (Phase 8)${NC}"
echo -e "${RED}❌ Parsing/AST: analyse syntaxique (Phase 5+)${NC}"

echo ""

# Cleanup
unset TEST_MANUAL

# Summary
echo -e "${BLUE}================================================${NC}"
echo -e "${BLUE}         RÉSUMÉ PHASES 2 & 3${NC}"
echo -e "${BLUE}================================================${NC}"

if [ $test_result -eq 0 ]; then
    echo -e "${GREEN}✅ Tests Phases 2 & 3: RÉUSSIS${NC}"
    echo -e "${YELLOW}🎯 Fonctionnalités de base opérationnelles${NC}"
    echo -e "${YELLOW}🚀 Prêt pour la Phase 4 (opérateurs avancés)${NC}"
else
    echo -e "${RED}❌ Tests Phases 2 & 3: ÉCHEC${NC}"
    echo -e "${YELLOW}🔧 Corrigez tokenisation/quotes/expansion avant de continuer${NC}"
fi

echo ""
echo -e "${BLUE}✨ Phases 2 & 3 complètes = Lexer fonctionnel de base !${NC}"
echo -e "${YELLOW}Prochaines étapes: Phase 4 (&&, ||, >>, <<) puis Phase 5 (parsing)${NC}"

exit $test_result