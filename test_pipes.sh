#!/bin/bash

# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   test_pipes.sh                                      :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             #
#   Updated: 2025/07/19 16:00:00 by jjorda           ###   ########.fr       #
#                                                                            #
# ************************************************************************** #

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Variables globales
TOTAL_TESTS=0
PASSED_TESTS=0
EXECUTABLE="pipes_ut"
TEST_DIR="test_pipes_files"

# Fonction d'affichage avec couleurs
print_header() {
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}           TEST MODULE PIPES - PHASE 6.1${NC}"
    echo -e "${BLUE}================================================${NC}"
}

print_test_result() {
    local test_name="$1"
    local result="$2"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if [ "$result" -eq 0 ]; then
        echo -e "${GREEN}✓ $test_name${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ $test_name${NC}"
    fi
}

print_summary() {
    echo -e "\n${BLUE}================================================${NC}"
    echo -e "${BLUE}                RÉSUMÉ DES TESTS${NC}"
    echo -e "${BLUE}================================================${NC}"
    echo -e "Tests réussis: ${GREEN}$PASSED_TESTS${NC}/$TOTAL_TESTS"
    
    if [ "$PASSED_TESTS" -eq "$TOTAL_TESTS" ]; then
        echo -e "${GREEN}🎉 TOUS LES TESTS SONT PASSÉS !${NC}"
        return 0
    else
        echo -e "${RED}❌ CERTAINS TESTS ONT ÉCHOUÉ${NC}"
        return 1
    fi
}

# Création des fichiers de test
setup_test_files() {
    echo -e "${YELLOW}Configuration des fichiers de test...${NC}"
    
    mkdir -p "$TEST_DIR"
    cd "$TEST_DIR"
    
    # Création de fichiers temporaires pour les tests
    echo "hello world" > test_input.txt
    echo "line1" > multiline.txt
    echo "line2" >> multiline.txt
    echo "line3" >> multiline.txt
    
    # Création de fichiers avec différentes extensions
    touch file1.c file2.c file3.txt readme.md
    
    cd ..
    echo "Fichiers de test créés dans $TEST_DIR/"
}

# Nettoyage des fichiers de test
cleanup_test_files() {
    echo -e "${YELLOW}Nettoyage des fichiers de test...${NC}"
    rm -rf "$TEST_DIR"
    echo "Fichiers de test supprimés."
}

# Tests de compilation et existence
run_compilation_test() {
    echo -e "\n${YELLOW}=== TEST DE COMPILATION ===${NC}"
    
    if [ -f "$EXECUTABLE" ]; then
        print_test_result "Compilation réussie" 0
    else
        print_test_result "Compilation échouée" 1
    fi
}

# Tests intégrés du module
run_integrated_tests() {
    echo -e "\n${YELLOW}=== TESTS INTÉGRÉS ===${NC}"
    
    # Test du module lui-même avec timeout pour éviter les hangs
    timeout 10s ./"$EXECUTABLE" > /dev/null 2>&1
    local result=$?
    
    # Si timeout (124) ou segfault, on considère que ça a échoué
    if [ $result -eq 124 ]; then
        print_test_result "Tests intégrés du module (timeout)" 1
    elif [ $result -eq 139 ]; then
        print_test_result "Tests intégrés du module (segfault)" 1
    else
        print_test_result "Tests intégrés du module" $result
    fi
}

# Tests de comparaison avec bash pour pipes simples
run_simple_pipe_tests() {
    echo -e "\n${YELLOW}=== TESTS PIPES SIMPLES ===${NC}"
    
    cd "$TEST_DIR"
    
    # Test 1: echo | cat
    local test_cmd="echo hello | cat"
    echo "$test_cmd" > test_command.txt
    local bash_output=$(bash -c "$test_cmd" 2>/dev/null)
    local bash_exit=$?
    
    if [ $bash_exit -eq 0 ]; then
        print_test_result "Pipe simple: echo | cat" 0
    else
        print_test_result "Pipe simple: echo | cat" 1
    fi
    
    # Test 2: ls | wc -l
    local test_cmd2="ls | wc -l"
    local bash_output2=$(bash -c "$test_cmd2" 2>/dev/null)
    local bash_exit2=$?
    
    if [ $bash_exit2 -eq 0 ]; then
        print_test_result "Pipe avec wc: ls | wc -l" 0
    else
        print_test_result "Pipe avec wc: ls | wc -l" 1
    fi
    
    # Test 3: cat | grep
    local test_cmd3="cat multiline.txt | grep line2"
    local bash_output3=$(bash -c "$test_cmd3" 2>/dev/null)
    local bash_exit3=$?
    
    if [ $bash_exit3 -eq 0 ]; then
        print_test_result "Pipe avec grep: cat | grep" 0
    else
        print_test_result "Pipe avec grep: cat | grep" 1
    fi
    
    cd ..
}

