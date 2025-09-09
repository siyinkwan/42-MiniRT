/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:01:03 by sguan             #+#    #+#             */
/*   Updated: 2025/09/09 13:04:17 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// Sphere:
// sp             0.0,0.0,20.6        12.6             10,0,255
// ∗ identifier: sp
// ∗ x, y, z coordinates of the sphere center: 0.0,0.0,20.6
// ∗ the sphere diameter: 12.6
// ∗ R,G,B colors in the range [0-255]: 10, 0, 255

bool	parse_material_options(t_scene *scene, char **tokens)
{
	
}



bool	parse_sphere(t_scene *scene, char **tokens)
{
	t_vec3		center;
	double		diameter;
	t_vec3		color;
	t_object	*new_obj;

	if (count_tokens(tokens) < 4)
		return (printf("Error: Sphere requires at least 3 parameters\n"), false);
	diameter = ft_atof(tokens[2]);
	if (!parse_vec3(tokens[1], &center) || diameter <= 0.0
		|| !parse_color(tokens[3], &color))
		return (printf("Error: Invalid sphere parameter\n"), false);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		return (printf("Error: Memory allocation failed\n"), false);
	new_obj->type = OBJECT_SPHERE;
	new_obj->data.sphere.center = center;
	new_obj->data.sphere.radius = diameter / 2.0;
	new_obj->data.sphere.material.color = color;
	parse_material_options();
	scene->objects = new_obj;
	

	return (true);
}
// parse_sphere()
// parse_plane()
// parse_cylinder()
// parse_cone()
