/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:58:53 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/11 11:27:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Affiche la liste des tokens (pour le débogage)
 * 
 * @param tok_h Tête de la liste des tokens
 */
static void print_token_list(t_list *tok_h)
{
    t_list *curr;
    t_token *token;
    
    ft_printf("\n=== LISTE DES TOKENS ===\n");
    if (!tok_h)
    {
        ft_printf("(aucun token)\n");
        return;
    }
    curr = tok_h;
    while (curr)
    {
        token = curr->content.token;
        if (!token)
        {
            ft_printf("(token invalide)\n");
            curr = curr->next;
            continue;
        }
        ft_printf("[%d] \"%s\"\n", token->type, token->value);
        curr = curr->next;
    }
    ft_printf("\n");
}

/**
 * @brief Initialise la structure shell pour les tests
 * 
 * @param shell Pointeur sur la structure shell
 * @param env Environnement système
 * @return int 0 en cas de succès, -1 en cas d'erreur
 */
static int init_shell(t_shell *shell, char **env)
{
    if (!shell)
        return (-1);
    shell->env = (t_env *)malloc(sizeof(t_env));
    if (!shell->env)
        return (-1);
    shell->env->env_vars = env;
    shell->env->local_env = NULL;
    shell->env->last_exit_code = 0;
    shell->token = NULL;
    shell->ast = NULL;
    shell->current_line = NULL;
    shell->prompt = NULL;
    shell->signal_received = 0;
    return (0);
}

/**
 * @brief Programme principal pour tester l'analyseur lexical et syntaxique
 */
int main(int argc, char **argv, char **env)
{
    t_shell shell;
    int result;
    
    if (argc != 2)
    {
        ft_printf("Usage: %s \"commande à analyser\"\n", argv[0]);
        return (1);
    }
    if (init_shell(&shell, env) < 0)
    {
        ft_printerr("Erreur: Initialisation de shell échouée\n");
        return (1);
    }
    shell.current_line = argv[1];
    ft_printf("=== ANALYSE DE LA COMMANDE ===\n");
    ft_printf("\"%s\"\n", shell.current_line);
    ft_printf("\n=== ÉTAPE 1: ANALYSE LEXICALE ===\n");
    shell.token = ft_lexing(&shell);
    if (!shell.token)
    {
        ft_printerr("Erreur: Analyse lexicale échouée\n");
        free(shell.env);
        return (1);
    }
    print_token_list(shell.token);
    ft_printf("=== ÉTAPE 2: ANALYSE SYNTAXIQUE ===\n");
    result = ft_parsing(&shell);
    if (result < 0 || !shell.ast)
    {
        ft_printerr("Erreur: Analyse syntaxique échouée (code %d)\n", result);
        ft_lstfree_t(shell.token);
        free(shell.env);
        return (1);
    }
    ft_printf("\n=== ARBRE SYNTAXIQUE ABSTRAIT ===\n");
    ft_print_ast(shell.ast, 0);
    ft_free_ast(shell.ast);
    ft_lstfree_t(shell.token);
    free(shell.env);
    ft_printf("\nAnalyse terminée avec succès.\n");
    return (0);
}
