// Test manuel des phases de parsing
#include "header/minishell.h"

void test_phase(int phase, char *input, char *description) {
    printf("\n=== PHASE %d: %s ===\n", phase, description);
    printf("Input: %s\n", input);
    
    t_shell shell;
    memset(&shell, 0, sizeof(t_shell));
    shell.line = input;
    
    // Test lexing
    t_list *tokens = ft_lexing_simple(&shell);
    if (!tokens) {
        printf("❌ Échec lexing\n");
        return;
    }
    printf("✅ Lexing OK - %d tokens\n", ft_lstsize(tokens));
    
    // Test parsing 
    t_ast_node *ast = ft_parser_phase8(tokens, &shell);
    if (!ast) {
        printf("❌ Échec parsing\n");
        ft_lstleak(&tokens, ft_clean_node_tok);
        return;
    }
    printf("✅ Parsing OK - Type: %d\n", ast->type);
    
    // Test exécution
    t_list *cmd_list = ft_ast_to_command_list(ast);
    if (!cmd_list) {
        printf("❌ Échec conversion AST\n");
    } else {
        printf("✅ AST to commands OK - %d commands\n", ft_lstsize(cmd_list));
        ft_free_command_list(cmd_list);
    }
    
    ft_free_ast_node(ast);
    ft_lstleak(&tokens, ft_clean_node_tok);
}

int main() {
    printf("🧪 TEST MANUEL DES PHASES DE PARSING\n");
    
    // Phase 1-2: Tokenisation
    test_phase(2, "echo hello", "Tokenisation simple");
    
    // Phase 3: Quotes et expansion  
    test_phase(3, "echo 'hello world'", "Quotes simples");
    test_phase(3, "echo \"$USER\"", "Quotes doubles + expansion");
    
    // Phase 4-5: Redirections
    test_phase(4, "echo test > file", "Redirection sortie");
    test_phase(4, "cat < input", "Redirection entrée");
    
    // Phase 6: Pipes
    test_phase(6, "echo test | cat", "Pipe simple");
    test_phase(6, "ls | grep txt | wc -l", "Pipes multiples");
    
    // Phase 7: Opérateurs logiques
    test_phase(7, "echo a && echo b", "Opérateur AND");
    test_phase(7, "false || echo backup", "Opérateur OR");
    
    // Phase 8: Parenthèses
    test_phase(8, "(echo a && echo b) | wc", "Parenthèses + pipe");
    
    return 0;
}
