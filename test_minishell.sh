#!/bin/bash

# ============================================================================
# Script de Test Complet pour Minishell - Toutes les Phases
# ============================================================================

# Configuration des couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configuration
MINISHELL="./minishell"
TIMEOUT_CMD="timeout 5s"
TEMP_DIR="./test_temp"
TEST_FILES_DIR="$TEMP_DIR/files"

# Compteurs globaux
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
CURRENT_PHASE=""

# ============================================================================
# FONCTIONS UTILITAIRES
# ============================================================================

setup_test_environment() {
    echo -e "${BLUE}🔧 Configuration de l'environnement de test...${NC}"
    
    # Sauvegarder le répertoire original
    ORIGINAL_DIR=$(pwd)
    
    # Créer dossiers temporaires
    mkdir -p "$TEMP_DIR" "$TEST_FILES_DIR" 2>/dev/null
    
    # Créer fichiers test pour wildcards
    touch "$TEST_FILES_DIR/test1.txt" "$TEST_FILES_DIR/test2.txt" 2>/dev/null
    touch "$TEST_FILES_DIR/main.c" "$TEST_FILES_DIR/utils.c" 2>/dev/null
    touch "$TEST_FILES_DIR/header.h" "$TEST_FILES_DIR/readme.md" 2>/dev/null
    
    # Variables d'environnement pour les tests
    export TEST_VAR="hello"
    export COMPLEX_VAR="integration"
    export USER="${USER:-testuser}"
    export HOME="${HOME:-/home/testuser}"
    
    # Créer fichiers d'entrée pour tests de redirection
    echo "test input data" > "$TEMP_DIR/input.txt"
    echo -e "line1\nline2\nline3" > "$TEMP_DIR/multiline.txt"
    
    cd "$TEST_FILES_DIR" 2>/dev/null || echo "Warning: Could not cd to test files directory"
}

cleanup_test_environment() {
    echo -e "${BLUE}🧹 Nettoyage de l'environnement de test...${NC}"
    cd - >/dev/null 2>&1
    rm -rf "$TEMP_DIR" 2>/dev/null
    unset TEST_VAR COMPLEX_VAR
}

print_phase_header() {
    local phase_name="$1"
    local phase_description="$2"
    
    CURRENT_PHASE="$phase_name"
    echo ""
    echo -e "${PURPLE}================================================${NC}"
    echo -e "${PURPLE}  $phase_name: $phase_description${NC}"
    echo -e "${PURPLE}================================================${NC}"
}

run_test() {
    local test_name="$1"
    local command="$2"
    local expected_success="$3"  # true/false
    local description="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"
    if [ -n "$description" ]; then
        echo -e "${CYAN}   → $description${NC}"
    fi
    echo -e "${BLUE}   Command: $command${NC}"
    
    # Exécuter la commande avec timeout
    # Utiliser le chemin absolu vers minishell depuis n'importe quel répertoire
    echo -e "$command\nexit" | timeout 5s "$ORIGINAL_DIR/minishell" >/dev/null 2>&1
    local exit_code=$?
    
    # Vérifier le résultat
    if [ "$expected_success" = "true" ]; then
        if [ $exit_code -eq 0 ]; then
            echo -e "${GREEN}   ✅ PASSED${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}   ❌ FAILED (exit code: $exit_code)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    else
        if [ $exit_code -ne 0 ]; then
            echo -e "${GREEN}   ✅ PASSED (correctly failed)${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}   ❌ FAILED (should have failed)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    fi
    echo ""
}

print_phase_summary() {
    local phase_tests_start="$1"
    local phase_passed=$((PASSED_TESTS - phase_tests_start))
    local phase_total=$((TOTAL_TESTS - phase_tests_start))
    
    echo -e "${BLUE}--- Résumé $CURRENT_PHASE ---${NC}"
    echo -e "${YELLOW}Tests réussis: $phase_passed/$phase_total${NC}"
    
    if [ $phase_passed -eq $phase_total ]; then
        echo -e "${GREEN}🎯 Phase $CURRENT_PHASE: RÉUSSIE${NC}"
    else
        local phase_failed=$((phase_total - phase_passed))
        echo -e "${RED}⚠️  Phase $CURRENT_PHASE: $phase_failed test(s) échoué(s)${NC}"
    fi
}

# ============================================================================
# TESTS PAR PHASE
# ============================================================================

