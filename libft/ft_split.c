/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:47:04 by sguan             #+#    #+#             */
/*   Updated: 2024/11/23 21:39:51 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	in_word;
	size_t	count;

	in_word = 0;
	count = 0;
	while (*s)
	{
		if (*s != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static void	free_split(char	**result, size_t i)
{
	while (i > 0)
	{
		free(result[--i]);
	}
	free(result);
}

static char	*get_next_word(const char **s, char c)
{
	char		*word;
	size_t		len;
	size_t		i;

	while (**s == c)
		(*s)++;
	len = 0;
	while ((*s)[len] && (*s)[len] != c)
		len++;
	if (len == 0)
		return (NULL);
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = (*s)[i];
		i++;
	}
	word[len] = '\0';
	*s += len;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	num;
	size_t	i;
	char	*word;

	if (!s)
		return (NULL);
	num = count_words(s, c);
	result = (char **)malloc((num + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (i < num)
	{
		word = get_next_word(&s, c);
		if (!word)
		{
			free_split(result, i);
			return (NULL);
		}
		result[i++] = word;
	}
	result[i] = NULL;
	return (result);
}
