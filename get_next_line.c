/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdetourn <gdetourn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 15:24:11 by gdetourn          #+#    #+#             */
/*   Updated: 2023/12/19 10:04:41 by gdetourn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*ft_next_line : clean buffer by removing line*/
char	*ft_next_line(char *stash)
{
	char	*clean_stash;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	clean_stash = ft_calloc((ft_strlen(stash) - i + 1), sizeof(char));
	if (!clean_stash)
		return (NULL);
	i++;
	while (stash[i])
		clean_stash[j++] = stash[i++];
	free(stash);
	return (clean_stash);
}

/*ft_get_line : Get line until '\n' from stash*/
char	*ft_get_line(char *stash)
{
	char	*line;
	int		i;

	i = 0;
	if (!stash[0])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	line = ft_calloc((i + 2), sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] && stash[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*newstr;
	int		total_len;

	total_len = ft_strlen(s1) + ft_strlen(s2);
	newstr = malloc(sizeof(char) * (total_len + 1));
	if (newstr == NULL)
		return (NULL);
	if (total_len == 0)
	{
		free(s1);
		free(newstr);
		return (ft_calloc(1, 1));
	}
	ft_strlcpy(newstr, s1, (ft_strlen(s1) + 1));
	ft_strlcat(newstr, s2, (total_len + 1));
	free(s1);
	return (newstr);
}

/*ft_read_text : Read text until BUFFER_SIZE in buffer
and join in stash*/
char	*ft_read_text(int fd, char *stash)
{
	char	*buffer;
	int		read_text;

	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (NULL);
	read_text = 1;
	while (read_text > 0)
	{
		read_text = read(fd, buffer, BUFFER_SIZE);
		if (read_text == -1)
		{
			free(buffer);
			free(stash);
			return (NULL);
		}
		buffer[read_text] = 0;
		stash = ft_strjoin_and_free(stash, buffer);
		if (ft_strchr(stash, '\n'))
			break ;
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer)
		buffer = ft_calloc(1, sizeof(char));
	buffer = ft_read_text(fd, buffer);
	if (!buffer)
	{
		free(buffer);
		return (NULL);
	}
	line = ft_get_line(buffer);
	buffer = ft_next_line(buffer);
	return (line);
}
