#!/bin/bash

# ============================================================================
# CRASH HUNTER - Tests agressifs pour détecter les failles du minishell
# ============================================================================
# ⚠️  OBJECTIF: Faire crasher le programme dans les limites du projet
# 🎯 Utilise uniquement des fonctionnalités autorisées par le sujet

# Configuration
MINISHELL="./minishell"
TIMEOUT=5
TOTAL_TESTS=0
CRASHED_TESTS=0
SURVIVED_TESTS=0

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
    echo "🔥================================================🔥"
    echo "  $1"
    echo "🔥================================================🔥"
}

crash_test() {
    local command="$1"
    local description="$2"
    local use_timeout="${3:-true}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "💥 Test $TOTAL_TESTS: $description"
    echo "   Command: $command"
    
    # Exécution avec minishell
    if [ "$use_timeout" = "true" ]; then
        minishell_output=$(timeout $TIMEOUT bash -c "echo '$command' | $MINISHELL" 2>&1)
        minishell_exit=$?
    else
        minishell_output=$(echo -e "$command" | $MINISHELL 2>&1)
        minishell_exit=$?
    fi
    
    # Vérification des crashes (ce qu'on VEUT ici)
    if [[ $minishell_exit -eq 139 || $minishell_exit -eq 134 ]]; then
        echo -e "   ${RED}💀 CRASH SUCCESSFUL!${NC} (exit code: $minishell_exit)"
        CRASHED_TESTS=$((CRASHED_TESTS + 1))
    elif [[ $minishell_exit -eq 124 ]]; then
        echo -e "   ${YELLOW}⏰ TIMEOUT (Potential infinite loop)${NC}"
        CRASHED_TESTS=$((CRASHED_TESTS + 1))
    else
        echo -e "   ${GREEN}🛡️  SURVIVED${NC} (exit code: $minishell_exit)"
        SURVIVED_TESTS=$((SURVIVED_TESTS + 1))
    fi
    echo ""
}

# Tests de stress intensifs
stress_tests() {
    print_section_header "🔥 TESTS DE STRESS EXTRÊMES 🔥"

    # Stress sur la mémoire avec des commandes très longues
    crash_test "$(printf 'echo %.0s' {1..10000})" "Commande de 10k caractères"
    crash_test "$(printf 'a%.0s' {1..5000})" "Nom de commande de 5k caractères"
    
    # Variables très longues
    crash_test "\$$(printf 'A%.0s' {1..1000})" "Variable de 1k caractères"
    crash_test "echo \$$(printf 'B%.0s' {1..2000})" "Echo variable 2k caractères"
    
    # Redirections en chaîne
    crash_test "echo test $(printf '> file%.0s ' {1..100})" "100 redirections consécutives"
    crash_test "cat $(printf '< file%.0s ' {1..50})" "50 redirections d'entrée"
    
    # Pipes extrêmes
    crash_test "echo test$(printf ' | cat%.0s' {1..500})" "500 pipes en chaîne"
    crash_test "$(printf 'echo test | %.0s' {1..200})cat" "200 pipes malformés"
}

# Tests de parenthèses complexes
parentheses_hell() {
    print_section_header "🔥 ENFER DES PARENTHÈSES 🔥"
    
    # Imbrication extrême
    crash_test "$(printf '(%.0s' {1..1000})echo hello$(printf ')%.0s' {1..1000})" "1000 niveaux d'imbrication"
    crash_test "$(printf '(%.0s' {1..500})echo test" "500 parenthèses ouvertes"
    crash_test "echo test$(printf ')%.0s' {1..500})" "500 parenthèses fermées"
    
    # Combinaisons complexes
    crash_test "(((((echo hello | cat) && true) || false) | grep test) && echo ok)" "Parenthèses + opérateurs complexes"
    crash_test "$(printf '((%.0s' {1..100})echo$(printf '))%.0s' {1..100})" "Parenthèses doubles imbriquées"
}

# Tests de variables extrêmes
variable_chaos() {
    print_section_header "🔥 CHAOS DES VARIABLES 🔥"
    
    # Variables récursives/circulaires (si supportées)
    crash_test "export A=\$B && export B=\$A && echo \$A" "Variables circulaires"
    crash_test "export TEST=\$TEST && echo \$TEST" "Variable auto-référentielle"
    
    # Variables avec caractères spéciaux
    crash_test "echo \$'$(printf '\x00%.0s' {1..100})'" "Variables avec null bytes"
    crash_test "echo \$$(printf '%s' '{|}~!@#$%^&*()[]')" "Variables avec caractères spéciaux"
    
    # Variables très nombreuses
    for i in {1..50}; do
        crash_test "export VAR$i=value$i && echo \$VAR$i" "Export massif de variables" true
    done
}

