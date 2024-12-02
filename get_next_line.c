/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 10:30:54 by oessoufi          #+#    #+#             */
/*   Updated: 2024/12/02 16:02:41 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*make_line(char *str, int *i)
{
	char	*line;
	char	*str_n;
	int		to_allocate;
	int		j;

	j = 0;
	str_n = ft_strchr((str + *i), '\n');
	if (str_n)
		to_allocate = str_n - (str + *i) + 1;
	else
		to_allocate = ft_strlen(str + *i);
	line = malloc(to_allocate + 1);
	if (line == NULL)
		return (NULL);
	while (str[*i] != '\n' && str[*i] != '\0')
		line[j++] = str[(*i)++];
	if (str[(*i)] == '\n')
		line[j++] = str[(*i)++];
	line[j] = '\0';
	return (line);
}

void	read_line(char *buffer, char **str, int fd, int *i)
{
	char	*previous_str;
	int		bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = 0;
		previous_str = *str;
		*str = ft_strjoin(previous_str, buffer);
		free(previous_str);
		if (*str == NULL)
			return ;
		if (ft_strchr(buffer, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytes_read == -1)
		reset_static(str, i);
}

void	reset_static(char **str, int *i)
{
	free(*str);
	*i = 0;
	*str = NULL;
}

char	*get_next_line(int fd)
{
	char			*buffer;
	static char		*str;
	static int		i;
	char			*line;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (buffer == NULL)
	{
		reset_static(&str, &i);
		return (NULL);
	}
	read_line(buffer, &str, fd, &i);
	free(buffer);
	if (!str || str[i] == '\0')
	{
		reset_static(&str, &i);
		return (NULL);
	}
	line = make_line(str, &i);
	if (str[i] == '\0' || line == NULL)
		reset_static(&str, &i);
	return (line);
}
