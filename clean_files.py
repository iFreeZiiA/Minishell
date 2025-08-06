#!/usr/bin/env python3

import re
import os
import glob

def clean_c_file(file_path):
    """Nettoie un fichier C en supprimant les commentaires inutiles et les sauts de ligne"""
    
    with open(file_path, 'r') as f:
        content = f.read()
    
    lines = content.split('\n')
    cleaned_lines = []
    i = 0
    
    # Garde le header jusqu'aux includes
    while i < len(lines):
        line = lines[i]
        if line.startswith('#include'):
            cleaned_lines.append(line)
            i += 1
            break
        elif line.strip().startswith('/*') or line.strip().startswith('*') or line.strip() == '':
            cleaned_lines.append(line)
        i += 1
    
    # Ajoute une ligne vide après les includes
    if cleaned_lines and not cleaned_lines[-1].strip() == '':
        cleaned_lines.append('')
    
    # Traite le reste du fichier
    in_function = False
    var_section_ended = False
    last_was_empty = False
    function_ended = False
    
    while i < len(lines):
        line = lines[i]
        stripped = line.strip()
        
        # Détecte une fonction
        if re.match(r'^[a-zA-Z_][a-zA-Z0-9_]*\s+[a-zA-Z_*][a-zA-Z0-9_*]*\s*\([^)]*\)\s*$', stripped):
            # Ajoute une ligne vide avant chaque fonction (sauf la première)
            if function_ended and cleaned_lines and cleaned_lines[-1].strip() != '':
                cleaned_lines.append('')
            cleaned_lines.append(line)
            in_function = True
            var_section_ended = False
            last_was_empty = False
            function_ended = False
        
        # Accolade ouvrante
        elif stripped == '{':
            cleaned_lines.append(line)
            in_function = True
            var_section_ended = False
            last_was_empty = False
        
        # Accolade fermante
        elif stripped == '}':
            cleaned_lines.append(line)
            in_function = False
            var_section_ended = False
            last_was_empty = False
            function_ended = True
        
        # Ligne vide
        elif stripped == '':
            if in_function and not var_section_ended and not last_was_empty:
                # Garde une ligne vide après les déclarations de variables
                prev_line = cleaned_lines[-1] if cleaned_lines else ""
                if prev_line.strip().endswith(';') and not re.match(r'.*\(.*\).*{', prev_line):
                    cleaned_lines.append('')
                    var_section_ended = True
            last_was_empty = True
        
        # Ligne normale
        else:
            # Supprime les commentaires inline sauf s'ils sont importants
            if '//' in line and not line.strip().startswith('//'):
                line = re.sub(r'\s*//.*$', '', line)
            
            cleaned_lines.append(line)
            last_was_empty = False
            
            # Marque la fin de la section variables si ce n'est pas une déclaration
            if in_function and not re.match(r'^\s*[a-zA-Z_][a-zA-Z0-9_*\s]*[a-zA-Z_][a-zA-Z0-9_]*\s*;', line):
                var_section_ended = True
        
        i += 1
    
    # Supprime les lignes vides en fin de fichier
    while cleaned_lines and cleaned_lines[-1].strip() == '':
        cleaned_lines.pop()
    
    # Ajoute une ligne vide après la dernière fonction
    if cleaned_lines:
        cleaned_lines.append('')
    
    # Écrit le fichier nettoyé
    with open(file_path, 'w') as f:
        f.write('\n'.join(cleaned_lines))

# Trouve tous les fichiers .c dans mandatory/
c_files = glob.glob('mandatory/**/*.c', recursive=True)

print(f"Nettoyage de {len(c_files)} fichiers .c...")

for file_path in c_files:
    print(f"Nettoyage: {file_path}")
    clean_c_file(file_path)

print("Nettoyage terminé!")
