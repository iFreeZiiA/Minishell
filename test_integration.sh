#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_parsing_integration.sh                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/26 18:00:00 by jjorda            #+#    #+#              #
#    Updated: 2025/07/26 18:30:00 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Configuration
EXECUTABLE="parser_integration_ut"
INTEGRATION_TEST="integration_test"

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Variables globales pour les statistiques
TESTS_TOTAL=0
TESTS_PASSED=0
TESTS_FAILED=0

# Fonctions utilitaires
print_header() {
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}         TESTS PARSING INTÉGRÉ${NC}"
    echo -e "${BLUE}       Phases 2-11.4 Complètes${NC}"
    echo -e "${BLUE}================================================${NC}"
    echo ""
}

print_test_result() {
    local test_name="$1"
    local result="$2"
    
    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    
    if [ $result -eq 0 ]; then
        echo -e "✅ ${GREEN}$test_name - PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "❌ ${RED}$test_name - FAIL${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
}

print_summary() {
    echo ""
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}                 RÉSUMÉ${NC}"
    echo -e "${BLUE}================================================${NC}"
    echo -e "Tests totaux: ${YELLOW}$TESTS_TOTAL${NC}"
    echo -e "Tests réussis: ${GREEN}$TESTS_PASSED${NC}"
    echo -e "Tests échoués: ${RED}$TESTS_FAILED${NC}"
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}🎉 Tous les tests sont passés !${NC}"
        return 0
    else
        echo -e "${RED}💥 $TESTS_FAILED test(s) ont échoué.${NC}"
        return 1
    fi
}

# Tests Phase 2: Tokenisation de base
test_phase_2() {
    echo -e "\n${YELLOW}=== PHASE 2: TOKENISATION ===${NC}"
    
    # Test tokenisation simple
    ./"$EXECUTABLE" "echo hello" > /dev/null 2>&1
    print_test_result "Tokenisation simple" $?
    
    # Test avec espaces multiples
    ./"$EXECUTABLE" "echo    hello    world" > /dev/null 2>&1
    print_test_result "Espaces multiples" $?
    
    # Test avec opérateurs
    ./"$EXECUTABLE" "echo hello | cat" > /dev/null 2>&1
    print_test_result "Tokenisation avec pipe" $?
    
    # Test avec redirections
    ./"$EXECUTABLE" "echo hello > file" > /dev/null 2>&1
    print_test_result "Tokenisation avec redirection" $?
}

# Tests Phase 3: Quotes et expansion
test_phase_3() {
    echo -e "\n${YELLOW}=== PHASE 3: QUOTES & EXPANSION ===${NC}"
    
    # Test quotes simples
    ./"$EXECUTABLE" "echo 'hello world'" > /dev/null 2>&1
    print_test_result "Quotes simples" $?
    
    # Test quotes doubles
    ./"$EXECUTABLE" "echo \"hello world\"" > /dev/null 2>&1
    print_test_result "Quotes doubles" $?
    
    # Test expansion variable (si disponible)
    export TEST_VAR="test_value"
    ./"$EXECUTABLE" "echo \$TEST_VAR" > /dev/null 2>&1
    print_test_result "Expansion variable" $?
    
    # Test expansion code sortie
    ./"$EXECUTABLE" "echo \$?" > /dev/null 2>&1
    print_test_result "Expansion code sortie" $?
    
    unset TEST_VAR
}

# Tests Phase 4: Opérateurs avancés
test_phase_4() {
    echo -e "\n${YELLOW}=== PHASE 4: OPÉRATEURS AVANCÉS ===${NC}"
    
    # Test append
    ./"$EXECUTABLE" "echo hello >> file" > /dev/null 2>&1
    print_test_result "Opérateur append" $?
    
    # Test opérateurs logiques
    ./"$EXECUTABLE" "true && echo success" > /dev/null 2>&1
    print_test_result "Opérateur AND" $?
    
    ./"$EXECUTABLE" "false || echo backup" > /dev/null 2>&1
    print_test_result "Opérateur OR" $?
}