test_phase_2() {
    print_phase_header "PHASE 2" "Tokenisation de Base"
    local start_tests=$PASSED_TESTS
    
    # Tests Simples
    run_test "Commande basique" "echo hello" true "Tokenisation simple"
    run_test "Commande avec argument" "ls" true "Commande sans arguments"
    run_test "Commande système" "pwd" true "Commande système basique"
    
    # Tests Intermédiaires
    run_test "Plusieurs arguments" "echo hello world" true "Tokenisation multiple mots"
    run_test "Commande avec options" "ls -la" true "Options avec tirets"
    run_test "Espaces multiples" "echo    hello    world" true "Gestion espaces multiples"
    run_test "Pipe simple" "ls | grep txt" true "Tokenisation avec pipe"
    
    # Tests Complexes (Limites)
    run_test "Pipe multiple" "echo hello | cat | wc -l | head -n 1" true "Chaîne de pipes"
    run_test "Redirections multiples" "ls -la > output < input | grep test" true "Tokens de redirection"
    run_test "Commande très longue" "command1 arg1 arg2 | command2 | command3 arg3" true "Tokenisation complexe"
    
    print_phase_summary $start_tests
}

test_phase_3() {
    print_phase_header "PHASE 3" "Quotes et Expansion"
    local start_tests=$PASSED_TESTS
    
    # Tests Simples
    run_test "Quote simple" "echo 'simple quote'" true "Quotes simples basiques"
    run_test "Quote double" "echo \"double quote\"" true "Quotes doubles basiques"
    run_test "Expansion variable" "echo \$TEST_VAR" true "Expansion variable simple"
    
    # Tests Intermédiaires
    run_test "Pas d'expansion en quote simple" "echo 'no expansion: \$TEST_VAR'" true "Protection par quotes simples"
    run_test "Expansion en quote double" "echo \"expansion: \$TEST_VAR\"" true "Expansion dans quotes doubles"
    run_test "Code de sortie" "echo \$?" true "Expansion code de sortie"
    run_test "Variable HOME" "echo \$HOME" true "Variable d'environnement"
    
    # Tests Complexes (Limites)
    run_test "Expansion complexe" "echo \"Complex: \$USER at \$HOME with code \$?\"" true "Multiples expansions"
    run_test "Quotes mixtes" "echo 'Mixed' \"quotes\" together" true "Combinaison de quotes"
    run_test "Expansion avec pipe" "echo \"\$TEST_VAR: \$HOME/\$USER\" | cat" true "Expansion + pipe"
    run_test "Différence quotes" "echo '\$HOME' and \"\$HOME\" different" true "Comportement différent quotes"
    
    print_phase_summary $start_tests
}

test_phase_4() {
    print_phase_header "PHASE 4" "Opérateurs Avancés"
    local start_tests=$PASSED_TESTS
    
    # Tests Simples
    run_test "Append" "echo hello >> file.txt" true "Opérateur append"
    run_test "AND simple" "true && echo success" true "Opérateur logique AND"
    run_test "OR simple" "false || echo backup" true "Opérateur logique OR"
    
    # Tests Intermédiaires
    run_test "Chaîne logique" "echo test && echo success || echo failed" true "AND puis OR"
    run_test "Condition avec ls" "ls -la && echo found || echo not_found" true "Commande réelle avec logique"
    run_test "Append multiple" "echo line1 >> file && echo line2 >> file" true "Multiples append"
    
    # Tests Complexes (Limites)
    run_test "Chaîne AND longue" "true && echo step1 && echo step2 && echo step3" true "Multiples AND"
    run_test "Chaîne OR longue" "false || echo backup1 || echo backup2 || echo final" true "Multiples OR"
    run_test "Logique avec redirections" "echo success && echo log >> logfile || echo error >> error.log" true "Logique + redirections"
    
    print_phase_summary $start_tests
}

test_phase_5_6() {
    print_phase_header "PHASE 5-6" "Parsing et Pipes"
    local start_tests=$PASSED_TESTS
    
    # Tests Simples
    run_test "Pipe basique" "echo hello | cat" true "Pipe simple"
    run_test "Pipe avec head" "ls | head -5" true "Pipe avec limitation"
    run_test "Redirection sortie" "echo test > file.txt" true "Redirection basique"
    
    # Tests Intermédiaires
    run_test "Triple pipe" "echo hello | cat | wc -c" true "Pipes multiples"
    run_test "Pipe avec grep" "ls -la | grep .txt | wc -l" true "Filtrage par pipe"
    run_test "Pipe vers fichier" "echo data | cat > output.txt" true "Pipe + redirection"
    
    # Tests Complexes (Limites)
    run_test "Chaîne pipes longue" "ls -la | grep .c | head -10 | tail -5 | wc -l" true "Longue chaîne de pipes"
    run_test "Pipes profonds" "echo test | cat | cat | cat | cat > deep_pipe.txt" true "Pipes répétitifs"
    run_test "Pipeline complet" "echo input | grep pattern | sort | uniq > result.txt" true "Pipeline de traitement"
    
    print_phase_summary $start_tests
}

