#!/bin/bash

# ============================================================================
# Tests bash de référence - Pour comparer avec minishell
# ============================================================================

# Configuration
TIMEOUT=3
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Fonctions utilitaires
print_section_header() {
    echo ""
    echo "=================================================="
    echo "  $1"
    echo "=================================================="
}

run_bash_test() {
    local command="$1"
    local description="$2"
    local use_timeout="${3:-true}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "Test $TOTAL_TESTS: $description"
    echo "   Command: $command"
    
    # Exécution avec bash
    if [ "$use_timeout" = "true" ]; then
        bash_output=$(timeout $TIMEOUT bash -c "$command" 2>&1)
        bash_exit=$?
    else
        bash_output=$(echo -e "$command" | bash 2>&1)
        bash_exit=$?
    fi
    
    # Vérification des segfaults/crashes
    if [[ $bash_exit -eq 139 || $bash_exit -eq 134 ]]; then
        echo -e "   ${RED}❌ BASH CRASH${NC} (exit code: $bash_exit)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    elif [[ $bash_exit -eq 124 ]]; then
        echo -e "   ${YELLOW}⏰ BASH TIMEOUT${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    else
        echo -e "   ${GREEN}✅ BASH OK${NC} (exit code: $bash_exit)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    fi
    echo ""
}

test_bash_reference() {
    print_section_header "TESTS BASH DE RÉFÉRENCE"

    # 🔹 Cas 1 – Redirections mal formées
    run_bash_test ">" "Redirection de sortie sans commande"
    run_bash_test "<" "Redirection d'entrée sans commande"
    run_bash_test "cat <" "cat avec redirection incomplète"
    run_bash_test "echo test >" "echo avec redirection incomplète"

    # 🔹 Cas 2 – Heredoc avec caractères spéciaux / vide
    run_bash_test "<< END" "Heredoc sans contenu" false
    run_bash_test "<< 'END'" "Heredoc avec délimiteur entre quotes" false
    run_bash_test "<< END\necho Hello\nEND" "Heredoc avec contenu"

    # 🔹 Cas 3 – Variables inconnues ou perturbatrices
    run_bash_test "echo \$" "Variable vide \$"
    run_bash_test "echo \$?" "Code retour sans commande"
    run_bash_test "echo \$0123456789" "Variable longue non définie"

    # 🔹 Cas 4 – Pipes mal formés
    run_bash_test "|" "Pipe seul"
    run_bash_test "ls |" "Pipe sans commande à droite"
    run_bash_test "| ls" "Pipe sans commande à gauche"
    run_bash_test "ls ||| grep test" "Pipes multiples invalides"

    # 🔹 Cas 5 – Accès fichiers/dossiers
    run_bash_test "cat no_such_file.txt" "Lecture fichier inexistant"
    run_bash_test "cd /no_such_directory" "CD vers dossier inexistant"
    run_bash_test "chmod 000 test_file.txt && cat test_file.txt" "Lecture fichier sans permission" false

    # 🔹 Cas 6 – Builtins
    run_bash_test "export TEST_VAR=" "Export variable vide"
    run_bash_test "unset TEST_VAR && echo \$TEST_VAR" "Unset d'une variable existante"
    run_bash_test "exit 9999999999" "Exit avec code trop grand"

    # 🔸 Wildcards
    run_bash_test "echo *" "Wildcard simple"
    run_bash_test "echo *.c" "Wildcard filtré par extension"
    run_bash_test "echo .*.c" "Wildcard sur fichiers cachés"

    # 🔸 Opérateurs logiques avec parenthèses
    run_bash_test "(true && false) || echo ok" "Logique avec parenthèses"
    run_bash_test "(nonexistent && echo no) || echo fallback" "Parenthèses avec erreur"
    
    # 🔸 Tests supplémentaires spécifiques
    run_bash_test "echo &&" "Echo suivi de &&"
    run_bash_test "|| echo test" "Commande commençant par ||"
    run_bash_test "echo > > file" "Double redirection"
    run_bash_test "echo < < file" "Double redirection entrée"
    run_bash_test "((echo hello" "Parenthèses non fermées"
    run_bash_test "echo hello))" "Parenthèses en trop"
    run_bash_test "" "Ligne vide"
    run_bash_test "   " "Espaces seulement"
}

# Fonction principale
main() {
    # Créer quelques fichiers de test
    echo "test content" > test_file.txt
    echo "*.c" > file1.txt
    echo "*.h" > file2.txt
    touch .hidden_file.c
    
    echo -e "${CYAN}🧪 TESTS BASH DE RÉFÉRENCE - COMPORTEMENT ATTENDU${NC}"
    echo -e "${CYAN}====================================================${NC}"
    echo "🎯 Objectif: Voir comment bash gère les mêmes cas limites"
    echo ""
    
    # Lancer les tests
    test_bash_reference
    
    # Afficher les statistiques finales
    print_section_header "RÉSULTATS BASH RÉFÉRENCE"
    echo "📊 STATISTIQUES BASH:"
    echo "   Total des tests: $TOTAL_TESTS"
    echo -e "   Tests bash OK: ${GREEN}$PASSED_TESTS${NC}"
    echo -e "   Tests bash problèmes: ${RED}$FAILED_TESTS${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n🎉 ${GREEN}BASH RÉFÉRENCE:${NC} Tous les tests passent!"
        echo -e "📋 Votre minishell doit reproduire ce comportement."
    else
        echo -e "\n⚠️  ${YELLOW}ATTENTION:${NC} $FAILED_TESTS test(s) bash avec problèmes."
        echo -e "🔧 Ces cas peuvent être problématiques."
    fi
    
    # Nettoyage
    rm -f test_file.txt file1.txt file2.txt .hidden_file.c
    rm -f file
    
    echo ""
}

# Lancer le script si exécuté directement
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
