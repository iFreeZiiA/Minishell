#!/bin/bash

# ============================================================================
# Tests supplémentaires - Cas limites pour détecter crash/undefined behavior
# Version 2 - Amélioration des tests
# ============================================================================

# Configuration
MINISHELL="./minishell"
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

run_strict_test() {
    local command="$1"
    local description="$2"
    local use_timeout="${3:-true}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "Test $TOTAL_TESTS: $description"
    echo "   Command: $command"
    
    # Exécution avec bash (référence)
    if [ "$use_timeout" = "true" ]; then
        bash_output=$(timeout $TIMEOUT bash -c "$command" 2>&1)
        bash_exit=$?
    else
        bash_output=$(echo -e "$command" | bash 2>&1)
        bash_exit=$?
    fi
    
    # Exécution avec minishell
    if [ "$use_timeout" = "true" ]; then
        minishell_output=$(timeout $TIMEOUT bash -c "echo '$command' | $MINISHELL" 2>&1)
        minishell_exit=$?
    else
        minishell_output=$(echo -e "$command" | $MINISHELL 2>&1)
        minishell_exit=$?
    fi
    
    # Vérification des segfaults/crashes
    if [[ $minishell_exit -eq 139 || $minishell_exit -eq 134 ]]; then
        echo -e "   ${RED}❌ CRASH DETECTED${NC} (exit code: $minishell_exit)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    elif [[ $minishell_exit -eq 124 ]]; then
        echo -e "   ${YELLOW}⏰ TIMEOUT${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    else
        echo -e "   ${GREEN}✅ NO CRASH${NC} (exit code: $minishell_exit)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    fi
    echo ""
}

test_crash_candidates() {
    print_section_header "TESTS LIMITES (Crash candidates mais valides selon le sujet)"

    # 🔹 Cas 1 – Redirections mal formées
    run_strict_test ">" "Redirection de sortie sans commande"
    run_strict_test "<" "Redirection d'entrée sans commande"
    run_strict_test "cat <" "cat avec redirection incomplète"
    run_strict_test "echo test >" "echo avec redirection incomplète"

    # 🔹 Cas 2 – Heredoc avec caractères spéciaux / vide
    run_strict_test "<< END" "Heredoc sans contenu" false
    run_strict_test "<< 'END'" "Heredoc avec délimiteur entre quotes" false
    run_strict_test "<< END\necho Hello\nEND" "Heredoc avec contenu"

    # 🔹 Cas 3 – Variables inconnues ou perturbatrices
    run_strict_test "echo \$" "Variable vide \$"
    run_strict_test "echo \$?" "Code retour sans commande"
    run_strict_test "echo \$0123456789" "Variable longue non définie"

    # 🔹 Cas 4 – Pipes mal formés
    run_strict_test "|" "Pipe seul"
    run_strict_test "ls |" "Pipe sans commande à droite"
    run_strict_test "| ls" "Pipe sans commande à gauche"
    run_strict_test "ls ||| grep test" "Pipes multiples invalides"

    # 🔹 Cas 5 – Accès fichiers/dossiers
    run_strict_test "cat no_such_file.txt" "Lecture fichier inexistant"
    run_strict_test "cd /no_such_directory" "CD vers dossier inexistant"
    run_strict_test "chmod 000 test_file.txt && cat test_file.txt" "Lecture fichier sans permission" false

    # 🔹 Cas 6 – Builtins
    run_strict_test "export TEST_VAR=" "Export variable vide"
    run_strict_test "unset TEST_VAR && echo \$TEST_VAR" "Unset d'une variable existante"
    run_strict_test "exit 9999999999" "Exit avec code trop grand"

    # 🔸 Wildcards
    run_strict_test "echo *" "Wildcard simple"
    run_strict_test "echo *.c" "Wildcard filtré par extension"
    run_strict_test "echo .*.c" "Wildcard sur fichiers cachés"

    # 🔸 Opérateurs logiques avec parenthèses
    run_strict_test "(true && false) || echo ok" "Logique avec parenthèses"
    run_strict_test "(nonexistent && echo no) || echo fallback" "Parenthèses avec erreur"
    
    # 🔸 Tests supplémentaires spécifiques
    run_strict_test "echo &&" "Echo suivi de &&"
    run_strict_test "|| echo test" "Commande commençant par ||"
    run_strict_test "echo > > file" "Double redirection"
    run_strict_test "echo < < file" "Double redirection entrée"
    run_strict_test "((echo hello" "Parenthèses non fermées"
    run_strict_test "echo hello))" "Parenthèses en trop"
    run_strict_test "" "Ligne vide"
    run_strict_test "   " "Espaces seulement"
}

# Fonction principale
main() {
    # Vérifier que minishell existe
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}❌ Erreur: $MINISHELL non trouvé${NC}"
        echo "Compilez d'abord avec: make"
        exit 1
    fi
    
    # Créer quelques fichiers de test
    echo "test content" > test_file.txt
    echo "*.c" > file1.txt
    echo "*.h" > file2.txt
    touch .hidden_file.c
    
    echo -e "${CYAN}🧪 TESTS DE ROBUSTESSE v2 - DÉTECTION DE CRASHES${NC}"
    echo -e "${CYAN}====================================================${NC}"
    echo "🎯 Objectif: Vérifier que minishell ne crash pas sur des inputs limites (version étendue)"
    echo ""
    
    # Lancer les tests
    test_crash_candidates
    
    # Afficher les statistiques finales
    print_section_header "RÉSULTATS FINAUX v2"
    echo "📊 STATISTIQUES CRASH TESTS:"
    echo "   Total des tests: $TOTAL_TESTS"
    echo -e "   Tests sans crash: ${GREEN}$PASSED_TESTS${NC}"
    echo -e "   Tests avec problèmes: ${RED}$FAILED_TESTS${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n🎉 ${GREEN}EXCELLENT!${NC} Aucun crash détecté!"
        echo -e "🛡️  Votre minishell est ultra-robuste aux cas limites."
        echo -e "🚀 Prêt pour la correction !"
    else
        echo -e "\n⚠️  ${YELLOW}ATTENTION:${NC} $FAILED_TESTS test(s) avec problèmes détectés."
        echo -e "🔧 Vérifiez la gestion d'erreurs pour ces cas."
    fi
    
    # Nettoyage
    rm -f test_file.txt file1.txt file2.txt .hidden_file.c
    rm -f minishell_output.txt bash_output.txt
    
    echo ""
    
    # Code de sortie basé sur les résultats
    if [ $FAILED_TESTS -eq 0 ]; then
        exit 0
    else
        exit 1
    fi
}

# Lancer le script si exécuté directement
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
