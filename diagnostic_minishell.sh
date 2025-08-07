#!/bin/bash

# ============================================================================
# DIAGNOSTIC AUTOMATIQUE MINISHELL - Tests de régression rapides
# ============================================================================

# Configuration
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

MINISHELL="./minishell"
PASSED=0
TOTAL=0

# ============================================================================
# FONCTIONS UTILITAIRES
# ============================================================================

test_feature() {
    local name="$1"
    local test_cmd="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    echo -e "${BLUE}Test $TOTAL: $name${NC}"
    echo -e "${YELLOW}Command: $test_cmd${NC}"
    
    # Exécuter le test
    result=$(echo "$test_cmd" | timeout 5s "$MINISHELL" 2>/dev/null | \
             sed 's/\x1b\[[0-9;]*m//g' | \
             grep -v "^minishell" | \
             grep -v "^exit$" | \
             sed '/^[[:space:]]*$/d' | \
             tr '\n' ' ' | \
             sed 's/[[:space:]]*$//')
    
    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✅ PASSED${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}❌ FAILED${NC}"
        echo -e "${YELLOW}Expected: '$expected'${NC}"
        echo -e "${YELLOW}Got:      '$result'${NC}"
    fi
    echo ""
}

compare_with_bash() {
    local name="$1"
    local test_cmd="$2"
    
    TOTAL=$((TOTAL + 1))
    echo -e "${BLUE}Test $TOTAL: $name${NC}"
    echo -e "${YELLOW}Command: $test_cmd${NC}"
    
    # Bash result
    bash_result=$(echo "$test_cmd" | bash 2>/dev/null | tr '\n' ' ' | sed 's/[[:space:]]*$//')
    
    # Minishell result
    mini_result=$(echo "$test_cmd" | timeout 5s "$MINISHELL" 2>/dev/null | \
                  sed 's/\x1b\[[0-9;]*m//g' | \
                  grep -v "^minishell" | \
                  grep -v "^exit$" | \
                  sed '/^[[:space:]]*$/d' | \
                  tr '\n' ' ' | \
                  sed 's/[[:space:]]*$//')
    
    if [ "$bash_result" = "$mini_result" ]; then
        echo -e "${GREEN}✅ IDENTICAL TO BASH${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}❌ DIFFERS FROM BASH${NC}"
        echo -e "${YELLOW}Bash:      '$bash_result'${NC}"
        echo -e "${YELLOW}Minishell: '$mini_result'${NC}"
    fi
    echo ""
}

# ============================================================================
# TESTS CRITIQUES
# ============================================================================

print_section() {
    echo -e "${BLUE}============================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}============================================${NC}"
    echo ""
}

test_critical_features() {
    print_section "🔥 TESTS CRITIQUES"
    
    # 1. Séparateur ;
    echo -e "${YELLOW}1. SÉPARATEUR ';' (CRITIQUE)${NC}"
    compare_with_bash "True then echo" "true; echo success"
    compare_with_bash "False then echo" "false; echo \$?"
    compare_with_bash "Multiple commands" "echo a; echo b; echo c"
    
    # 2. Redirections
    echo -e "${YELLOW}2. REDIRECTIONS (CRITIQUE)${NC}"
    compare_with_bash "Simple redirect" "echo test > /tmp/minitest.txt && cat /tmp/minitest.txt"
    compare_with_bash "Append redirect" "echo line1 > /tmp/minitest.txt && echo line2 >> /tmp/minitest.txt && cat /tmp/minitest.txt"
    
    # 3. Echo -n
    echo -e "${YELLOW}3. ECHO -N (CRITIQUE)${NC}"
    test_feature "Echo no newline" "echo -n hello" "hello"
    test_feature "Echo normal" "echo hello" "hello"
    test_feature "Echo empty" "echo" ""
}

test_builtin_features() {
    print_section "🚨 BUILT-INS MAJEURS"
    
    # CD
    echo -e "${YELLOW}1. CD AVANCÉ${NC}"
    compare_with_bash "CD home" "cd && pwd"
    # Note: cd - difficile à tester automatiquement
    
    # EXIT avec erreurs
    echo -e "${YELLOW}2. EXIT GESTION ERREURS${NC}"
    # Note: exit termine le shell, difficile à tester automatiquement
    
    # Variables vides
    echo -e "${YELLOW}3. VARIABLES VIDES${NC}"
    compare_with_bash "Undefined variable" "echo \$UNDEFINED_VAR_TEST"
    compare_with_bash "Empty expansion" "echo prefix\$UNDEFINED_VAR_TESTsuffix"
}