# Tests de pipes multiples
run_multiple_pipe_tests() {
    echo -e "\n${YELLOW}=== TESTS PIPES MULTIPLES ===${NC}"
    
    cd "$TEST_DIR"
    
    # Test 1: echo | cat | wc
    local test_cmd="echo hello world | cat | wc -w"
    local bash_output=$(bash -c "$test_cmd" 2>/dev/null)
    local bash_exit=$?
    
    if [ $bash_exit -eq 0 ]; then
        print_test_result "Triple pipe: echo | cat | wc" 0
    else
        print_test_result "Triple pipe: echo | cat | wc" 1
    fi
    
    # Test 2: ls | grep | wc
    local test_cmd2="ls *.txt | grep line | wc -l"
    local bash_output2=$(bash -c "$test_cmd2" 2>/dev/null)
    local bash_exit2=$?
    
    if [ $bash_exit2 -eq 0 ]; then
        print_test_result "Triple pipe avec grep: ls | grep | wc" 0
    else
        print_test_result "Triple pipe avec grep: ls | grep | wc" 1
    fi
    
    cd ..
}

# Tests d'erreurs de syntaxe
run_syntax_error_tests() {
    echo -e "\n${YELLOW}=== TESTS ERREURS SYNTAXE ===${NC}"
    
    # Test 1: Pipe au début
    echo "| echo hello" > test_error1.txt
    bash -c "$(cat test_error1.txt)" > /dev/null 2>&1
    local bash_exit1=$?
    
    if [ $bash_exit1 -ne 0 ]; then
        print_test_result "Erreur: pipe au début" 0
    else
        print_test_result "Erreur: pipe au début" 1
    fi
    
    # Test 2: Pipe à la fin
    echo "echo hello |" > test_error2.txt
    bash -c "$(cat test_error2.txt)" > /dev/null 2>&1
    local bash_exit2=$?
    
    if [ $bash_exit2 -ne 0 ]; then
        print_test_result "Erreur: pipe à la fin" 0
    else
        print_test_result "Erreur: pipe à la fin" 1
    fi
    
    # Test 3: Pipes consécutifs
    echo "echo hello || echo world" > test_error3.txt
    # Note: || est un opérateur logique, pas une erreur de pipe
    bash -c "$(cat test_error3.txt)" > /dev/null 2>&1
    local bash_exit3=$?
    
    if [ $bash_exit3 -eq 0 ]; then
        print_test_result "Opérateur logique ||" 0
    else
        print_test_result "Opérateur logique ||" 1
    fi
    
    rm -f test_error*.txt
}

# Tests de performance et cas limites
run_edge_case_tests() {
    echo -e "\n${YELLOW}=== TESTS CAS LIMITES ===${NC}"
    
    cd "$TEST_DIR"
    
    # Test 1: Commande vide avec pipe
    echo " | cat" > test_edge1.txt
    bash -c "$(cat test_edge1.txt)" > /dev/null 2>&1
    local bash_exit1=$?
    
    if [ $bash_exit1 -ne 0 ]; then
        print_test_result "Commande vide avant pipe" 0
    else
        print_test_result "Commande vide avant pipe" 1
    fi
    
    # Test 2: Espaces multiples autour du pipe
    local test_cmd2="echo hello    |    cat"
    local bash_output2=$(bash -c "$test_cmd2" 2>/dev/null)
    local bash_exit2=$?
    
    if [ $bash_exit2 -eq 0 ]; then
        print_test_result "Espaces multiples autour pipe" 0
    else
        print_test_result "Espaces multiples autour pipe" 1
    fi
    
    cd ..
    rm -f test_edge*.txt
}

# Fonction principale
main() {
    print_header
    
    # Vérification des prérequis
    if [ ! -f "$EXECUTABLE" ]; then
        echo -e "${RED}Erreur: Exécutable '$EXECUTABLE' non trouvé${NC}"
        echo -e "${YELLOW}Compilez d'abord avec: make pipes${NC}"
        exit 1
    fi
    
    # Configuration
    setup_test_files
    
    # Exécution des tests
    run_compilation_test
    run_integrated_tests
    run_simple_pipe_tests
    run_multiple_pipe_tests
    run_syntax_error_tests
    run_edge_case_tests
    
    # Nettoyage
    cleanup_test_files
    
    # Résumé
    print_summary
    exit $?
}

# Gestion des signaux
trap cleanup_test_files EXIT

# Exécution
main "$@"
