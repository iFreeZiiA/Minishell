#!/bin/bash

# ============================================================================
# Script de Test ULTRA-STRICT - Comparaison exacte avec bash
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
TEMP_DIR="./test_strict_temp"
ORIGINAL_DIR=$(pwd)

# Compteurs globaux
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
CURRENT_SECTION=""

# ============================================================================
# FONCTIONS UTILITAIRES ULTRA-STRICTES
# ============================================================================

setup_environment() {
    echo -e "${BLUE}🔧 Configuration de l'environnement STRICT...${NC}"
    
    # Créer dossiers temporaires
    mkdir -p "$TEMP_DIR" 2>/dev/null
    cd "$TEMP_DIR" 2>/dev/null || {
        echo -e "${RED}❌ Erreur: Impossible de créer le répertoire de test${NC}"
        exit 1
    }
    
    # Créer fichiers de test identiques pour bash et minishell
    echo "Test content" > test_file.txt
    touch main.c parser.c
    
    # Variables d'environnement identiques
    export TEST_VAR="hello_world"
    export USER="${USER:-testuser}"
    export HOME="${HOME:-/home/testuser}"
    
    echo -e "${GREEN}✓ Environnement STRICT configuré${NC}"
}

cleanup_environment() {
    echo -e "${BLUE}🧹 Nettoyage...${NC}"
    cd "$ORIGINAL_DIR" 2>/dev/null
    rm -rf "$TEMP_DIR" 2>/dev/null
}

print_section_header() {
    local section_name="$1"
    
    CURRENT_SECTION="$section_name"
    echo ""
    echo -e "${PURPLE}================================================${NC}"
    echo -e "${PURPLE}  $section_name${NC}"
    echo -e "${PURPLE}================================================${NC}"
}

# Fonction ULTRA-STRICTE pour comparer avec bash
run_strict_test() {
    local command="$1"
    local description="$2"
    local test_exit_code="${3:-true}"  # true = tester aussi le code de sortie
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test $TOTAL_TESTS: $description${NC}"
    echo -e "${CYAN}   Command: $command${NC}"
    
    # Créer des fichiers temporaires pour capturer les sorties
    local bash_output="bash_output_$TOTAL_TESTS.txt"
    local bash_error="bash_error_$TOTAL_TESTS.txt"
    local minishell_output="minishell_output_$TOTAL_TESTS.txt"
    local minishell_error="minishell_error_$TOTAL_TESTS.txt"
    
    # Exécuter avec bash
    echo -e "$command\necho EXIT_CODE:\$?\nexit" | $TIMEOUT_CMD bash > "$bash_output" 2> "$bash_error"
    local bash_exit_code=$?

    # Exécuter avec minishell et nettoyer directement  
    # 1. Supprimer les codes ANSI de couleur
    # 2. Supprimer les lignes avec prompts
    # 3. Garder seulement les sorties de commandes
    echo -e "$command\necho EXIT_CODE:\$?\nexit" | $TIMEOUT_CMD "$ORIGINAL_DIR/$MINISHELL" 2> "$minishell_error" | \
    sed 's/\x1b\[[0-9;]*m//g' | \
    grep -v "^minishell" | \
    grep -v "^exit$" | \
    sed '/^[[:space:]]*$/d' > "$minishell_output"
    local minishell_exit_code=$?

    # Variables pour les résultats
    local output_match=true
    local error_match=true
    local exit_code_match=true
    local overall_match=true

    # Comparer les sorties standard
    if ! diff -q "$bash_output" "$minishell_output" >/dev/null 2>&1; then
        output_match=false
        overall_match=false
        echo -e "${RED}   ❌ SORTIE DIFFÉRENTE:${NC}"
        echo -e "${YELLOW}   BASH:${NC}"
        cat "$bash_output" | head -3
        echo -e "${YELLOW}   MINISHELL:${NC}"  
        cat "$minishell_output" | head -3
        echo -e "${YELLOW}   ---${NC}"
    fi
    
    # Comparer les erreurs (en ignorant les détails de format)
    # Seulement vérifier la présence d'erreurs similaires
    local bash_has_error=false
    local minishell_has_error=false
    
    if [ -s "$bash_error" ]; then
        bash_has_error=true
    fi
    if [ -s "$minishell_error" ]; then
        minishell_has_error=true
    fi
    
    # Pour les erreurs, on vérifie seulement qu'il y a erreur ou pas
    # (le format exact peut différer)
    if [ "$bash_has_error" != "$minishell_has_error" ]; then
        error_match=false
        overall_match=false
        echo -e "${RED}   ❌ ERREURS DIFFÉRENTES:${NC}"
        echo -e "${YELLOW}   BASH erreur: $bash_has_error, MINISHELL erreur: $minishell_has_error${NC}"
    fi
    
    # Comparer les codes de sortie si demandé
    if [ "$test_exit_code" = "true" ]; then
        # Extraire le code de sortie avec regex pour éviter $? non expandé
        local bash_reported_exit=$(grep -o "EXIT_CODE:[0-9]*" "$bash_output" 2>/dev/null | cut -d: -f2 | tail -1)
        local minishell_reported_exit=$(grep -o "EXIT_CODE:[0-9]*" "$minishell_output" 2>/dev/null | cut -d: -f2 | tail -1)
        
        if [ "$bash_reported_exit" != "$minishell_reported_exit" ]; then
            exit_code_match=false
            overall_match=false
            echo -e "${RED}   ❌ CODE DE SORTIE DIFFÉRENT:${NC}"
            echo -e "${YELLOW}   BASH \$?: $bash_reported_exit, MINISHELL \$?: $minishell_reported_exit${NC}"
        fi
    fi
    
    # Nettoyer les fichiers temporaires
    rm -f "$bash_output" "$bash_error" "$minishell_output" "$minishell_error" 2>/dev/null
    
    # Résultat final
    if [ "$overall_match" = "true" ]; then
        echo -e "${GREEN}   ✅ PASSED (sortie identique à bash)${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}   ❌ FAILED (différences avec bash)${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    echo ""
}