# Tests Phase 5-6: Parsing et pipes
test_phase_5_6() {
    echo -e "\n${YELLOW}=== PHASE 5-6: PARSING & PIPES ===${NC}"
    
    # Test parsing simple
    ./"$EXECUTABLE" "echo hello" > /dev/null 2>&1
    print_test_result "Parsing commande simple" $?
    
    # Test pipe simple
    ./"$EXECUTABLE" "echo hello | cat" > /dev/null 2>&1
    print_test_result "Pipe simple" $?
    
    # Test pipe multiple
    ./"$EXECUTABLE" "echo hello | cat | wc -l" > /dev/null 2>&1
    print_test_result "Pipes multiples" $?
    
    # Test avec redirections
    ./"$EXECUTABLE" "echo hello | cat > output" > /dev/null 2>&1
    print_test_result "Pipe avec redirection" $?
}

# Tests Phase 7: Opérateurs logiques
test_phase_7() {
    echo -e "\n${YELLOW}=== PHASE 7: OPÉRATEURS LOGIQUES ===${NC}"
    
    # Test AND simple
    ./"$EXECUTABLE" "echo a && echo b" > /dev/null 2>&1
    print_test_result "AND simple" $?
    
    # Test OR simple
    ./"$EXECUTABLE" "echo a || echo b" > /dev/null 2>&1
    print_test_result "OR simple" $?
    
    # Test combinaison
    ./"$EXECUTABLE" "true && echo success || echo failure" > /dev/null 2>&1
    print_test_result "Combinaison AND/OR" $?
}

# Tests Phase 8: Parenthèses
test_phase_8() {
    echo -e "\n${YELLOW}=== PHASE 8: PARENTHÈSES ===${NC}"
    
    # Test groupe simple
    ./"$EXECUTABLE" "(echo hello)" > /dev/null 2>&1
    print_test_result "Groupe simple" $?
    
    # Test groupe avec opérateurs
    ./"$EXECUTABLE" "(echo a && echo b)" > /dev/null 2>&1
    print_test_result "Groupe avec AND" $?
    
    # Test groupe avec pipe
    ./"$EXECUTABLE" "(echo hello) | cat" > /dev/null 2>&1
    print_test_result "Groupe avec pipe" $?
}

# Tests Phase 9: Wildcards
test_phase_9() {
    echo -e "\n${YELLOW}=== PHASE 9: WILDCARDS ===${NC}"
    
    # Créer des fichiers test temporaires
    touch test_file.c test_main.c test_readme.txt 2>/dev/null
    
    # Test wildcard simple
    ./"$EXECUTABLE" "echo *.c" > /dev/null 2>&1
    print_test_result "Wildcard *.c" $?
    
    # Test pattern complexe
    ./"$EXECUTABLE" "echo test*" > /dev/null 2>&1
    print_test_result "Pattern test*" $?
    
    # Nettoyage
    rm -f test_file.c test_main.c test_readme.txt 2>/dev/null
}

# Tests Phase 10: Heredoc
test_phase_10() {
    echo -e "\n${YELLOW}=== PHASE 10: HEREDOC ===${NC}"
    echo -e "${PURPLE}Note: Tests heredoc simplifiés (parsing uniquement)${NC}"
    
    # Test détection heredoc
    ./"$EXECUTABLE" "cat << EOF" > /dev/null 2>&1
    print_test_result "Détection heredoc" $?
    
    echo -e "${CYAN}ℹ️  Tests heredoc complets disponibles avec: make test_heredoc${NC}"
}

