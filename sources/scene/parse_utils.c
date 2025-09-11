/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:47:29 by sguan             #+#    #+#             */
/*   Updated: 2025/09/11 22:42:09 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	count_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (0);
	while (tokens[i])
		i++;
	return (i);
}

bool	validate_cone_params(t_cone *co, char **tokens, double angle_degrees)
{
	if (!parse_vec3(tokens[1], &co->apex) || !parse_vec3(tokens[2], &co->axis)
		|| angle_degrees <= 0.0 || angle_degrees >= 90.0 || co->height <= 0.0
		|| !parse_color(tokens[5], &co->material.color)
		|| !is_normalized(co->axis))
		return (false);
	co->angle = angle_degrees * PI / 180.0;
	return (true);
}

bool	has_invalid_chars(char *line)
{
    int	i;

    i = 0;
    while (line[i])
    {
        if (!ft_isdigit(line[i]) && !ft_isalpha(line[i])
            && line[i] != ' ' && line[i] != '\t' && line[i] != ','
            && line[i] != '.' && line[i] != '-' && line[i] != '+'
			&& line[i] != '\n')
            return (true);
        i++;
    }
    return (false);
}