# ============================================================================
# TESTS ULTRA-STRICTS
# ============================================================================

test_command_not_found() {
    print_section_header "COMMANDES INEXISTANTES (Ultra-Strict)"
    
    run_strict_test "gdsfhsgfd" "Commande inexistante simple"
    run_strict_test "gdsfhsgfd && echo after" "Commande inexistante avec &&"
    run_strict_test "nonexistent || echo fallback" "Commande inexistante avec ||" 
    run_strict_test "/bin/nonexistent" "Chemin absolu inexistant"
}

test_exit_codes() {
    print_section_header "CODES DE SORTIE (Ultra-Strict)"
    
    run_strict_test "true" "Commande true"
    run_strict_test "false" "Commande false"
    run_strict_test "/bin/false" "False avec chemin absolu"
    run_strict_test "ls /nonexistent" "ls sur fichier inexistant"
    run_strict_test "echo test" "Echo simple (doit être 0)"
}

test_logical_operators_strict() {
    print_section_header "OPÉRATEURS LOGIQUES (Ultra-Strict)"
    
    run_strict_test "true && echo success" "AND avec true"
    run_strict_test "false && echo never" "AND avec false"
    run_strict_test "true || echo never" "OR avec true" 
    run_strict_test "false || echo fallback" "OR avec false"
    run_strict_test "echo hello && echo world" "AND avec echo"
    run_strict_test "nonexistent && echo after" "AND après erreur"
    run_strict_test "ls /nonexistent || echo backup" "OR après erreur"
}

test_pipes_strict() {
    print_section_header "PIPES (Ultra-Strict)"
    
    run_strict_test "echo hello | cat" "Pipe simple"
    run_strict_test "ls | head -n 2" "Pipe avec head"
    run_strict_test "echo test | grep test" "Pipe avec grep (match)"
    run_strict_test "echo test | grep nomatch" "Pipe avec grep (no match)"
    run_strict_test "nonexistent | cat" "Pipe avec commande inexistante"
}

test_echo_strict() {
    print_section_header "ECHO (Ultra-Strict)"
    
    run_strict_test "echo" "Echo sans arguments"
    run_strict_test "echo hello" "Echo simple"
    run_strict_test "echo hello world" "Echo multiples mots"
    run_strict_test "echo -n hello" "Echo avec -n"
    run_strict_test "echo -n -n hello" "Echo multiples -n"
}