# Tests Phase 11: Intégration complexe
test_phase_11() {
    echo -e "\n${YELLOW}=== PHASE 11: INTÉGRATION COMPLEXE ===${NC}"
    
    if [ -f "$INTEGRATION_TEST" ]; then
        # Test avec l'exécutable d'intégration
        ./"$INTEGRATION_TEST" > /dev/null 2>&1
        print_test_result "Tests intégration automatiques" $?
    fi
    
    # Test commande complexe
    ./"$EXECUTABLE" "echo hello && echo world | cat" > /dev/null 2>&1
    print_test_result "Commande complexe AND+PIPE" $?
    
    # Test avec groupes
    ./"$EXECUTABLE" "(echo a && echo b) || echo c" > /dev/null 2>&1
    print_test_result "Groupes avec logique" $?
    
    # Test ultra-complexe
    export TEST_VAR="hello"
    ./"$EXECUTABLE" "echo \$TEST_VAR && (echo world | cat) || echo failed" > /dev/null 2>&1
    print_test_result "Commande ultra-complexe" $?
    unset TEST_VAR
}

# Tests de robustesse
test_robustness() {
    echo -e "\n${YELLOW}=== TESTS DE ROBUSTESSE ===${NC}"
    
    # Test input vide
    ./"$EXECUTABLE" "" > /dev/null 2>&1
    local result=$?
    if [ $result -ne 0 ]; then
        print_test_result "Input vide (doit échouer)" 0
    else
        print_test_result "Input vide (ne devrait pas réussir)" 1
    fi
    
    # Test quotes non fermées
    ./"$EXECUTABLE" "echo \"hello" > /dev/null 2>&1
    local result=$?
    if [ $result -ne 0 ]; then
        print_test_result "Quotes non fermées (doit échouer)" 0
    else
        print_test_result "Quotes non fermées (ne devrait pas réussir)" 1
    fi
    
    # Test opérateur mal placé
    ./"$EXECUTABLE" "echo hello &&" > /dev/null 2>&1
    local result=$?
    if [ $result -ne 0 ]; then
        print_test_result "Opérateur mal placé (doit échouer)" 0
    else
        print_test_result "Opérateur mal placé (ne devrait pas réussir)" 1
    fi
    
    # Test parenthèses déséquilibrées
    ./"$EXECUTABLE" "(echo hello" > /dev/null 2>&1
    local result=$?
    if [ $result -ne 0 ]; then
        print_test_result "Parenthèses déséquilibrées (doit échouer)" 0
    else
        print_test_result "Parenthèses déséquilibrées (ne devrait pas réussir)" 1
    fi
}

# Tests de performance (optionnel)
test_performance() {
    echo -e "\n${YELLOW}=== TESTS DE PERFORMANCE ===${NC}"
    
    # Test commande longue
    local long_cmd="echo a"
    for i in {1..10}; do
        long_cmd="$long_cmd && echo $i"
    done
    
    local start_time=$(date +%s%N)
    ./"$EXECUTABLE" "$long_cmd" > /dev/null 2>&1
    local end_time=$(date +%s%N)
    local duration=$(( (end_time - start_time) / 1000000 )) # en millisecondes
    
    if [ $duration -lt 1000 ]; then  # moins d'1 seconde
        print_test_result "Performance commande longue (<1s)" 0
    else
        print_test_result "Performance commande longue (>1s)" 1
    fi
    
    echo -e "${CYAN}ℹ️  Temps d'exécution: ${duration}ms${NC}"
}

# Vérification des prérequis
check_prerequisites() {
    if [ ! -f "$EXECUTABLE" ]; then
        echo -e "${RED}Erreur: Exécutable '$EXECUTABLE' non trouvé${NC}"
        echo -e "${YELLOW}Compilez d'abord avec: make parsing${NC}"
        exit 1
    fi
    
    if [ ! -x "$EXECUTABLE" ]; then
        chmod +x "$EXECUTABLE"
    fi
    
    if [ -f "$INTEGRATION_TEST" ] && [ ! -x "$INTEGRATION_TEST" ]; then
        chmod +x "$INTEGRATION_TEST"
    fi
}

