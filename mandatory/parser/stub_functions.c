// Fonctions stub temporaires pour les tests
#include "../../header/minishell.h"

// Forward declarations des fonctions PHASE 4-5
int ft_count_word_tokens_until_pipe(t_list *tokens);
char **ft_extract_command_args_until_pipe(t_list *tokens, int count);
bool ft_has_redirections_until_pipe(t_list *tokens);
t_list *ft_create_clean_token_list(t_list *tokens);
void ft_free_token_list(t_list *tokens);
// Déclarations pour les fonctions debug Phase 4-5
int ft_parse_redirections_debug(t_shell *shell, t_list *tokens, t_command *command);
char *ft_extract_redirection_file_debug(t_list *redir_token);
int ft_process_single_redirection_debug(t_shell *shell, t_list **current_token, t_command *command);
void ft_add_redirection_to_command(t_command *cmd, t_redir *redirection);
redir_type ft_token_type_to_redir_type(t_token_type token_type);

// Fonction debug pour afficher les tokens
void ft_debug_tokens(t_list *tokens);

// Override de ft_process_single_redirection
#define ft_process_single_redirection ft_process_single_redirection_compat
int ft_process_single_redirection_compat(t_list *redir_token, t_command *cmd);

// Stub functions pour permettre la compilation et tester l'executor
int ft_validate_token_list(t_list *tokens)
{
    (void)tokens;
    return (0); // Toujours valide pour le test (0 = succès)
}

int ft_count_word_tokens(t_list *tokens)
{
    t_list *curr;
    int count;
    
    curr = tokens;
    count = 0;
    while (curr)
    {
        if (curr->content.token->type == TOKEN_WORD)
            count++;
        curr = curr->next;
    }
    return (count);
}

char **ft_extract_command_args(t_list *tokens, int count)
{
    char **args;
    t_list *curr;
    int i;
    
    args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return (NULL);
    
    curr = tokens;
    i = 0;
    while (curr && i < count)
    {
        if (curr->content.token->type == TOKEN_WORD)
        {
            args[i] = ft_strdup(curr->content.token->value);
            i++;
        }
        curr = curr->next;
    }
    args[i] = NULL;
    return (args);
}

// PHASE 4-5: Fonction pour créer un nœud commande avec redirections
t_ast_node *ft_create_command_with_redirections(t_list *tokens, t_shell *shell)
{
    t_ast_node *node;
    t_command *cmd;
    char **args;
    int word_count;
    t_list *clean_tokens;
    
    // Crée une liste de tokens nettoyée (jusqu'au pipe)
    clean_tokens = ft_create_clean_token_list(tokens);
    if (!clean_tokens)
        return (NULL);
    
    // Compte les mots (arguments) dans la liste nettoyée
    word_count = ft_count_word_tokens_until_pipe(clean_tokens);
    if (word_count == 0)
    {
        ft_free_token_list(clean_tokens);
        return (NULL);
    }
    
    // Extrait les arguments de la liste nettoyée
    args = ft_extract_command_args_until_pipe(clean_tokens, word_count);
    if (!args)
    {
        ft_free_token_list(clean_tokens);
        return (NULL);
    }
    
    // Crée le nœud commande simple
    node = ft_create_simple_command_node(args);
    if (!node)
    {
        ft_free_token_list(clean_tokens);
        return (NULL);
    }
    
    // Traite les redirections si présentes dans la liste nettoyée
    cmd = (t_command *)node->data;
    if (ft_has_redirections_until_pipe(clean_tokens))
    {
        if (ft_parse_redirections(shell, clean_tokens, cmd) != 0)
        {
            ft_free_ast_node(node);
            ft_free_token_list(clean_tokens);
            return (NULL);
        }
        ft_printf("DEBUG: ft_parse_redirections a réussi\n");
    }
    else
    {
        ft_printf("DEBUG: Aucune redirection détectée\n");
    }
    
    ft_free_token_list(clean_tokens);
    return (node);
}

