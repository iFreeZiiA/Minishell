/* ************************************************************************** */
/*   test_simple_tokens.c - Test direct des tokens                           */
/* ************************************************************************** */

#include "header/minishell.h"

// Test simple pour les tokens
int main()
{
    char test_and[] = "&&";
    char test_or[] = "||"; 
    char test_paren_open[] = "(";
    char test_paren_close[] = ")";
    
    printf("=== TEST DIRECT DES TOKENS ===\n");
    
    printf("Test '&&': is_operator=%d, type=%d\n", 
           ft_is_operator(test_and[0]), 
           ft_get_operator_type(test_and));
           
    printf("Test '||': is_operator=%d, type=%d\n", 
           ft_is_operator(test_or[0]), 
           ft_get_operator_type(test_or));
           
    printf("Test '(': is_operator=%d, type=%d\n", 
           ft_is_operator(test_paren_open[0]), 
           ft_get_operator_type(test_paren_open));
           
    printf("Test ')': is_operator=%d, type=%d\n", 
           ft_is_operator(test_paren_close[0]), 
           ft_get_operator_type(test_paren_close));
    
    // Test de référence 
    char test_pipe[] = "|";
    printf("Test '|' (ref): is_operator=%d, type=%d\n", 
           ft_is_operator(test_pipe[0]), 
           ft_get_operator_type(test_pipe));
    
    printf("=== VALEURS ATTENDUES ===\n");
    printf("TOKEN_AND = %d\n", TOKEN_AND);
    printf("TOKEN_OR = %d\n", TOKEN_OR);
    printf("TOKEN_PAREN_OPEN = %d\n", TOKEN_PAREN_OPEN);
    printf("TOKEN_PAREN_CLOSE = %d\n", TOKEN_PAREN_CLOSE);
    printf("TOKEN_PIPE = %d\n", TOKEN_PIPE);
    
    return (0);
}
