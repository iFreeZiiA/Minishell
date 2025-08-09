#!/bin/bash

# Fichiers à préserver (garder NOTRE version)
PRESERVE_FILES=(
    "header/struct.h"
    "mandatory/parser/lexing/parse_var.c"
    "mandatory/parser/lexing/extract.c"
    "mandatory/parser/lexing/quote_processing.c"
    "header/parser/lexing.h"
    "mandatory/signal/signal.c"
)

echo "=== Résolution automatique des conflits ==="

# Fonction pour résoudre les conflits
resolve_conflicts() {
    # Obtenir la liste des fichiers en conflit
    CONFLICT_FILES=$(git status --porcelain | grep "^UU\|^AA\|^DD" | cut -c4-)
    
    if [ -z "$CONFLICT_FILES" ]; then
        echo "Aucun conflit détecté."
        return 0
    fi
    
    echo "Fichiers en conflit détectés:"
    echo "$CONFLICT_FILES"
    echo ""
    
    # Pour chaque fichier en conflit
    for file in $CONFLICT_FILES; do
        # Vérifier si le fichier doit être préservé
        preserve=false
        for preserve_file in "${PRESERVE_FILES[@]}"; do
            if [[ "$file" == "$preserve_file" ]]; then
                preserve=true
                break
            fi
        done
        
        if $preserve; then
            echo "📝 Préservation de notre version: $file"
            git checkout --ours "$file"
        else
            echo "📥 Acceptation des changements entrants: $file"
            git checkout --theirs "$file"
        fi
    done
    
    # Ajouter tous les fichiers résolus
    git add .
    
    echo ""
    echo "✅ Tous les conflits ont été résolus!"
}

# Résoudre les conflits actuels
resolve_conflicts

# Continuer le rebase
echo "🔄 Continuation du rebase..."
git rebase --continue
