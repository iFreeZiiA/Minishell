/* ************************************************************************** */
/*                                                                            */
/*   test_tokens_debug.c                                                      */
/*                                                                            */
/* ************************************************************************** */

#include "header/minishell.h"

void print_tokens_direct(t_list *tokens)
{
    t_list *curr;
    t_token *token;
    int i;
    
    curr = tokens;
    i = 0;
    printf("=== TOKENS DEBUG DIRECT ===\n");
    while (curr)
    {
        token = curr->content.token;
        printf("Token %d: type=%d, value='%s'\n", i, token->type, token->value ? token->value : "NULL");
        curr = curr->next;
        i++;
    }
    printf("==========================\n");
}

int main()
{
    t_shell shell;
    t_list *tokens;
    char test1[] = "echo test";
    char test2[] = "echo a && echo b";
    char test3[] = "(echo test)";
    
    // Initialisation basique
    ft_memset(&shell, 0, sizeof(t_shell));
    
    printf("TEST 1: '%s'\n", test1);
    shell.current_line = test1;
    tokens = ft_lexing_new(&shell);
    if (tokens)
    {
        print_tokens_direct(tokens);
        ft_debug_tokens(tokens);
        ft_lstfree_t(tokens);
    }
    
    printf("\nTEST 2: '%s'\n", test2);
    shell.current_line = test2;
    tokens = ft_lexing_new(&shell);
    if (tokens)
    {
        print_tokens_direct(tokens);
        ft_debug_tokens(tokens);
        ft_lstfree_t(tokens);
    }
    
    printf("\nTEST 3: '%s'\n", test3);
    shell.current_line = test3;
    tokens = ft_lexing_new(&shell);
    if (tokens)
    {
        print_tokens_direct(tokens);
        ft_debug_tokens(tokens);
        ft_lstfree_t(tokens);
    }
    
    return (0);
}
