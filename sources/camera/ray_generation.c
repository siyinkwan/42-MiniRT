/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_generation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:48:33 by sguan             #+#    #+#             */
/*   Updated: 2025/10/31 14:48:29 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_ray	generate_ray(t_camera *camera, double pixel_x, double pixel_y, int width, int height)
{
	double		fov_rad;
	t_viewport	viewport;
	double	u;
	double	v;
	t_ray	ray;

	fov_rad = (camera->fov * PI) / 180;
	viewport.height = 2 * tan(fov_rad / 2);
	viewport.width = viewport.height * camera->aspect_ratio;
	u = (pixel_x + 0.5) / (double)width;
	v = (pixel_y + 0.5) / (double)height;
	viewport.x = (u - 0.5) * viewport.width;
	viewport.y = (0.5 - v) * viewport.height;
	viewport.pos_in_world = vec3_add(camera->position, camera->forward);
	viewport.pos_in_world = vec3_add(viewport.pos_in_world, vec3_scale(camera->right, viewport.x));	
	viewport.pos_in_world = vec3_add(viewport.pos_in_world, vec3_scale(camera->up, viewport.y));
	ray = ray_create(camera->position, vec3_subtract(viewport.pos_in_world, camera->position));
	return (ray);
}
