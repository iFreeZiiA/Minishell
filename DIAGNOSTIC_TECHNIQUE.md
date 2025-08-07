# 🔬 DIAGNOSTIC TECHNIQUE DÉTAILLÉ - Minishell

**Complément au plan d'amélioration principal**

## 🧪 TESTS DE DIAGNOSTIC

### Script de diagnostic rapide
```bash
#!/bin/bash
# diagnostic_minishell.sh

echo "=== DIAGNOSTIC MINISHELL ==="

echo "1. Test séparateur ;"
echo "true; echo \$?" | ./minishell
echo "Expected: 0"

echo -e "\n2. Test redirection >"
echo "echo test > temp.txt && cat temp.txt" | ./minishell
echo "Expected: test"

echo -e "\n3. Test echo -n"
echo "echo -n hello" | ./minishell
echo " <- Should not have newline"

echo -e "\n4. Test cd -"
echo -e "cd /tmp\ncd -\npwd" | ./minishell
echo "Expected: initial directory"

echo -e "\n5. Test exit arguments"
echo "exit 1 2" | ./minishell
echo "Expected: too many arguments error"
```

## 🔍 ANALYSE DES COMPOSANTS

### 1. Lexer/Parser Status

#### Tokens actuellement supportés
```c
// Vérifiés dans les tests
✅ TOKEN_WORD
✅ TOKEN_PIPE         // |
✅ TOKEN_AND          // &&  
✅ TOKEN_OR           // ||
✅ TOKEN_REDIR_OUT    // >
✅ TOKEN_REDIR_IN     // <
✅ TOKEN_APPEND       // >>
✅ TOKEN_HEREDOC      // <<
// Note: TOKEN_SEMICOLON non requis selon les spécifications
```

#### AST Nodes actuellement supportés
```c
// Vérifiés dans les tests
✅ NODE_COMMAND
✅ NODE_PIPE
✅ NODE_AND
✅ NODE_OR
✅ NODE_REDIR
// Note: NODE_SEQUENCE non requis selon les spécifications
```

### 2. Executor Status

#### Fonctions d'exécution
```c
✅ execute_command()     // Commandes simples
✅ execute_pipe()        // Pipes - CORRIGÉ
✅ execute_and()         // AND logique
✅ execute_or()          // OR logique
⚠️ execute_redirections() // Buggy
// Note: execute_sequence() non requis selon les spécifications
```

### 3. Built-ins Status

#### Statut des built-ins
```c
✅ echo        // Basique OK, flag -n buggy
✅ pwd         // Fonctionne
✅ env         // Fonctionne
✅ export      // Fonctionne
✅ unset       // Fonctionne
⚠️ cd          // Incomplet (pas de cd -, cd)
⚠️ exit        // Gestion d'erreurs incomplète
```

## 🔧 SOLUTIONS TECHNIQUES PRÉCISES

### 1. REDIRECTIONS - Debug et correction

#### A. Diagnostic du problème
```bash
# Test de diagnostic
echo "echo debug > test.txt" | strace -e trace=file ./minishell 2>&1 | grep test.txt
# Vérifier si le fichier est créé ET si les arguments sont corrects
```

#### B. Correction probable dans execute_command
```c
// mandatory/executor/executor.c
// Dans execute_command(), s'assurer que:

int execute_command(t_command *cmd, t_shell *shell)
{
    // 1. Appliquer les redirections AVANT fork()
    if (cmd->redirections) {
        if (apply_redirections(cmd->redirections) != 0) {
            return (1);
        }
    }
    
    // 2. S'assurer que les fichiers de redirection ne sont PAS dans args
    char **clean_args = filter_redirection_args(cmd->args);
    
    // 3. Exécuter avec les arguments nettoyés
    execve(cmd->path, clean_args, shell->env);
}
```

### 2. ECHO -n - Correction du flush

