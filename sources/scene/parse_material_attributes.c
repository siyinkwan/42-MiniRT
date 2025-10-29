/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material_attributes.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 17:06:19 by sguan             #+#    #+#             */
/*   Updated: 2025/10/29 17:17:21 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	set_default_material(t_material *material)
{
	material->ambient = 0.1;
	material->diffuse = 0.9;
	material->specular = 0.9;
	material->shininess = 200.0;
	material->pattern_type = PATTERN_SOLID;
	material->bump = false;
}

bool	parse_mtl_attr(t_material *material, char **tokens,
			int count, int i)
{
	set_default_material(material);
	while (i < count)
	{
		if (ft_strcmp(tokens[i], "checker") == 0)
		{
			if (i + 2 >= count)
				return (error_exit("Checker needs color and scale"), false);
			material->pattern_type = PATTERN_CHECKER;
			if (!parse_color(tokens[i + 1], &material->pattern_color))
				return (false);
			material->pattern_scale = ft_atof(tokens[i + 2]);
			i += 3;
		}
		else if (ft_strcmp(tokens[i], "bump") == 0)
		{
			material->bump = true;
			i++;
		}
		else
			return (error_exit("Unknown material option"), false);
	}
	return (true);
}

