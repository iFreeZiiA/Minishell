# 📋 PLAN D'AMÉLIORATION MINISHELL - Compatibilité 100% avec Bash

**Date d'analyse :** 6 août 2025
**Version actuelle :** Taux de réussite 77% (test strict) / 85% (test général)
**Objectif :** Atteindre 100% de compatibilité avec bash

## 🎯 RÉSUMÉ EXÉCUTIF

### Status Actuel
- ✅ **Fonctionnalités principales** : Opérateurs logiques (&&, ||), pipes de base, echo simple
- ❌ **Points critiques** : Redirections, built-ins incomplets, gestion d'erreurs

### Priorités (par ordre d'impact)
1. 🔥 **CRITIQUE** : Redirections (>, >>, <) 
2. 🔥 **CRITIQUE** : Built-in echo (flag -n)
3. 🚨 **MAJEUR** : Built-in cd (cd -, cd ~)
4. 🚨 **MAJEUR** : Built-in exit (gestion arguments)
5. ⚠️ **IMPORTANT** : Expansion variables vides ($NONEXISTENT)

---

## 1. 🔥 REDIRECTIONS - PRIORITÉ CRITIQUE

### **Problèmes identifiés**

#### A. Redirection simple `>`
```bash
# Bash
echo test > output.txt; cat output.txt    # Affiche: test

# Minishell
echo test > output.txt; cat output.txt    # Affiche: test output.txt
# ❌ ERREUR: "cat" prend "output.txt" comme argument au lieu de lire le fichier
```

#### B. Redirection append `>>`
```bash
echo append >> file    # Ne fonctionne pas correctement
```

### **Solutions requises**

#### A. Parser - Reconnaissance des redirections
```c
// Dans mandatory/parser/parsing/redirections.c
// S'assurer que ft_parse_redirections() traite correctement:

int ft_parse_output_redirection(t_list **tokens, t_command *cmd);
int ft_parse_append_redirection(t_list **tokens, t_command *cmd);
int ft_parse_input_redirection(t_list **tokens, t_command *cmd);

// Vérifier que les tokens de redirection sont bien séparés des arguments
```

#### B. Executor - Application des redirections
```c
// Dans mandatory/executor/executor.c
// Vérifier execute_command() pour s'assurer que:

1. Les redirections sont appliquées AVANT l'exécution
2. Les fichiers de redirection ne sont PAS passés comme arguments
3. Les descripteurs sont correctement restaurés après exécution
```

### **Diagnostic à effectuer**
```bash
# Tester la séparation des arguments
echo "echo test > output.txt" | ./minishell
# Vérifier si "output.txt" apparaît dans les arguments de echo

# Vérifier la création du fichier
ls -la output.txt  # Le fichier doit exister et contenir "test"
```

---

## 2. 🔥 BUILT-IN ECHO - PRIORITÉ CRITIQUE

### **Problèmes identifiés**

#### A. Echo sans arguments
```bash
# Bash
echo          # Affiche une ligne vide

# Minishell  
echo          # Affiche une ligne vide ✅ (fonctionne)
```

#### B. Flag -n (pas de newline)
```bash
# Bash
echo -n hello    # Affiche: hello (sans newline)

# Minishell
echo -n hello    # Affiche: hellominishell$ (mélangé avec prompt)
```

### **Solution requise**

#### Correction du built-in echo
```c
// Dans mandatory/builtin/builtin_echo.c
// Le code actuel semble correct, le problème est probablement dans l'affichage

// Vérifier que ft_printf() flush correctement le buffer
// Ou utiliser write() directement pour garantir l'affichage immédiat

int builtin_echo(char **args)
{
    // ... code existant ...
    
    if (newline)
        write(STDOUT_FILENO, "\n", 1);  // Au lieu de ft_printf("\n")
    
    return (0);
}
```

### **Tests de validation**
```bash
echo              # Une ligne vide
echo hello        # hello + newline
echo -n hello     # hello sans newline
echo -n -n hello  # hello sans newline (flags multiples)
echo -n; echo world   # pas de newline puis world sur même ligne
```

---

## 3. 🚨 BUILT-IN CD - PRIORITÉ MAJEURE

### **Problèmes identifiés**

#### A. `cd -` (répertoire précédent)
```bash
# Bash
cd /tmp; cd -    # Retourne au répertoire précédent

# Minishell
cd -             # ❌ Ne fonctionne pas
```

#### B. `cd` sans argument (retour à HOME)
```bash
# Bash
cd               # Va à $HOME

# Minishell
cd               # ❌ Ne fonctionne pas
```

### **Solutions requises**

