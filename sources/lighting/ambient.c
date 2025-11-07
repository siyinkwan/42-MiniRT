/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambient.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:45:15 by sguan             #+#    #+#             */
/*   Updated: 2025/11/07 17:07:50 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3	calculate_ambient(t_scene *scene, t_material *material)
{
	t_vec3	ambient;

	ambient.x = scene->ambient.color.x * material->color.x
		* scene->ambient.ratio * material->ambient;
	ambient.y = scene->ambient.color.y * material->color.y
		* scene->ambient.ratio * material->ambient;
	ambient.z = scene->ambient.color.z * material->color.z
		* scene->ambient.ratio * material->ambient;
	return (ambient);
}
