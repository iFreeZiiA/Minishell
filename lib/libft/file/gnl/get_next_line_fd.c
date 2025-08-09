/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 21:10:59 by jjorda            #+#    #+#             */
/*   Updated: 2024/11/07 12:36:03 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_fd.h"

char	*ft_free(char **str)
{
	free(*str);
	*str = NULL;
	return (NULL);
}

static char	*ft_set_line(char **buff)
{
	char	*temp;
	char	*result;
	int		nl_pos;

	temp = ft_strdup(*buff);
	ft_free(buff);
	if (!temp)
		return (NULL);
	if (!ft_gnl_strchr(temp, '\n'))
	{
		result = ft_strdup(temp);
		ft_free(&temp);
		return (result);
	}
	nl_pos = ft_gnl_strchr(temp, '\n');
	result = ft_substr(temp, 0, nl_pos);
	if (!result)
		return (ft_free(&temp));
	*buff = ft_substr(temp, nl_pos, ft_strlen(temp) - nl_pos);
	ft_free(&temp);
	if (!*buff || !*buff[0])
		ft_free(buff);
	return (result);
}

static char	*ft_fill_buff(char *left, int fd)
{
	char	*addbuff;
	int		b_read;

	b_read = 1;
	addbuff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!addbuff)
		return (ft_free(&left));
	while (b_read > 0 && !ft_gnl_strchr(left, '\n'))
	{
		b_read = read(fd, addbuff, BUFFER_SIZE);
		if (b_read < 0)
		{
			ft_free(&addbuff);
			return (ft_free(&left));
		}
		addbuff[b_read] = '\0';
		if (!left && b_read > 0)
			left = ft_strdup(addbuff);
		else if (b_read > 0)
			left = ft_gnl_strjoin(left, addbuff);
	}
	ft_free(&addbuff);
	return (left);
}

/**
 * Reads the next line from the given file descriptor.
 * 
 * @param fd The file descriptor to read from.
 * @return The next line from the file, or NULL if an error occurs or end of
 * file is reached.
 * 
 * Note: Memory for the line is allocated using malloc().
 *       If malloc fails or there is an error, the function returns NULL.
 */
char	*get_next_line(int fd)
{
	static char	*left[1400];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!left[fd] || !ft_gnl_strchr(left[fd], '\n'))
		left[fd] = ft_fill_buff(left[fd], fd);
	if (!left[fd])
		return (NULL);
	return (ft_set_line(&left[fd]));
}