test_variables_strict() {
    print_section_header "VARIABLES (Ultra-Strict)"
    
    run_strict_test "echo \$HOME" "Variable HOME"
    run_strict_test "echo \$USER" "Variable USER"
    run_strict_test "echo \$?" "Code de sortie initial"
    run_strict_test "echo \$NONEXISTENT" "Variable inexistante"
    
    # Test séquences pour $?
    run_strict_test "true; echo \$?" "Code après true"
    run_strict_test "false; echo \$?" "Code après false"
}

test_redirections_strict() {
    print_section_header "REDIRECTIONS (Ultra-Strict)"
    
    run_strict_test "echo test > output.txt; cat output.txt" "Redirection simple"
    run_strict_test "echo append >> output.txt; cat output.txt" "Redirection append"
    run_strict_test "cat < test_file.txt" "Redirection entrée"
    run_strict_test "cat < nonexistent.txt" "Redirection fichier inexistant"
}

# ============================================================================
# FONCTION PRINCIPALE
# ============================================================================

print_header() {
    echo -e "${BLUE}============================================================================${NC}"
    echo -e "${BLUE}                    TEST ULTRA-STRICT vs BASH${NC}"
    echo -e "${BLUE}                   Comparaison Exacte Requise${NC}"
    echo -e "${BLUE}============================================================================${NC}"
    echo ""
    echo -e "${RED}⚠️  ATTENTION: Ces tests sont ULTRA-STRICTS${NC}"
    echo -e "${RED}    Sortie et codes de sortie doivent être IDENTIQUES à bash${NC}"
    echo ""
}

print_final_summary() {
    echo ""
    echo -e "${BLUE}============================================================================${NC}"
    echo -e "${BLUE}                       RÉSUMÉ ULTRA-STRICT${NC}"
    echo -e "${BLUE}============================================================================${NC}"
    echo ""
    echo -e "${YELLOW}📊 STATISTIQUES ULTRA-STRICTES:${NC}"
    echo -e "   Total des tests: $TOTAL_TESTS"
    echo -e "   Tests réussis: ${GREEN}$PASSED_TESTS${NC}"
    echo -e "   Tests échoués: ${RED}$FAILED_TESTS${NC}"
    echo ""
    
    if [ $TOTAL_TESTS -eq 0 ]; then
        echo -e "${RED}❌ Aucun test exécuté${NC}"
        return 1
    fi
    
    local success_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "${YELLOW}📈 Taux de réussite STRICT: $success_rate%${NC}"
    echo ""
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "${GREEN}🎉 INCROYABLE! Comportement IDENTIQUE à bash!${NC}"
        echo -e "${GREEN}🚀 Votre minishell est parfaitement compatible!${NC}"
        return 0
    elif [ $success_rate -ge 95 ]; then
        echo -e "${GREEN}🌟 EXCELLENT! Quasi-identique à bash.${NC}"
        echo -e "${YELLOW}🔧 Quelques détails mineurs à ajuster.${NC}"
        return 0
    elif [ $success_rate -ge 80 ]; then
        echo -e "${YELLOW}✅ Bon travail! Majoritairement compatible.${NC}"
        echo -e "${YELLOW}🛠️  Quelques comportements à corriger.${NC}"
        return 1
    elif [ $success_rate -ge 60 ]; then
        echo -e "${YELLOW}⚠️  Correct mais beaucoup de différences avec bash.${NC}"
        return 2
    else
        echo -e "${RED}❌ Comportement très différent de bash.${NC}"
        echo -e "${RED}🔨 Corrections majeures nécessaires.${NC}"
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
    
    # Vérifier bash disponible
    if ! command -v bash >/dev/null 2>&1; then
        echo -e "${RED}❌ Erreur: bash non trouvé pour comparaison${NC}"
        exit 1
    fi
    
    # Rendre exécutable
    chmod +x "$MINISHELL" 2>/dev/null
    
    print_header
    setup_environment
    
    # Exécuter les tests ultra-stricts
    test_command_not_found
    test_exit_codes
    test_logical_operators_strict
    test_pipes_strict
    test_echo_strict
    test_variables_strict
    test_redirections_strict
    
    print_final_summary
    local exit_code=$?
    
    cleanup_environment
    exit $exit_code
}

# Gestion des signaux pour cleanup
trap cleanup_environment EXIT

# Lancer le script
main "$@"
