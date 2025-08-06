#include "../../header/minishell.h"

// Déclarations pour les fonctions debug Phase 4-5
int ft_parse_redirections_debug(t_shell *shell, t_list *tokens, t_command *command);
char *ft_extract_redirection_file_debug(t_list *redir_token);
int ft_process_single_redirection_debug(t_shell *shell, t_list **current_token, t_command *command);
void ft_add_redirection_to_command(t_command *cmd, t_redir *redirection);
redir_type ft_token_type_to_redir_type(t_token_type token_type);
// Fonction debug pour afficher les tokens
void ft_debug_tokens(t_list *tokens);
char *ft_get_token_type_name(t_token_type type);
// Override de ft_process_single_redirection
#define ft_process_single_redirection ft_process_single_redirection_compat
int ft_process_single_redirection_compat(t_list *redir_token, t_command *cmd);
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
            return (REDIR_OUT);
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