# Tests de heredoc malveillants
heredoc_nightmare() {
    print_section_header "🔥 CAUCHEMAR HEREDOC 🔥"
    
    # Heredoc sans fin (devrait timeout)
    crash_test "cat << EOF\n$(printf 'line %.0s\n' {1..10000})" "Heredoc de 10k lignes sans EOF" false
    
    # Heredoc avec délimiteurs bizarres
    crash_test "cat << \$(echo END)\ntest\nEND" "Heredoc avec délimiteur variable"
    crash_test "cat << ''\ntest" "Heredoc avec délimiteur vide"
    
    # Heredoc imbriqués
    crash_test "cat << EOF1\ncat << EOF2\ntest\nEOF2\nEOF1" "Heredoc imbriqués"
}

# Tests de wildcards destructeurs
wildcard_mayhem() {
    print_section_header "🔥 ANARCHIE DES WILDCARDS 🔥"
    
    # Créer beaucoup de fichiers pour stress test
    for i in {1..1000}; do
        touch "stress_file_$i.txt" 2>/dev/null
    done
    
    # Wildcards sur énormément de fichiers
    crash_test "echo stress_file_*.txt" "Wildcard sur 1000 fichiers"
    crash_test "ls stress_file_*.txt | wc -l" "Wildcard + pipe avec 1000 fichiers"
    
    # Patterns complexes
    crash_test "echo $(printf '*%.0s' {1..100})" "100 wildcards consécutifs"
    crash_test "echo /*/$(printf '*%.0s' {1..50})" "Wildcards récursifs"
    
    # Nettoyage
    rm -f stress_file_*.txt 2>/dev/null
}

# Tests de signaux et interruptions
signal_chaos() {
    print_section_header "🔥 CHAOS DES SIGNAUX 🔥"
    
    # Commandes qui peuvent être interrompues
    crash_test "sleep 10" "Sleep long (pour test Ctrl+C)" true
    crash_test "cat" "Cat sans input (pour test Ctrl+D)" true
    crash_test "yes" "Yes infini (pour test Ctrl+C)" true
}

# Tests de limites système
system_limits() {
    print_section_header "🔥 LIMITES SYSTÈME 🔥"
    
    # Tenter d'épuiser les file descriptors
    crash_test "$(printf 'cat < /dev/null & %.0s' {1..100})" "100 processus cat en background"
    
    # Tenter d'épuiser la mémoire
    crash_test "$(printf 'echo %s ' {1..10000}) > /dev/null" "Echo de 10k arguments"
    
    # Fork bomb basique (dans les limites)
    crash_test "echo test | echo test | echo test | echo test" "Pipes multiples simples"
}

# Fonction principale
main() {
    # Vérifier que minishell existe
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}❌ Erreur: $MINISHELL non trouvé${NC}"
        echo "Compilez d'abord avec: make"
        exit 1
    fi
    
    echo -e "${RED}💀💀💀 CRASH HUNTER - DÉTECTEUR DE FAILLES 💀💀💀${NC}"
    echo -e "${RED}======================================================${NC}"
    echo -e "${YELLOW}⚠️  AVERTISSEMENT: Ce script tente activement de faire crasher votre minishell${NC}"
    echo -e "${YELLOW}🎯 Objectif: Trouver des vulnérabilités dans les limites du projet${NC}"
    echo ""
    
    # Lancer tous les tests de crash
    stress_tests
    parentheses_hell
    variable_chaos
    heredoc_nightmare
    wildcard_mayhem
    signal_chaos
    system_limits
    
    # Afficher les statistiques finales
    print_section_header "💀 BILAN FINAL DES DÉGÂTS 💀"
    echo "📊 STATISTIQUES CRASH HUNTER:"
    echo "   Total des tests: $TOTAL_TESTS"
    echo -e "   Crashes réussis: ${RED}$CRASHED_TESTS${NC} 💀"
    echo -e "   Tests survivés: ${GREEN}$SURVIVED_TESTS${NC} 🛡️"
    
    if [ $CRASHED_TESTS -gt 0 ]; then
        echo -e "\n${RED}💀 SUCCÈS!${NC} $CRASHED_TESTS crash(s) détecté(s)!"
        echo -e "🔧 Votre minishell a des vulnérabilités à corriger."
        echo -e "📝 Zones à améliorer détectées."
    else
        echo -e "\n${GREEN}🛡️  IMPRESSIONNANT!${NC} Aucun crash provoqué!"
        echo -e "🏆 Votre minishell résiste à tous les tests de stress."
        echo -e "💎 Qualité robustesse exceptionnelle!"
    fi
    
    echo ""
    echo -e "${PURPLE}🎯 Mission accomplie: Test de robustesse extrême terminé!${NC}"
    
    # Code de sortie: 0 si crashes trouvés (mission accomplie), 1 sinon
    if [ $CRASHED_TESTS -gt 0 ]; then
        exit 0
    else
        exit 1
    fi
}

# Lancer le script si exécuté directement
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
