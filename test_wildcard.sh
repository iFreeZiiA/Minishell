#!/bin/bash

# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   test_wildcard.sh                                   :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2025/07/19 15:30:00 by jjorda            #+#    #+#             #
#   Updated: 2025/07/19 15:30:00 by jjorda           ###   ########.fr       #
#                                                                            #
# ************************************************************************** #

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Variables globales
TOTAL_TESTS=0
PASSED_TESTS=0
EXECUTABLE="wcard_ut"

# Fonction d'affichage avec couleurs
print_header() {
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}           TEST MODULE WILDCARD${NC}"
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
    
    # Création de fichiers temporaires pour les tests
    touch test_file.c
    touch main.c
    touch wildcard.c
    touch readme.txt
    touch .hidden_file
    touch another_test.txt
    
    echo "Fichiers de test créés."
}

# Nettoyage des fichiers de test
cleanup_test_files() {
    echo -e "${YELLOW}Nettoyage des fichiers de test...${NC}"
    
    rm -f test_file.c main.c wildcard.c readme.txt .hidden_file another_test.txt
    
    echo "Fichiers de test supprimés."
}

# Tests spécifiques
run_basic_tests() {
    echo -e "\n${YELLOW}=== TESTS DE BASE ===${NC}"
    
    # Test sans argument (tests intégrés)
    ./"$EXECUTABLE" > /dev/null 2>&1
    print_test_result "Tests intégrés de détection" $?
}

run_pattern_tests() {
    echo -e "\n${YELLOW}=== TESTS DE PATTERNS ===${NC}"
    
    # Test avec pattern simple
    ./"$EXECUTABLE" "*.c" > /dev/null 2>&1
    print_test_result "Pattern *.c" $?
    
    # Test avec pattern au début
    ./"$EXECUTABLE" "test*" > /dev/null 2>&1
    print_test_result "Pattern test*" $?
    
    # Test avec pattern au milieu
    ./"$EXECUTABLE" "*file*" > /dev/null 2>&1
    print_test_result "Pattern *file*" $?
    
    # Test sans wildcard
    ./"$EXECUTABLE" "readme.txt" > /dev/null 2>&1
    print_test_result "Fichier exact readme.txt" $?
}

run_edge_cases() {
    echo -e "\n${YELLOW}=== TESTS CAS LIMITES ===${NC}"
    
    # Test avec pattern vide
    ./"$EXECUTABLE" "*" > /dev/null 2>&1
    print_test_result "Pattern * (tous fichiers)" $?
    
    # Test avec fichiers cachés
    ./"$EXECUTABLE" ".*" > /dev/null 2>&1
    print_test_result "Pattern .* (fichiers cachés)" $?
    
    # Test pattern inexistant
    ./"$EXECUTABLE" "*.nonexistent" > /dev/null 2>&1
    print_test_result "Pattern inexistant *.nonexistent" $?
}

run_compilation_test() {
    echo -e "\n${YELLOW}=== TEST DE COMPILATION ===${NC}"
    
    # Vérification que l'exécutable existe
    if [ -f "$EXECUTABLE" ]; then
        print_test_result "Compilation réussie" 0
    else
        print_test_result "Compilation échouée" 1
    fi
}

# Fonction principale
main() {
    print_header
    
    # Vérification des prérequis
    if [ ! -f "$EXECUTABLE" ]; then
        echo -e "${RED}Erreur: Exécutable '$EXECUTABLE' non trouvé${NC}"
        echo -e "${YELLOW}Compilez d'abord avec: make wildcard${NC}"
        exit 1
    fi
    
    # Configuration
    setup_test_files
    
    # Exécution des tests
    run_compilation_test
    run_basic_tests
    run_pattern_tests
    run_edge_cases
    
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
