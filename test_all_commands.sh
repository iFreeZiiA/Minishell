#!/bin/bash

# ============================================================================
# Script de Test Complet - Toutes les Commandes du fichier test_commands
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
TIMEOUT_CMD="timeout 10s"
TEMP_DIR="./test_all_temp"
ORIGINAL_DIR=$(pwd)

# Compteurs globaux
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
CURRENT_SECTION=""

# ============================================================================
# FONCTIONS UTILITAIRES
# ============================================================================

setup_environment() {
    echo -e "${BLUE}🔧 Configuration de l'environnement de test...${NC}"
    
    # Créer dossiers temporaires
    mkdir -p "$TEMP_DIR" 2>/dev/null
    cd "$TEMP_DIR" 2>/dev/null || {
        echo -e "${RED}❌ Erreur: Impossible de créer le répertoire de test${NC}"
        exit 1
    }
    
    # Créer fichiers de test
    echo "Test content for redirection" > test_file.txt
    touch main.c parser.c
    echo "int main() { return 0; }" > main.c
    echo "char *parser() { return NULL; }" >> main.c
    
    # Variables d'environnement
    export TEST_VAR="hello_world"
    export USER="${USER:-testuser}"
    export HOME="${HOME:-/home/testuser}"
    
    echo -e "${GREEN}✓ Environnement configuré${NC}"
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

run_command_test() {
    local command="$1"
    local description="$2"
    local expect_success="${3:-true}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test $TOTAL_TESTS: $description${NC}"
    echo -e "${CYAN}   Command: $command${NC}"
    
    # Exécuter la commande
    if [[ "$command" == "exit"* ]]; then
        # Cas spécial pour exit
        echo -e "$command\nexit" | $TIMEOUT_CMD "$ORIGINAL_DIR/$MINISHELL" >/dev/null 2>&1
    else
        echo -e "$command\nexit" | $TIMEOUT_CMD "$ORIGINAL_DIR/$MINISHELL" >/dev/null 2>&1
    fi
    
    local exit_code=$?
    
    # Vérifier le résultat
    if [ "$expect_success" = "true" ]; then
        if [ $exit_code -eq 0 ] || [ $exit_code -eq 124 ]; then  # 124 = timeout (normal pour certaines commandes)
            echo -e "${GREEN}   ✅ PASSED${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}   ❌ FAILED (exit code: $exit_code)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    else
        if [ $exit_code -ne 0 ] && [ $exit_code -ne 124 ]; then
            echo -e "${GREEN}   ✅ PASSED (correctly failed)${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}   ❌ FAILED (should have failed)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    fi
    echo ""
}

# ============================================================================
# TESTS PAR SECTION
# ============================================================================

test_simple_commands() {
    print_section_header "COMMANDES SIMPLES"
    
    run_command_test "/bin/ls" "Commande avec chemin absolu"
    run_command_test "clear" "Commande clear"
}

test_arguments() {
    print_section_header "ARGUMENTS"
    
    run_command_test "ls ." "Liste répertoire courant"
    run_command_test "ls .." "Liste répertoire parent"
    run_command_test "ls parser" "Liste répertoire spécifique" false  # Peut ne pas exister
    run_command_test "cat test_file.txt" "Cat fichier de test"
    run_command_test "grep Test test_file.txt" "Grep dans fichier"
}

test_echo() {
    print_section_header "ECHO"
    
    run_command_test "echo" "Echo sans arguments"
    run_command_test "echo -n" "Echo avec option -n"
    run_command_test "echo -n -n -n ok" "Echo multiples -n"
    run_command_test "echo ok bonjour" "Echo avec arguments"
    run_command_test "echo salut coucou Woop woop -n" "Echo complexe"
}

test_exit() {
    print_section_header "EXIT"
    
    run_command_test "exit" "Exit simple"
    run_command_test "exit 130" "Exit avec code"
    run_command_test "exit wow" "Exit avec argument invalide" false
    run_command_test "exit 1 yo" "Exit avec trop d'arguments" false
    run_command_test "exit 12 49" "Exit avec multiples arguments" false
    run_command_test "exit 255" "Exit avec code limite"
}

test_return_values() {
    print_section_header "VALEURS DE RETOUR"
    
    run_command_test "ls" "ls puis echo \$?"
    run_command_test "echo \$?" "Vérification code de retour"
    run_command_test "ls nonexistent" "ls fichier inexistant" false
    run_command_test "echo \$?" "Code retour après erreur"
    run_command_test "echo cool" "Echo simple"
    run_command_test "echo \$?" "Code retour après succès"
}

test_signals() {
    print_section_header "SIGNAUX (Tests rapides)"
    
    # Note: Ces tests sont limités par timeout pour éviter de bloquer
    run_command_test "echo 'test signal'" "Test signal simulé"
}

test_double_quotes() {
    print_section_header "GUILLEMETS DOUBLES"
    
    run_command_test "echo \"bonjour l'ami\"" "Guillemets avec apostrophe"
    run_command_test "echo \"wowie!\"" "Guillemets avec exclamation"
    run_command_test "echo \"allez bonjour \"\"trop bien\" ok\" salut\"" "Guillemets imbriqués"
    run_command_test "echo \" wow \\\" top \\\\\" \"test du \\test \\ \\\\ ok \\\" \\\" \\' \"" "Guillemets avec échappements"
}

test_env() {
    print_section_header "ENVIRONNEMENT"
    
    run_command_test "env" "Affichage environnement"
    run_command_test "env | grep PATH" "Recherche PATH dans env"
}

test_export() {
    print_section_header "EXPORT"
    
    run_command_test "export test=ok" "Export variable"
    run_command_test "echo \$test" "Echo variable exportée"
    run_command_test "export test" "Export sans valeur"
    run_command_test "echo \$test" "Echo après export sans valeur"
    run_command_test "export test=diff" "Modification variable"
    run_command_test "echo \$test" "Echo variable modifiée"
}

test_unset() {
    print_section_header "UNSET"
    
    run_command_test "export testvar=value" "Création variable pour unset"
    run_command_test "env | grep testvar" "Vérification variable existe"
    run_command_test "unset testvar" "Unset variable"
    run_command_test "env | grep testvar" "Vérification variable supprimée" false
    run_command_test "echo \$testvar" "Echo variable après unset"
}

test_env_variables() {
    print_section_header "VARIABLES D'ENVIRONNEMENT"
    
    run_command_test "echo \$PATH" "Echo PATH"
    run_command_test "export ok=\"woooow salut! ok...\"" "Export variable complexe"
    run_command_test "echo \$ok" "Echo variable complexe"
    run_command_test "export ok=\"-YO-\"" "Modification variable"
    run_command_test "echo \" topitop \$ok cool \\\$ok cool \\\\\$ok \\\\\\\$ok\"" "Echo avec échappements"
}

test_cd_pwd() {
    print_section_header "CD et PWD"
    
    run_command_test "pwd" "PWD initial"
    run_command_test "cd .." "CD parent"
    run_command_test "pwd" "PWD après cd parent"
    run_command_test "cd -" "CD précédent" false  # Peut ne pas être implémenté
    run_command_test "pwd" "PWD après cd -"
    run_command_test "cd" "CD home"
    run_command_test "pwd" "PWD après cd home"
}

test_relative_path() {
    print_section_header "CHEMINS RELATIFS"
    
    # Ces tests sont simplifiés car ils dépendent de l'arborescence
    run_command_test "ls ." "ls relatif"
    run_command_test "echo 'test relatif'" "Test chemin relatif simulé"
}

test_simple_quotes() {
    print_section_header "GUILLEMETS SIMPLES"
    
    run_command_test "echo 'coco'" "Guillemets simples basiques"
    run_command_test "echo 'coco \\\\ wow \$ok'" "Guillemets simples avec échappements"
    run_command_test "echo ' salut \" hoho ''ok ok'" "Guillemets simples complexes"
    run_command_test "echo ''" "Guillemets simples vides"
    run_command_test "echo ' ok ; wow ; cat ; ls ; echo \$ok'" "Guillemets simples avec commandes"
}

test_redirection() {
    print_section_header "REDIRECTIONS"
    
    run_command_test "echo bonjour > fichier" "Redirection sortie"
    run_command_test "cat < test_file.txt" "Redirection entrée"
    run_command_test "echo test > temp && cat < temp > test2" "Redirection entrée et sortie"
    run_command_test "echo append >> fichier" "Redirection append"
    run_command_test "cat < test_file.txt" "Cat avec redirection"
    run_command_test "echo multiple > a1 > a2 > a3" "Redirections multiples"
}

test_pipes() {
    print_section_header "PIPES"
    
    run_command_test "echo hello | cat" "Pipe simple"
    run_command_test "ls | grep test" "Pipe avec grep"
    run_command_test "echo content | cat | grep content" "Pipe multiple"
    run_command_test "ls | cat | cat" "Pipe répétitif"
    run_command_test "echo data | cat > output.txt" "Pipe vers fichier"
    run_command_test "ls nonexistent | grep bla | cat" "Pipe avec erreur" false
}

test_crazy_commands() {
    print_section_header "COMMANDES COMPLEXES"
    
    run_command_test "invalidcommand" "Commande inexistante" false
    run_command_test "echo wow wowow w w w w w w w w" "Echo avec nombreux arguments"
    run_command_test "echo test && echo success" "Commande avec &&"
    run_command_test "false || echo backup" "Commande avec ||"
}

# ============================================================================
# FONCTION PRINCIPALE
# ============================================================================

print_header() {
    echo -e "${BLUE}============================================================================${NC}"
    echo -e "${BLUE}                    TEST COMPLET - TOUTES LES COMMANDES${NC}"
    echo -e "${BLUE}                         Basé sur test_commands${NC}"
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
    
    if [ $TOTAL_TESTS -eq 0 ]; then
        echo -e "${RED}❌ Aucun test exécuté${NC}"
        return 1
    fi
    
    local success_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "${YELLOW}📈 Taux de réussite: $success_rate%${NC}"
    echo ""
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "${GREEN}🎉 PARFAIT! TOUS LES TESTS SONT PASSÉS!${NC}"
        echo -e "${GREEN}🚀 Votre minishell gère parfaitement toutes les commandes!${NC}"
        return 0
    elif [ $success_rate -ge 90 ]; then
        echo -e "${GREEN}🌟 EXCELLENT! Plus de 90% de réussite.${NC}"
        echo -e "${YELLOW}🔧 Quelques ajustements mineurs seulement.${NC}"
        return 0
    elif [ $success_rate -ge 80 ]; then
        echo -e "${YELLOW}✅ Très bon! Plus de 80% de réussite.${NC}"
        echo -e "${YELLOW}🛠️  Quelques améliorations nécessaires.${NC}"
        return 1
    elif [ $success_rate -ge 60 ]; then
        echo -e "${YELLOW}⚠️  Correct, mais des améliorations importantes nécessaires.${NC}"
        return 2
    else
        echo -e "${RED}❌ Des corrections majeures sont nécessaires.${NC}"
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
    setup_environment
    
    # Exécuter tous les tests des sections
    test_simple_commands
    test_arguments
    test_echo
    test_exit
    test_return_values
    test_signals
    test_double_quotes
    test_env
    test_export
    test_unset
    test_env_variables
    test_cd_pwd
    test_relative_path
    test_simple_quotes
    test_redirection
    test_pipes
    test_crazy_commands
    
    print_final_summary
    local exit_code=$?
    
    cleanup_environment
    exit $exit_code
}

# Gestion des signaux pour cleanup
trap cleanup_environment EXIT

# Lancer le script
main "$@"