test_phase_7() {
    print_phase_header "PHASE 7" "Opérateurs Logiques"
    local start_tests=$PASSED_TESTS
    
    # Tests Simples
    run_test "AND basique" "echo a && echo b" true "AND simple"
    run_test "OR basique" "echo a || echo b" true "OR simple"
    run_test "AND avec true" "true && echo success" true "Condition vraie"
    
    # Tests Intermédiaires
    run_test "AND multiple" "echo step1 && echo step2 && echo step3" true "Chaîne AND"
    run_test "OR multiple" "false || echo plan_b || echo plan_c" true "Chaîne OR"
    run_test "Test fichier" "ls file.txt && echo found || echo missing" true "Test existence fichier"
    
    # Tests Complexes (Limites)
    run_test "Longue chaîne AND" "echo a && echo b && echo c && echo d && echo e" true "AND très long"
    run_test "OR jusqu'au succès" "false || false || false || true && echo finally" true "OR puis AND"
    run_test "Logique complexe" "true && echo ok || echo backup && echo always" true "Combinaison complexe"
    
    print_phase_summary $start_tests
}

test_phase_8() {
    print_phase_header "PHASE 8" "Parenthèses"
    local start_tests=$PASSED_TESTS
    
    # Tests Simples
    run_test "Groupe simple" "(echo hello)" true "Parenthèses basiques"
    run_test "Groupe ls" "(ls)" true "Commande groupée"
    run_test "Groupe avec pipe" "(echo test) | cat" true "Groupe vers pipe"
    
    # Tests Intermédiaires
    run_test "Groupe avec AND" "(echo a && echo b)" true "Logique groupée"
    run_test "Groupe avec OR" "(echo test) || echo backup" true "Groupe avec alternative"
    run_test "Groupe pipe interne" "(ls -la) | grep .txt" true "Pipe depuis groupe"
    
    # Tests Complexes (Limites)
    run_test "Groupes multiples" "(echo a && echo b) || (echo c && echo d)" true "Multiples groupes"
    run_test "Parenthèses imbriquées" "((echo nested) && echo works)" true "Imbrication"
    run_test "Groupes avec pipes" "(echo group | cat) && (echo another | wc -l)" true "Groupes complexes"
    
    print_phase_summary $start_tests
}

test_phase_9() {
    print_phase_header "PHASE 9" "Wildcards"
    local start_tests=$PASSED_TESTS
    
    # Tests Simples
    run_test "Wildcard txt" "echo *.txt" true "Pattern .txt"
    run_test "Wildcard c" "echo *.c" true "Pattern .c"
    run_test "Wildcard h" "echo *.h" true "Pattern .h"
    
    # Tests Intermédiaires
    run_test "Wildcard avec pipe" "ls *.txt | wc -l" true "Wildcard + pipe"
    run_test "Pattern test" "echo test* | cat" true "Pattern préfixe"
    run_test "Cat wildcard" "echo 'test content' > test.txt && cat *.txt" true "Lecture par wildcard"
    
    # Tests Complexes (Limites)
    run_test "Multiples wildcards" "echo *.txt *.c *.h | tr ' ' '\n' | sort" true "Multiples patterns"
    run_test "Wildcard filtré" "ls *.* | head -5" true "Wildcard avec limitation"
    run_test "Compter fichiers" "echo * | wc -w" true "Tous les fichiers"
    
    print_phase_summary $start_tests
}

test_phase_11() {
    print_phase_header "PHASE 11" "Intégration Complexe"
    local start_tests=$PASSED_TESTS
    
    # Tests Simples
    run_test "Variable complexe" "echo \$COMPLEX_VAR && echo success" true "Variable + logique"
    
    # Tests Intermédiaires
    run_test "Multiples variables" "echo \$HOME | cat && echo \$USER || echo unknown" true "Variables + pipes + logique"
    run_test "PATH processing" "(echo \$PATH) | head -1" true "Variable système groupée"
    
    # Tests Complexes (Limites Absolues)
    run_test "Test ultime" "echo \"Start: \$TEST_VAR\" | cat && echo \"User: \$USER\" || echo \"Failed: \$?\"" true "Intégration maximale"
    run_test "Groupes imbriqués complexes" "((echo nested && echo groups) | cat) && (echo success || echo failed)" true "Syntaxe complexe maximale"
    run_test "Expansion maximale" "echo \"User \$USER in \$HOME\" | cat | head -1" true "Expansion + pipes"
    
    print_phase_summary $start_tests
}

