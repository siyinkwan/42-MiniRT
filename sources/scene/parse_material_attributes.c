/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material_attributes.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 17:06:19 by sguan             #+#    #+#             */
/*   Updated: 2025/10/31 14:38:43 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	set_default_material(t_material *material)
{
	material->ambient = 0.5;
	material->diffuse = 0.7;
	material->specular = 0.8;
	material->shininess = 1000;
	material->pattern_type = PATTERN_SOLID;
	material->pattern_color = vec3_create(1.0, 1.0, 1.0);
	material->pattern_scale = 1.0;
	material->bump = false;
}

bool	parse_mtl_attr(t_material *material, char **tokens,
			int count, int i)
{
	set_default_material(material);
	if (i < count)
		material->ambient = ft_atof(tokens[i++]);
	if (i < count)
		material->diffuse = ft_atof(tokens[i++]);
	if (i < count)
		material->specular = ft_atof(tokens[i++]);
	if (i < count)
		material->shininess = ft_atof(tokens[i++]);
	if (i < count)
		material->pattern_type = (t_pattern_type)ft_atoi(tokens[i++]);
	if (i < count)
	{
		if (!parse_color(tokens[i++], &material->pattern_color))
			return (error_exit("Invalid pattern color"), false);
	}
	if (i < count)
		material->pattern_scale = ft_atof(tokens[i++]);
	if (i < count)
		material->bump = (ft_atoi(tokens[i]) != 0);
	return (true);
}

