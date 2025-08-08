#!/bin/bash

# ============================================================================
# Comparaison BASH vs MINISHELL - Analyse des différences
# ============================================================================

# Configuration
MINISHELL="./minishell"
TIMEOUT=3
TOTAL_TESTS=0
IDENTICAL_TESTS=0
DIFFERENT_TESTS=0

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

print_section_header() {
    echo ""
    echo "=================================================================="
    echo "  $1"
    echo "=================================================================="
}

compare_test() {
    local command="$1"
    local description="$2"
    local use_timeout="${3:-true}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo ""
    echo -e "${CYAN}Test $TOTAL_TESTS: $description${NC}"
    echo "Command: $command"
    echo "---"
    
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
    
    # Affichage des résultats
    echo -e "${BLUE}BASH:${NC}"
    echo "   Exit code: $bash_exit"
    if [ -n "$bash_output" ]; then
        echo "   Output: $bash_output"
    else
        echo "   Output: (vide)"
    fi
    
    echo -e "${PURPLE}MINISHELL:${NC}"
    echo "   Exit code: $minishell_exit"
    if [ -n "$minishell_output" ]; then
        echo "   Output: $minishell_output"
    else
        echo "   Output: (vide)"
    fi
    
    # Comparaison
    if [[ $bash_exit -eq $minishell_exit ]]; then
        echo -e "${GREEN}✅ CODES DE SORTIE IDENTIQUES${NC}"
        IDENTICAL_TESTS=$((IDENTICAL_TESTS + 1))
    else
        echo -e "${YELLOW}⚠️  CODES DE SORTIE DIFFÉRENTS${NC}"
        DIFFERENT_TESTS=$((DIFFERENT_TESTS + 1))
    fi
    
    # Vérification des crashes
    if [[ $minishell_exit -eq 139 || $minishell_exit -eq 134 ]]; then
        echo -e "${RED}💥 CRASH MINISHELL DÉTECTÉ!${NC}"
    fi
}

test_comparison() {
    print_section_header "COMPARAISON BASH vs MINISHELL"

    # Tests les plus critiques
    compare_test ">" "Redirection sortie sans commande"
    compare_test "|" "Pipe seul"
    compare_test "echo &&" "Echo suivi de &&"
    compare_test "|| echo test" "Commande commençant par ||"
    compare_test "ls ||| grep test" "Pipes multiples invalides"
    compare_test "echo > > file" "Double redirection"
    compare_test "((echo hello" "Parenthèses non fermées"
    compare_test "echo hello))" "Parenthèses en trop"
    
    # Tests d'erreurs normales
    compare_test "cat no_such_file.txt" "Fichier inexistant"
    compare_test "cd /no_such_directory" "Dossier inexistant"
    compare_test "exit 9999999999" "Exit avec code énorme"
    
    # Tests de fonctionnalités
    compare_test "echo \$?" "Variable \$?"
    compare_test "echo *" "Wildcard simple"
    compare_test "(true && false) || echo ok" "Logique avec parenthèses"
    compare_test "" "Ligne vide"
    compare_test "   " "Espaces seulement"
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
    touch .hidden_file.c
    
    echo -e "${CYAN}🔍 COMPARAISON DÉTAILLÉE BASH vs MINISHELL${NC}"
    echo -e "${CYAN}============================================${NC}"
    echo "🎯 Objectif: Comparer comportement exact bash vs minishell"
    echo ""
    
    # Lancer les tests
    test_comparison
    
    # Afficher les statistiques finales
    print_section_header "ANALYSE COMPARATIVE FINALE"
    echo "📊 STATISTIQUES DE COMPARAISON:"
    echo "   Total des tests: $TOTAL_TESTS"
    echo -e "   Comportements identiques: ${GREEN}$IDENTICAL_TESTS${NC}"
    echo -e "   Comportements différents: ${YELLOW}$DIFFERENT_TESTS${NC}"
    
    compatibility_percentage=$((IDENTICAL_TESTS * 100 / TOTAL_TESTS))
    
    echo ""
    echo -e "🎯 ${CYAN}TAUX DE COMPATIBILITÉ BASH: $compatibility_percentage%${NC}"
    
    if [ $compatibility_percentage -ge 90 ]; then
        echo -e "🎉 ${GREEN}EXCELLENT!${NC} Très haute compatibilité avec bash"
    elif [ $compatibility_percentage -ge 75 ]; then
        echo -e "👍 ${YELLOW}BON!${NC} Bonne compatibilité avec bash"
    else
        echo -e "⚠️  ${YELLOW}ATTENTION:${NC} Quelques différences avec bash à vérifier"
    fi
    
    # Nettoyage
    rm -f test_file.txt .hidden_file.c file
    
    echo ""
}

# Lancer le script si exécuté directement
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