# Fonction de nettoyage
cleanup() {
    # Supprimer les fichiers temporaires créés pendant les tests
    rm -f output test_output.txt temp_file.txt test_file.* 2>/dev/null
    echo -e "${CYAN}ℹ️  Fichiers temporaires supprimés${NC}"
}

# Tests spécialisés par demande utilisateur
run_specific_phase() {
    local phase="$1"
    
    case $phase in
        "2")
            test_phase_2
            ;;
        "3")
            test_phase_3
            ;;
        "4")
            test_phase_4
            ;;
        "5"|"6")
            test_phase_5_6
            ;;
        "7")
            test_phase_7
            ;;
        "8")
            test_phase_8
            ;;
        "9")
            test_phase_9
            ;;
        "10")
            test_phase_10
            ;;
        "11")
            test_phase_11
            ;;
        "robustness"|"robust")
            test_robustness
            ;;
        "performance"|"perf")
            test_performance
            ;;
        *)
            echo -e "${RED}Phase inconnue: $phase${NC}"
            echo -e "${YELLOW}Phases disponibles: 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, robustness, performance${NC}"
            exit 1
            ;;
    esac
}

# Affichage de l'aide
show_help() {
    echo -e "${BLUE}📚 AIDE - Script de test parsing intégré${NC}"
    echo ""
    echo -e "${YELLOW}USAGE:${NC}"
    echo "  $0                    # Lance tous les tests"
    echo "  $0 [phase]            # Lance une phase spécifique"
    echo "  $0 help               # Affiche cette aide"
    echo ""
    echo -e "${YELLOW}PHASES DISPONIBLES:${NC}"
    echo "  2          - Tokenisation de base"
    echo "  3          - Quotes et expansion"
    echo "  4          - Opérateurs avancés"
    echo "  5|6        - Parsing et pipes"
    echo "  7          - Opérateurs logiques"
    echo "  8          - Parenthèses"
    echo "  9          - Wildcards"
    echo "  10         - Heredoc"
    echo "  11         - Intégration complexe"
    echo "  robustness - Tests de robustesse"
    echo "  performance- Tests de performance"
    echo ""
    echo -e "${YELLOW}EXEMPLES:${NC}"
    echo "  $0 7                  # Test uniquement les opérateurs logiques"
    echo "  $0 robustness         # Test uniquement la robustesse"
    echo "  $0                    # Tous les tests"
}

# Fonction principale
main() {
    # Vérification des arguments
    if [ "$1" = "help" ] || [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
        show_help
        exit 0
    fi
    
    print_header
    check_prerequisites
    
    # Gestion des signaux pour cleanup
    trap cleanup EXIT
    
    if [ $# -eq 1 ]; then
        # Test d'une phase spécifique
        run_specific_phase "$1"
    else
        # Tous les tests
        echo -e "${CYAN}🚀 Lancement de tous les tests...${NC}"
        test_phase_2
        test_phase_3
        test_phase_4
        test_phase_5_6
        test_phase_7
        test_phase_8
        test_phase_9
        test_phase_10
        test_phase_11
        test_robustness
        
        # Tests de performance optionnels
        echo -e "\n${YELLOW}Lancer les tests de performance ? (y/N)${NC}"
        read -t 5 -n 1 response
        if [[ $response =~ ^[Yy]$ ]]; then
            test_performance
        else
            echo -e "\n${CYAN}ℹ️  Tests de performance ignorés${NC}"
        fi
    fi
    
    # Résumé final
    print_summary
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}🎉 Parsing intégré fonctionnel !${NC}"
        echo -e "${CYAN}ℹ️  Prêt pour l'intégration dans main.c${NC}"
    else
        echo -e "${RED}💥 Des problèmes détectés dans le parsing${NC}"
        echo -e "${YELLOW}⚠️  Vérifiez les phases en échec avant intégration${NC}"
    fi
    
    exit $exit_code
}

# Point d'entrée
main "$@"
