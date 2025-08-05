// Test pour debugger get_path
#include "../../header/minishell.h"

int main(void)
{
    char *envp[] = {
        "PATH=/usr/bin:/bin:/usr/local/bin",
        "HOME=/home/mel",
        NULL
    };
    
    char *result;
    
    printf("Test get_path:\n");
    printf("Command: ls\n");
    printf("PATH in envp[0]: %s\n", envp[0]);
    
    // Test direct
    printf("Testing access to /usr/bin/ls: ");
    if (access("/usr/bin/ls", X_OK) == 0)
        printf("OK\n");
    else
        printf("FAILED\n");
    
    result = get_path_debug("ls", envp);
    
    if (result)
    {
        printf("Found: %s\n", result);
        free(result);
    }
    else
    {
        printf("NOT FOUND\n");
    }
    
    return (0);
}