test_error_handling() {
    print_phase_header "ROBUSTESSE" "Gestion d'Erreurs"
    local start_tests=$PASSED_TESTS
    
    # Tests d'Erreurs Simples
    run_test "Input vide" "" false "Commande vide"
    run_test "Espaces seulement" "   " false "Espaces uniquement"
    run_test "Quote non fermée double" "echo \"unclosed" false "Quote double non fermée"
    run_test "Quote non fermée simple" "echo 'unclosed" false "Quote simple non fermée"
    
    # Tests d'Erreurs Complexes
    run_test "AND en fin" "echo hello &&" false "Opérateur AND mal placé"
    run_test "OR au début" "|| echo invalid" false "Opérateur OR mal placé"
    run_test "Pipe en fin" "echo hello |" false "Pipe mal placé"
    run_test "Parenthèse non fermée" "((echo unbalanced" false "Parenthèses déséquilibrées"
    run_test "Parenthèse fermante seule" "echo hello)" false "Parenthèse fermante orpheline"
    run_test "Opérateurs multiples" "echo hello |||" false "Opérateurs malformés"
    
    print_phase_summary $start_tests
}

# ============================================================================
# FONCTION PRINCIPALE
# ============================================================================

print_header() {
    echo -e "${BLUE}============================================================================${NC}"
    echo -e "${BLUE}                    SCRIPT DE TEST COMPLET MINISHELL${NC}"
    echo -e "${BLUE}                          Toutes les Phases${NC}"
    echo -e "${BLUE}============================================================================${NC}"
    echo ""
}

print_final_summary() {
    echo ""
    echo -e "${BLUE}============================================================================${NC}"
    echo -e "${BLUE}                           RÉSUMÉ FINAL${NC}"
    echo -e "${BLUE}============================================================================${NC}"
    echo ""
    echo -e "${YELLOW}📊 STATISTIQUES GLOBALES:${NC}"
    echo -e "   Total des tests: $TOTAL_TESTS"
    echo -e "   Tests réussis: ${GREEN}$PASSED_TESTS${NC}"
    echo -e "   Tests échoués: ${RED}$FAILED_TESTS${NC}"
    echo ""
    
    local success_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "${YELLOW}📈 Taux de réussite: $success_rate%${NC}"
    echo ""
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "${GREEN}🎉 FÉLICITATIONS! TOUS LES TESTS SONT PASSÉS!${NC}"
        echo -e "${GREEN}🚀 Votre minishell est entièrement fonctionnel!${NC}"
        return 0
    elif [ $success_rate -ge 80 ]; then
        echo -e "${YELLOW}✅ Excellent travail! Plus de 80% de réussite.${NC}"
        echo -e "${YELLOW}🔧 Quelques ajustements mineurs nécessaires.${NC}"
        return 1
    elif [ $success_rate -ge 60 ]; then
        echo -e "${YELLOW}⚠️  Bon travail, mais des améliorations sont nécessaires.${NC}"
        echo -e "${YELLOW}🛠️  Concentrez-vous sur les phases qui échouent.${NC}"
        return 2
    else
        echo -e "${RED}❌ Des corrections importantes sont nécessaires.${NC}"
        echo -e "${RED}🔨 Reprenez les phases de base avant de continuer.${NC}"
        return 3
    fi
}

main() {
    # Vérifier la présence du minishell
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}❌ Erreur: $MINISHELL non trouvé${NC}"
        echo -e "${YELLOW}💡 Compilez d'abord avec: make${NC}"
        exit 1
    fi
    
    # Rendre exécutable
    chmod +x "$MINISHELL" 2>/dev/null
    
    print_header
    setup_test_environment
    
    # Gestion des arguments
    if [ $# -eq 1 ]; then
        case "$1" in
            "2") test_phase_2 ;;
            "3") test_phase_3 ;;
            "4") test_phase_4 ;;
            "5"|"6") test_phase_5_6 ;;
            "7") test_phase_7 ;;
            "8") test_phase_8 ;;
            "9") test_phase_9 ;;
            "11") test_phase_11 ;;
            "error"|"robustness") test_error_handling ;;
            *)
                echo -e "${RED}Phase inconnue: $1${NC}"
                echo -e "${YELLOW}Phases disponibles: 2, 3, 4, 5, 6, 7, 8, 9, 11, error${NC}"
                cleanup_test_environment
                exit 1
                ;;
        esac
    else
        # Exécuter tous les tests
        echo -e "${CYAN}🚀 Lancement de tous les tests...${NC}"
        test_phase_2
        test_phase_3
        test_phase_4
        test_phase_5_6
        test_phase_7
        test_phase_8
        test_phase_9
        test_phase_11
        test_error_handling
    fi
    
    print_final_summary
    local exit_code=$?
    
    cleanup_test_environment
    exit $exit_code
}

# Gestion des signaux pour cleanup
trap cleanup_test_environment EXIT

# Lancer le script
main "$@"