#### Solution probable
```c
// mandatory/builtin/builtin_echo.c
// Remplacer ft_printf par write direct

int builtin_echo(char **args)
{
    int i = 1;
    int newline = 1;
    
    // ... logique existante pour -n ...
    
    while (args[i]) {
        write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
        if (args[i + 1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    
    if (newline)
        write(STDOUT_FILENO, "\n", 1);
    
    return (0);
}
```

### 3. CD AVANCÉ - Implementation complète

#### Variables d'environnement requises
```c
// mandatory/builtin/builtin_cd.c
// Gérer PWD et OLDPWD correctement

int builtin_cd(char **args, t_shell *shell)
{
    char current_pwd[PATH_MAX];
    char *target_dir;
    char *old_pwd;
    
    // Obtenir le répertoire actuel
    if (!getcwd(current_pwd, sizeof(current_pwd))) {
        perror("getcwd");
        return (1);
    }
    
    // Déterminer le répertoire cible
    if (!args[1]) {
        // cd sans argument -> HOME
        target_dir = ft_getenv("HOME", shell->env);
        if (!target_dir) {
            ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
            return (1);
        }
    } else if (ft_strcmp(args[1], "-") == 0) {
        // cd - -> OLDPWD
        target_dir = ft_getenv("OLDPWD", shell->env);
        if (!target_dir) {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
            return (1);
        }
        // Afficher le répertoire cible (comportement bash)
        ft_putstr_fd(target_dir, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
    } else {
        target_dir = args[1];
    }
    
    // Changer de répertoire
    if (chdir(target_dir) != 0) {
        perror("cd");
        return (1);
    }
    
    // Mettre à jour les variables d'environnement
    ft_setenv("OLDPWD", current_pwd, shell);
    
    if (getcwd(current_pwd, sizeof(current_pwd))) {
        ft_setenv("PWD", current_pwd, shell);
    }
    
    return (0);
}
```

## 📊 MÉTRIQUES DE QUALITÉ

### Tests de validation automatique

#### Script de validation continue
```bash
#!/bin/bash
# validation_continue.sh

SCORE=0
TOTAL=0

test_feature() {
    local name="$1"
    local bash_cmd="$2" 
    local mini_cmd="$3"
    
    bash_result=$(eval "$bash_cmd" 2>/dev/null)
    mini_result=$(eval "$mini_cmd" 2>/dev/null)
    
    TOTAL=$((TOTAL + 1))
    if [ "$bash_result" = "$mini_result" ]; then
        echo "✅ $name"
        SCORE=$((SCORE + 1))
    else
        echo "❌ $name"
        echo "   Bash: '$bash_result'"
        echo "   Mini: '$mini_result'"
    fi
}

# Tests critiques
test_feature "Semicolon" "echo 'true; echo 1' | bash" "echo 'true; echo 1' | ./minishell"
test_feature "Echo -n" "echo 'echo -n hello' | bash" "echo 'echo -n hello' | ./minishell"
test_feature "Redirection" "echo 'echo test > /tmp/test && cat /tmp/test' | bash" "echo 'echo test > /tmp/test && cat /tmp/test' | ./minishell"

echo "Score: $SCORE/$TOTAL ($(( SCORE * 100 / TOTAL ))%)"
```

### Objectifs de qualité
- **Phase 1** : 85% → 90% (corrections critiques)
- **Phase 2** : 90% → 95% (built-ins)  
- **Phase 3** : 95% → 98%+ (finitions)

## 🚀 INTÉGRATION CONTINUE

### Makefile targets suggérés
```makefile
# Dans le Makefile principal
test_quick:
	@./diagnostic_minishell.sh

test_strict:
	@./test_strict_comparison.sh | grep "Taux de réussite"

test_all:
	@./test_all_commands.sh | grep "Taux de réussite"

validate: test_quick test_strict test_all
	@echo "Validation complète terminée"
```

Cette analyse technique donne les outils nécessaires pour diagnostiquer et corriger méthodiquement chaque problème identifié.
