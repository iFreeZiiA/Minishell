#!/bin/bash

# Tests avancés pour les wildcards
echo "🔬 Tests avancés wildcards"

# Créer un environnement de test plus complexe
mkdir -p test_advanced
cd test_advanced

# Créer une structure de fichiers variée
touch main.c parser.c lexer.c
touch main.h parser.h lexer.h
touch README.md Makefile test.txt
touch .gitignore .vscode_settings
mkdir subdir
touch subdir/file.c

echo "Structure créée:"
ls -la

echo "=== Test 1: Wildcard avec echo ==="
echo "Commande: echo *.c"
echo "minishell:"
echo "echo *.c" | ../minishell
echo "bash:"
echo "*.c" | bash

echo "=== Test 2: Wildcard avec ls ==="
echo "Commande: ls *.h"
echo "minishell:"
echo "ls *.h" | ../minishell
echo "bash:"
echo "ls *.h" | bash

echo "=== Test 3: Wildcard avec cat ==="
echo "Commande: cat README.md"
echo "minishell:"
echo "cat README.md" | ../minishell
echo "bash:"
echo "cat README.md" | bash

echo "=== Test 4: Wildcard inexistant ==="
echo "Commande: echo *.xyz"
echo "minishell:"
echo "echo *.xyz" | ../minishell
echo "bash:"
echo "echo *.xyz" | bash

echo "=== Test 5: Combinaison avec pipes ==="
echo "Commande: echo *.c | wc -w"
echo "minishell:"
echo "echo *.c | wc -w" | ../minishell
echo "bash:"
echo "echo *.c | wc -w" | bash

cd ..
rm -rf test_advanced

echo "✅ Tests avancés terminés"
