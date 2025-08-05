/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_debug.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: debug                                                                 */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static char	*search_cmd_in_paths_debug(char **paths, char *cmd)
{
	char	*joined;
	char	*path;
	int		i;

	i = 0;
	printf("Searching cmd '%s' in paths:\n", cmd);
	while (paths[i])
	{
		printf("  Path %d: %s\n", i, paths[i]);
		joined = ft_strjoin(paths[i], "/");
		if (!joined)
		{
			printf("  ERROR: ft_strjoin failed\n");
			return (NULL);
		}
		path = ft_strjoin(joined, cmd);
		free(joined);
		if (!path)
		{
			printf("  ERROR: second ft_strjoin failed\n");
			return (NULL);
		}
		printf("  Trying: %s\n", path);
		if (access(path, X_OK) == 0)
		{
			printf("  FOUND: %s\n", path);
			return (path);
		}
		printf("  Not found\n");
		free(path);
		i++;
	}
	printf("No command found in any path\n");
	return (NULL);
}

char	*get_path_debug(char *cmd, char **envp)
{
	char	**paths;
	char	*path;

	printf("get_path_debug called with cmd='%s'\n", cmd ? cmd : "NULL");
	
	if (!cmd)
		return (NULL);
		
	if (ft_strchr(cmd, '/'))
	{
		printf("Command contains '/', returning strdup\n");
		return (ft_strdup(cmd));
	}
	
	printf("Looking for PATH in envp...\n");
	while (*envp)
	{
		printf("  env: %s\n", *envp);
		if (!ft_strncmp(*envp, "PATH=", 5))
		{
			printf("Found PATH: %s\n", *envp);
			break;
		}
		envp++;
	}
	
	if (!*envp)
	{
		printf("PATH not found in envp\n");
		return (NULL);
	}
	
	printf("Splitting PATH: %s\n", *envp + 5);
	paths = ft_split(*envp + 5, ':');
	if (!paths)
	{
		printf("ft_split failed\n");
		return (NULL);
	}
	
	path = search_cmd_in_paths_debug(paths, cmd);
	ft_free_arr(paths);
	return (path);
}
