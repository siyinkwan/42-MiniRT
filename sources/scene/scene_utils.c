/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:29:36 by sguan             #+#    #+#             */
/*   Updated: 2025/09/07 20:39:51 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool validate_scene(t_scene *scene)
{
	if (!scene)
		return (false);
	if (scene->ambient.ratio < 0)
	{
		printf("Error: Missing ambient light definition\n");
		return (false);
	}
	if (scene->camera.fov <= 0)
	{
		printf("Error: Missing camera definition\n");
		return (false);
	}
	if (!scene->objects)
	{
		printf("Error: Scene must contain at least one object\n");
		return (false);
	}
	return (true);
}

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

void	error_exit(char *msg)
{
	printf("Error: %s\n", msg);
	exit(1);
}

void	free_tokens(char **tokens)
{
    int	i;

    if (!tokens)
        return;
    i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}


void	free_scene(t_scene *scene)
{
	
}




