#!/bin/bash

# Script de nettoyage des fichiers .c
# Garde seulement les commentaires au-dessus des fonctions
# Supprime tous les sauts de ligne inutiles sauf celui entre variables et code

clean_c_file() {
    local file="$1"
    local temp_file=$(mktemp)
    
    awk '
    BEGIN {
        in_header = 1
        in_function = 0
        var_section_done = 0
        prev_line = ""
        function_comment = ""
        collecting_comment = 0
    }
    
    # Garde le header jusqu'à la première ligne non-commentaire
    /^\/\*.*\*\/$/ && in_header { print; next }
    /^\/\*/ && in_header { print; next }
    /^\*/ && in_header { print; next }
    /^\*\// && in_header { print; next }
    /^#include/ && in_header { print; in_header = 0; next }
    
    # Détecte le début d'\''un commentaire de fonction
    /^\/\*\*/ && !in_header {
        collecting_comment = 1
        function_comment = $0 "\n"
        next
    }
    
    # Continue à collecter le commentaire de fonction
    collecting_comment && !/^\*\// {
        function_comment = function_comment $0 "\n"
        next
    }
    
    # Fin du commentaire de fonction
    /^\*\// && collecting_comment {
        function_comment = function_comment $0 "\n"
        collecting_comment = 0
        next
    }
    
    # Détecte une fonction
    /^[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*[a-zA-Z_*][a-zA-Z0-9_*]*[[:space:]]*\(/ && !in_header {
        if (function_comment != "") {
            printf "%s", function_comment
            function_comment = ""
        }
        print
        in_function = 1
        var_section_done = 0
        next
    }
    
    # Dans une fonction
    in_function {
        # Accolade ouvrante
        if (/^{/) {
            print
            next
        }
        
        # Accolade fermante - fin de fonction
        if (/^}/) {
            print
            in_function = 0
            var_section_done = 0
            next
        }
        
        # Ligne vide
        if (/^[[:space:]]*$/) {
            # Garde une ligne vide après les déclarations de variables
            if (!var_section_done && prev_line ~ /;[[:space:]]*$/ && $0 ~ /^[[:space:]]*$/) {
                print
                var_section_done = 1
            }
            next
        }
        
        # Ligne de code normale
        print
        
        # Marque la fin de la section variables si ce n'\''est pas une déclaration
        if (!/^[[:space:]]*[a-zA-Z_][a-zA-Z0-9_*[:space:]]*[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*;/) {
            var_section_done = 1
        }
    }
    
    # Hors fonction, garde seulement les lignes importantes
    !in_header && !in_function && !collecting_comment {
        if (!/^[[:space:]]*$/) {
            print
        }
    }
    
    { prev_line = $0 }
    ' "$file" > "$temp_file"
    
    # Remplace le fichier original
    mv "$temp_file" "$file"
}

# Traite tous les fichiers .c
find mandatory -name "*.c" -type f | while read -r file; do
    echo "Nettoyage de: $file"
    clean_c_file "$file"
done

echo "Nettoyage terminé!"