test_standard_features() {
    print_section "✅ FONCTIONNALITÉS STANDARD"
    
    # Opérateurs logiques (déjà corrigés)
    compare_with_bash "AND true" "true && echo success"
    compare_with_bash "AND false" "false && echo never"
    compare_with_bash "OR true" "true || echo never"
    compare_with_bash "OR false" "false || echo fallback"
    
    # Pipes simples
    compare_with_bash "Simple pipe" "echo hello | cat"
    compare_with_bash "Pipe with grep" "echo test | grep test"
    
    # Variables standard
    compare_with_bash "HOME variable" "echo \$HOME"
    compare_with_bash "Exit code" "true && echo \$?"
}

# ============================================================================
# TESTS DE PERFORMANCE
# ============================================================================

test_performance() {
    print_section "⚡ TESTS DE PERFORMANCE"
    
    echo -e "${YELLOW}Tests de stabilité...${NC}"
    
    # Test de commandes multiples
    local complex_cmd="echo a && echo b || echo c | cat && true; echo \$?"
    echo -e "${BLUE}Complex command test:${NC} $complex_cmd"
    
    start_time=$(date +%s%N)
    result=$(echo "$complex_cmd" | timeout 10s "$MINISHELL" 2>/dev/null)
    end_time=$(date +%s%N)
    
    duration=$(( (end_time - start_time) / 1000000 ))  # ms
    
    if [ $? -eq 0 ] && [ $duration -lt 1000 ]; then
        echo -e "${GREEN}✅ Performance OK (${duration}ms)${NC}"
    else
        echo -e "${RED}❌ Performance issue or timeout${NC}"
    fi
    echo ""
}

# ============================================================================
# RAPPORT FINAL
# ============================================================================

generate_report() {
    print_section "📊 RAPPORT DE DIAGNOSTIC"
    
    local success_rate=0
    if [ $TOTAL -gt 0 ]; then
        success_rate=$((PASSED * 100 / TOTAL))
    fi
    
    echo -e "${YELLOW}Statistiques:${NC}"
    echo -e "  Tests exécutés: $TOTAL"
    echo -e "  Tests réussis:  ${GREEN}$PASSED${NC}"
    echo -e "  Tests échoués:  ${RED}$((TOTAL - PASSED))${NC}"
    echo -e "  Taux de succès: ${YELLOW}$success_rate%${NC}"
    echo ""
    
    if [ $success_rate -ge 95 ]; then
        echo -e "${GREEN}🎉 EXCELLENT! Minishell fonctionne parfaitement!${NC}"
        return 0
    elif [ $success_rate -ge 85 ]; then
        echo -e "${YELLOW}✅ BON! Quelques corrections mineures nécessaires.${NC}"
        return 1
    elif [ $success_rate -ge 70 ]; then
        echo -e "${YELLOW}⚠️  MOYEN. Corrections importantes nécessaires.${NC}"
        echo -e "${BLUE}Consultez PLAN_AMELIORATION_MINISHELL.md${NC}"
        return 2
    else
        echo -e "${RED}❌ INSUFFISANT. Corrections critiques nécessaires.${NC}"
        echo -e "${BLUE}Consultez PLAN_AMELIORATION_MINISHELL.md et DIAGNOSTIC_TECHNIQUE.md${NC}"
        return 3
    fi
}

# ============================================================================
# FONCTION PRINCIPALE
# ============================================================================

main() {
    # Vérifications préliminaires
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}❌ Erreur: $MINISHELL non trouvé${NC}"
        echo -e "${YELLOW}💡 Compilez d'abord avec: make${NC}"
        exit 1
    fi
    
    chmod +x "$MINISHELL" 2>/dev/null
    
    echo -e "${BLUE}============================================================================${NC}"
    echo -e "${BLUE}                    DIAGNOSTIC AUTOMATIQUE MINISHELL${NC}"
    echo -e "${BLUE}                         Tests de Régression${NC}"
    echo -e "${BLUE}============================================================================${NC}"
    echo ""
    
    # Exécuter les tests
    test_critical_features
    test_builtin_features  
    test_standard_features
    test_performance
    
    # Générer le rapport
    generate_report
    exit_code=$?
    
    echo ""
    echo -e "${BLUE}Fichiers de référence:${NC}"
    echo -e "  📋 ${YELLOW}PLAN_AMELIORATION_MINISHELL.md${NC} - Plan complet des améliorations"
    echo -e "  🔬 ${YELLOW}DIAGNOSTIC_TECHNIQUE.md${NC} - Solutions techniques détaillées"
    echo -e "  🧪 ${YELLOW}diagnostic_minishell.sh${NC} - Ce script de diagnostic"
    
    return $exit_code
}

# Gestion des signaux
trap 'echo -e "\n${YELLOW}Diagnostic interrompu${NC}"; exit 1' INT TERM

# Lancer le diagnostic
main "$@"