#### A. Implémentation de OLDPWD
```c
// Dans mandatory/builtin/builtin_cd.c
// Ajouter la gestion de la variable OLDPWD

int builtin_cd(char **args, t_shell *shell)
{
    char *old_pwd;
    char *new_path;
    
    // Sauvegarder PWD actuel dans OLDPWD
    old_pwd = ft_getenv("PWD", shell->env);
    
    if (!args[1]) {
        // cd sans argument -> aller à HOME
        new_path = ft_getenv("HOME", shell->env);
    } else if (ft_strcmp(args[1], "-") == 0) {
        // cd - -> aller à OLDPWD
        new_path = ft_getenv("OLDPWD", shell->env);
        if (!new_path) {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
            return (1);
        }
        // Afficher le nouveau répertoire comme bash
        ft_putstr_fd(new_path, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
    } else {
        new_path = args[1];
    }
    
    // Changer de répertoire et mettre à jour les variables
    if (chdir(new_path) == 0) {
        ft_setenv("OLDPWD", old_pwd, shell);
        ft_setenv("PWD", getcwd(NULL, 0), shell);
        return (0);
    }
    
    // Gestion d'erreur...
    return (1);
}
```

### **Tests de validation**
```bash
pwd; cd /tmp; pwd; cd -; pwd     # Test cd -
cd; pwd                          # Test cd sans argument
cd /nonexistent                  # Test erreur
```

---

## 4. 🚨 BUILT-IN EXIT - PRIORITÉ MAJEURE

### **Problèmes identifiés**

#### A. Arguments invalides
```bash
# Bash
exit wow         # bash: exit: wow: numeric argument required

# Minishell  
exit wow         # ❌ Comportement différent
```

#### B. Trop d'arguments
```bash
# Bash
exit 1 2         # bash: exit: too many arguments

# Minishell
exit 1 2         # ❌ Comportement différent
```

### **Solutions requises**

#### Correction du built-in exit
```c
// Dans mandatory/builtin/builtin_exit.c
int builtin_exit(char **args, t_shell *shell)
{
    int exit_code = 0;
    
    if (!args[1]) {
        // exit sans argument
        exit_code = shell->last_exit_code;
    } else if (args[2]) {
        // Trop d'arguments
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return (1);  // Ne pas quitter, retourner 1
    } else {
        // Vérifier si l'argument est numérique
        if (!ft_is_numeric(args[1])) {
            ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
            ft_putstr_fd(args[1], STDERR_FILENO);
            ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
            exit_code = 2;
        } else {
            exit_code = ft_atoi(args[1]);
        }
    }
    
    // Afficher "exit" comme bash en mode interactif
    if (shell->interactive) {
        ft_putstr_fd("exit\n", STDOUT_FILENO);
    }
    
    cleanup_shell(shell);
    exit(exit_code);
}
```

### **Tests de validation**
```bash
exit 0           # Sortie normale
exit 42          # Code de sortie personnalisé
exit wow         # Erreur argument non numérique
exit 1 2         # Erreur trop d'arguments
```

---

## 5. ⚠️ EXPANSION VARIABLES - PRIORITÉ IMPORTANTE

### **Problème identifié**

#### Variables non définies
```bash
# Bash
echo $NONEXISTENT    # Affiche une ligne vide

# Minishell
echo $NONEXISTENT    # ❌ Comportement différent
```

### **Solution requise**

#### Correction de l'expansion
```c
// Dans mandatory/parser/lexing/expansion/var_expansion.c
char *ft_expand_variable(char *var_name, t_shell *shell)
{
    char *value;
    
    value = ft_getenv(var_name, shell->env);
    if (!value) {
        // Variable non définie -> retourner chaîne vide
        return (ft_strdup(""));
    }
    
    return (ft_strdup(value));
}
```

---

## 6. 🔧 CORRECTIONS MINEURES

### A. Commande `ls` avec arguments
```bash
ls parser        # Doit lister le contenu du répertoire parser/
ls nonexistent   # Doit afficher une erreur mais ne pas planter
```

### B. Gestion des pipes avec erreurs
```bash
ls nonexistent | grep test | cat    # Doit propager l'erreur correctement
```

### C. Built-in env avec grep
```bash
env | grep testvar    # Doit fonctionner après unset
```

---

## 📋 PLAN D'IMPLÉMENTATION RECOMMANDÉ

### Phase 1 (2-3 jours) - Corrections critiques
1. **Redirections** - Debugging et correction
2. **Built-in echo** - Correction flag -n

### Phase 2 (1-2 jours) - Built-ins
3. **Built-in cd** - cd -, cd sans argument
4. **Built-in exit** - Gestion des erreurs
5. **Expansion variables** - Variables vides

### Phase 3 (1 jour) - Finitions
6. **Tests et corrections mineures**
7. **Validation complète avec scripts de test**

### Tests de validation finale
```bash
./test_strict_comparison.sh    # Doit atteindre 95%+
./test_all_commands.sh         # Doit atteindre 95%+
```

---

## 🎯 OBJECTIF FINAL

**Compatibilité 100%** avec bash pour toutes les fonctionnalités implémentées :
- ✅ Opérateurs logiques (&&, ||)
- ✅ Pipes simples et multiples  
- ✅ Redirections (>, >>, <)
- ✅ Built-ins complets (echo, cd, exit, pwd, env, export, unset)
- ✅ Expansion de variables ($VAR, $?)
- ✅ Gestion des quotes (' et ")
- ✅ Wildcards (*)
- ✅ Parenthèses pour grouper
- ✅ Heredoc (<<)

**Résultat attendu :** Taux de réussite 98-100% sur tous les tests automatisés.
