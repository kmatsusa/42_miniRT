/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmatsusa <kmatsusa@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 22:41:46 by kmatsusa          #+#    #+#             */
/*   Updated: 2021/03/10 18:38:02 by kmatsusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
// size_t		ft_strlen(char *s)
// {
// 	size_t i;

// 	i = 0;
// 	while (s[i])
// 		i++;
// 	return (i);
// }

size_t		ft_strchr(char *s, char c)
{
	size_t i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (i);
}

void		ft_strcpy(char *s1, char *s2, size_t n)
{
	while (n--)
		s1[n] = s2[n];
}

char		*ft_strdup(char *s1)
{
	size_t	n;
	char	*p;

	n = ft_strlen(s1);
	p = (char *)malloc(n + 1);
	if (p == NULL)
		return (NULL);
	p[n] = 0;
	ft_strcpy(p, s1, n);
	return (p);
}

char		*ft_strjoin(char *s1, char *s2, ssize_t n1, ssize_t n2)
{
	char *p;

	if (n1 == -1)
		n1 = ft_strlen(s1);
	p = (char *)malloc(n1 + n2 + 1);
	if (p == NULL)
		return (NULL);
	ft_strcpy(p, s1, n1);
	ft_strcpy(p + n1, s2, n2);
	p[n1 + n2] = 0;
	return (p);
}

void	ft_free(char *memo, char **line)
{
	free(memo);
	memo = NULL;
	free(*line);
}

int		ft_put_line(char **line, char **memo, char *buf)
{
	int		flag;
	size_t	n;
	char	*temp;

	n = ft_strchr(buf, '\n');
	temp = ft_strjoin(*line, buf, -1, n);
	if (temp == NULL)
		return (-1);
	free(*line);
	*line = temp;
	temp = NULL;
	flag = 0;
	if (buf[n] == '\n')
	{
		temp = ft_strdup(buf + n + 1);
		if (temp == NULL)
			return (-1);
		flag = 1;
	}
	free(*memo);
	*memo = temp;
	return (flag);
}

int		get_next_line(int fd, char **line)
{
	int			flag;
	static char	*memo;
	char		*buf;
	ssize_t		n;

	if (line == NULL || !(*line = (char *)malloc(1)))
		return (-1);
	*line[0] = 0;
	flag = 0;
	if (memo)
		flag = ft_put_line(line, &memo, memo);
	if (!(buf = (char *)malloc(10000 + 1)))
		flag = -1;
	while (flag == 0 && (n = read(fd, buf, 10000)) > 0)
	{
		buf[n] = 0;
		flag = ft_put_line(line, &memo, buf);
	}
	if (n < 0)
		flag = -1;
	free(buf);
	if (flag == -1)
		ft_free(memo, line);
	return (flag);
}