// PHASE 4-5: Compte les mots jusqu'au pipe
int ft_count_word_tokens_until_pipe(t_list *tokens)
{
    t_list *curr;
    int count;
    
    curr = tokens;
    count = 0;
    while (curr && curr->content.token->type != TOKEN_PIPE)
    {
        if (curr->content.token->type == TOKEN_WORD)
            count++;
        curr = curr->next;
    }
    return (count);
}

// PHASE 4-5: Extrait les arguments jusqu'au pipe
char **ft_extract_command_args_until_pipe(t_list *tokens, int count)
{
    char **args;
    t_list *curr;
    int i;
    
    args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return (NULL);
    
    curr = tokens;
    i = 0;
    while (curr && curr->content.token->type != TOKEN_PIPE && i < count)
    {
        if (curr->content.token->type == TOKEN_WORD)
        {
            args[i] = ft_strdup(curr->content.token->value);
            i++;
        }
        curr = curr->next;
    }
    args[i] = NULL;
    return (args);
}

// PHASE 4-5: Détection des redirections dans les tokens jusqu'au pipe
bool ft_has_redirections_until_pipe(t_list *tokens)
{
    t_list *curr;
    t_token *token;
    
    curr = tokens;
    while (curr && curr->content.token->type != TOKEN_PIPE)
    {
        token = curr->content.token;
        if (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT ||
            token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
            return (true);
        curr = curr->next;
    }
    return (false);
}

// PHASE 4-5: Crée une liste de tokens jusqu'au pipe (pour parsing propre)
t_list *ft_create_clean_token_list(t_list *tokens)
{
    t_list *clean_list;
    t_list *curr;
    t_token *token_copy;
    t_content content;
    
    clean_list = NULL;
    curr = tokens;
    
    // Copie tous les tokens jusqu'au pipe (ou jusqu'à la fin s'il n'y a pas de pipe)
    while (curr && curr->content.token->type != TOKEN_PIPE)
    {
        // Copie le token
        token_copy = malloc(sizeof(t_token));
        if (!token_copy)
        {
            ft_free_token_list(clean_list);
            return (NULL);
        }
        token_copy->type = curr->content.token->type;
        token_copy->value = ft_strdup(curr->content.token->value);
        if (!token_copy->value)
        {
            free(token_copy);
            ft_free_token_list(clean_list);
            return (NULL);
        }
        
        // Prépare le contenu et ajoute à la liste propre
        content.token = token_copy;
        if (!ft_lstadd_back(&clean_list, content, TYPE_TOKEN))
        {
            free(token_copy->value);
            free(token_copy);
            ft_free_token_list(clean_list);
            return (NULL);
        }
        
        curr = curr->next;
    }
    
    return (clean_list);
}

// PHASE 4-5: Libère une liste de tokens copiée
void ft_free_token_list(t_list *tokens)
{
    t_list *curr;
    t_list *next;
    t_token *token;
    
    curr = tokens;
    while (curr)
    {
        next = curr->next;
        if (curr->type == TYPE_TOKEN)
        {
            token = curr->content.token;
            if (token)
            {
                if (token->value)
                    free(token->value);
                free(token);
            }
        }
        free(curr);
        curr = next;
    }
}

t_ast_node *ft_create_simple_command_node(char **args)
{
    t_ast_node *node;
    t_command *cmd;
    
    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    
    node = malloc(sizeof(t_ast_node));
    if (!node)
    {
        free(cmd);
        return (NULL);
    }
    
    cmd->args = args;
    cmd->redirs = NULL;
    
    node->type = NODE_COMMAND;
    node->data = cmd;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

// Fonction ft_parse_redirections supprimée - utilise maintenant la vraie implémentation

void ft_free_ast_node(t_ast_node *node)
{
    if (!node)
        return;
    if (node->type == NODE_COMMAND && node->data)
    {
        t_command *cmd = (t_command *)node->data;
        if (cmd->args)
        {
            int i = 0;
            while (cmd->args[i])
                free(cmd->args[i++]);
            free(cmd->args);
        }
        free(cmd);
    }
    ft_free_ast_node(node->left);
    ft_free_ast_node(node->right);
    free(node);
}

// Fonction debug pour afficher les types de tokens
char *ft_get_token_type_name(t_token_type type)
{
    if (type == TOKEN_WORD) return ("WORD");
    if (type == TOKEN_OR) return ("OR");
    if (type == TOKEN_AND) return ("AND");
    if (type == TOKEN_PIPE) return ("PIPE");
    if (type == TOKEN_PAREN_OPEN) return ("LPAREN");
    if (type == TOKEN_PAREN_CLOSE) return ("RPAREN");
    if (type == TOKEN_REDIR_OUT) return ("REDIR_OUT");
    if (type == TOKEN_REDIR_IN) return ("REDIR_IN");
    if (type == TOKEN_APPEND) return ("APPEND");
    if (type == TOKEN_HEREDOC) return ("HEREDOC");
    return ("UNKNOWN");
}

void ft_debug_tokens(t_list *tokens)
{
    t_list *current;
    int count;
    
    current = tokens;
    count = 1;
    ft_printf("DEBUG: === TOKEN ANALYSIS ===\n");
    while (current)
    {
        ft_printf("DEBUG: Token %d - Type: %s, Value: '%s'\n", 
                  count, 
                  ft_get_token_type_name(current->content.token->type),
                  current->content.token->value ? current->content.token->value : "NULL");
        current = current->next;
        count++;
    }
    ft_printf("DEBUG: === END TOKENS ===\n");
}

t_ast_node *ft_parser(t_list *tokens, t_shell *shell)
{
    t_ast_node *result;
    
    // printf("DEBUG: ft_parser called with tokens = %p\n", tokens);
    // if (tokens)
    //     printf("DEBUG: first token type = %d, value = %s\n", 
    //            tokens->content.token->type, 
    //            tokens->content.token->value ? tokens->content.token->value : "NULL");
    
    // Utilise enhanced parser pour les commandes simples
    result = ft_enhanced_parser(tokens, shell);
    // printf("DEBUG: ft_enhanced_parser returned %p\n", result);
    return (result);
}

t_ast_node *ft_parser_phase8(t_list *tokens, t_shell *shell)
{
    t_list *pipe_pos;
    t_ast_node *left_node;
    t_ast_node *right_node;
    
    // Version améliorée : gère les pipes simples + redirections
    if (!tokens || !shell)
        return (NULL);
    
    // DEBUG: Affichage simple
    ft_printf("DEBUG: %d tokens parsed\n", ft_lstsize(tokens));
        
    // Cherche un pipe dans les tokens
    pipe_pos = tokens;
    while (pipe_pos && pipe_pos->content.token->type != TOKEN_PIPE)
        pipe_pos = pipe_pos->next;
        
    if (pipe_pos) // Il y a un pipe
    {
        // Crée le nœud gauche (avant pipe) avec redirections
        left_node = ft_create_command_with_redirections(tokens, shell);
        if (!left_node)
            return (NULL);
        
        // Crée le nœud droit (après pipe) avec redirections
        t_list *right_tokens = pipe_pos->next;
        if (!right_tokens)
        {
            ft_free_ast_node(left_node);
            return (NULL);
        }
        
        right_node = ft_create_command_with_redirections(right_tokens, shell);
        if (!right_node)
        {
            ft_free_ast_node(left_node);
            return (NULL);
        }
        
        // Crée le nœud pipe
        return (ft_create_pipe_node(left_node, right_node));
    }
    else // Pas de pipe, commande simple avec redirections possibles
    {
        return (ft_create_command_with_redirections(tokens, shell));
    }
}

void free_command_list(t_list *cmd_list)
{
    (void)cmd_list; // Stub pour éviter l'erreur
}

// REMOVED: ft_parse_enhanced stub - using real implementation from parse.c

// REMOVED: ft_create_pipe_node stub - using real implementation from operators.c

// PHASE 4-5: Version debug de ft_parse_redirections pour diagnostiquer
int ft_parse_redirections_debug(t_shell *shell, t_list *tokens, t_command *cmd)
{
    ft_printf("DEBUG: Entrée ft_parse_redirections_debug\n");
    
    if (!shell || !tokens || !cmd)
    {
        ft_printf("DEBUG: Paramètres NULL - shell:%p tokens:%p cmd:%p\n", shell, tokens, cmd);
        return (-1);
    }
    
    ft_printf("DEBUG: Appel ft_validate_redirection_syntax...\n");
    if (ft_validate_redirection_syntax(tokens) != 0)
    {
        ft_printf("DEBUG: ft_validate_redirection_syntax a échoué\n");
        return (-1);
    }
    ft_printf("DEBUG: ft_validate_redirection_syntax OK\n");
    
    ft_printf("DEBUG: Recherche des redirections dans les tokens...\n");
    t_list *current = tokens;
    int redir_count = 0;
    while (current)
    {
        ft_printf("DEBUG: Token type: %d, value: %s\n", 
                  current->content.token->type, 
                  current->content.token->value ? current->content.token->value : "NULL");
        
        if (ft_is_redirection_token(current->content.token))
        {
            redir_count++;
            ft_printf("DEBUG: Redirection trouvée! Type: %d\n", current->content.token->type);
            ft_printf("DEBUG: Token suivant: %s\n", 
                      current->next && current->next->content.token ? 
                      current->next->content.token->value : "NULL");
            
            if (ft_process_single_redirection_debug(shell, &current, cmd) != 0)
            {
                ft_printf("DEBUG: ft_process_single_redirection_debug a échoué pour token type %d\n", 
                          current->content.token->type);
                return (-1);
            }
            ft_printf("DEBUG: ft_process_single_redirection_debug OK pour token type %d\n", 
                      current->content.token->type);
        }
        current = current->next;
    }
    
    ft_printf("DEBUG: %d redirections traitées\n", redir_count);
    
    ft_printf("DEBUG: Vérification des conflits...\n");
    if (ft_check_redirection_conflicts(cmd->redirs) != 0)
    {
        ft_printf("DEBUG: ft_check_redirection_conflicts a échoué\n");
        return (-1);
    }
    ft_printf("DEBUG: ft_check_redirection_conflicts OK\n");
    
    ft_printf("DEBUG: ft_parse_redirections_debug terminé avec succès\n");
    return (0);
}

// PHASE 4-5: Version debug de ft_extract_redirection_file
char *ft_extract_redirection_file_debug(t_list *redir_token)
{
    t_list *next_token;
    
    ft_printf("DEBUG: ft_extract_redirection_file_debug - entrée\n");
    
    if (!redir_token)
    {
        ft_printf("DEBUG: redir_token est NULL\n");
        return (NULL);
    }
    
    ft_printf("DEBUG: redir_token OK, recherche next_token...\n");
    next_token = redir_token->next;
    
    if (!next_token || !next_token->content.token)
    {
        ft_printf("DEBUG: next_token est NULL ou invalide\n");
        return (NULL);
    }
    
    ft_printf("DEBUG: next_token trouvé: type=%d, value='%s'\n", 
              next_token->content.token->type, 
              next_token->content.token->value ? next_token->content.token->value : "NULL");
    
    if (next_token->content.token->type != TOKEN_WORD)
    {
        ft_printf("DEBUG: next_token n'est pas un TOKEN_WORD (type=%d)\n", 
                  next_token->content.token->type);
        return (NULL);
    }
    
    ft_printf("DEBUG: extraction du filename réussie: '%s'\n", 
              next_token->content.token->value);
    
    return (ft_strdup(next_token->content.token->value));
}

// PHASE 4-5: Version debug de ft_process_single_redirection  
int ft_process_single_redirection_debug(t_shell *shell __attribute__((unused)), t_list **current_token, t_command *command)
{
    t_token_type redir_type;
    char *filename;
    t_redir *redirection;
    
    ft_printf("DEBUG: ft_process_single_redirection_debug - entrée\n");
    
    if (!current_token || !*current_token || !(*current_token)->content.token)
    {
        ft_printf("DEBUG: current_token invalide\n");
        return (-1);
    }
    
    redir_type = (*current_token)->content.token->type;
    ft_printf("DEBUG: type de redirection: %d\n", redir_type);
    
    // Extraire le nom du fichier
    filename = ft_extract_redirection_file_debug(*current_token);
    if (!filename)
    {
        ft_printf("DEBUG: échec extraction filename\n");
        return (-1);
    }
    
    ft_printf("DEBUG: filename extrait: '%s'\n", filename);
    
    // Créer la structure redirection avec la vraie fonction
    redirection = ft_create_redirection(ft_token_type_to_redir_type(redir_type), filename);
    if (!redirection)
    {
        ft_printf("DEBUG: échec création redirection\n");
        free(filename);
        return (-1);
    }
    
    ft_printf("DEBUG: redirection créée avec succès\n");
    
    // Ajouter à la commande
    ft_add_redirection_to_command(command, redirection);
    
    ft_printf("DEBUG: redirection ajoutée à la commande\n");
    
    // Avancer de 2 tokens (redirection + fichier)
    *current_token = (*current_token)->next;
    if (*current_token)
        *current_token = (*current_token)->next;
    
    ft_printf("DEBUG: ft_process_single_redirection_debug terminé avec succès\n");
    
    free(filename);
    return (0);
}

// PHASE 4-5: Stub pour ft_add_redirection_to_command  
void ft_add_redirection_to_command(t_command *cmd, t_redir *redirection)
{
    t_content content;
    
    ft_printf("DEBUG: ft_add_redirection_to_command - ajout redirection\n");
    
    if (!cmd || !redirection)
    {
        ft_printf("DEBUG: cmd ou redirection NULL\n");
        return;
    }
        
    content.redir = redirection;
    ft_printf("DEBUG: content.redir = %p, TYPE_REDIR = %d\n", content.redir, TYPE_REDIR);
    
    if (!ft_lstadd_back(&cmd->redirs, content, TYPE_REDIR))
    {
        ft_printf("DEBUG: échec ajout redirection avec TYPE_REDIR=%d\n", TYPE_REDIR);
    }
    else
    {
        ft_printf("DEBUG: redirection ajoutée avec succès\n");
    }
}

// PHASE 4-5: Conversion t_token_type vers redir_type
redir_type ft_token_type_to_redir_type(t_token_type token_type)
{
    switch (token_type)
    {
        case TOKEN_REDIR_IN:
            return (REDIR_IN);
        case TOKEN_REDIR_OUT:
            return (REDIR_OUT);
        case TOKEN_APPEND:
            return (REDIR_APPEND);
        case TOKEN_HEREDOC:
            return (REDIR_HEREDOC);
        default:
            ft_printf("DEBUG: token_type %d non reconnu pour redirection\n", token_type);
            return (REDIR_OUT); // Valeur par défaut
    }
}

// PHASE 4-5: Version compatible de ft_process_single_redirection  
int ft_process_single_redirection_compat(t_list *redir_token, t_command *cmd)
{
    t_token_type redir_type;
    char *filename;
    t_redir *redirection;
    t_content content;
    
    if (!redir_token || !redir_token->content.token || !cmd)
        return (-1);
    
    redir_type = redir_token->content.token->type;
    
    // Extraire le nom du fichier
    filename = ft_extract_redirection_file_debug(redir_token);
    if (!filename)
        return (-1);
    
    // Créer la structure redirection avec la vraie fonction
    redirection = ft_create_redirection(ft_token_type_to_redir_type(redir_type), filename);
    if (!redirection)
    {
        free(filename);
        return (-1);
    }
    
    // Ajouter à la commande
    content.redir = redirection;
    if (!ft_lstadd_back(&cmd->redirs, content, TYPE_REDIR))
    {
        free(filename);
        return (-1);
    }
    
    free(filename);
    return (0);
}
