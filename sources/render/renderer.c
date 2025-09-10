/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:40:21 by sguan             #+#    #+#             */
/*   Updated: 2025/09/10 16:16:14 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
typedef struct s_hit
{
	bool		hit;		// Did intersection occur?
	double		t;			// Distance parameter along ray
	t_vec3		point;		// 3D intersection point
	t_vec3		normal;		// Surface normal at intersection (normalized)
	t_material	*material;	// Pointer to intersected object's material
	t_object	*object;	// Pointer to intersected object
}	t_hit;

void	put_pixel(t_minirt *minirt, int x, int y, int color)
{
	char	*pixel_addr;
	int		bytes_per_pixel;

	pixel_addr = NULL;
	bytes_per_pixel = minirt->bits_per_pixel / 8;
	if (x >= 0 && y >= 0 && x < minirt->width && y < minirt->height)
	{
		pixel_addr = minirt->img_data + y * minirt->line_length + x * bytes_per_pixel;
		*(unsigned int*)pixel_addr = color;
	}
}
int	vec3_to_rgb(t_vec3 color)
{
	
}

void	render_scene(t_minirt *minirt)
{
	t_ray	ray;
	int		x;
	int		y;
	t_hit	hit;
	int		color;
	
	y = 0.0;
	while (y < minirt->height)
	{
		x = 0.0;
		while (x < minirt->width)
		{
			ray = generate_ray(&minirt->scene.camera, x, y, minirt->width, minirt->height);
			hit = intersect_scene(ray, &minirt->scene);
			if (hit.hit)
				color = 0xFFFDD0;
			else
				color = 0x000000;
			put_pixel(minirt, (int)x, (int)y, color);
			x = x + 1.0;
		}
		y = y + 1.0;
	}
}

