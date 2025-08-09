#include "header/minishell.h"

int main()
{
    char *env[] = {"PATH=/usr/bin", "TEST=hello", "HOME=/home/user", NULL};
    
    printf("get_env_index(env, \"TEST\") = %d\n", get_env_index(env, "TEST"));
    printf("get_env_index(env, \"PATH\") = %d\n", get_env_index(env, "PATH"));
    printf("get_env_index(env, \"NOTFOUND\") = %d\n", get_env_index(env, "NOTFOUND"));
    
    return 0;
}
