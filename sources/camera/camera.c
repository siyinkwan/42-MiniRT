/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 18:27:17 by sguan             #+#    #+#             */
/*   Updated: 2025/10/31 14:59:33 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	camera_setup(t_camera *camera)
{
	t_vec3	world_up;
	t_vec3	right_ref;

	camera->forward = vec3_normalize(camera->forward);
	world_up.x = 0;
	world_up.y = 1;
	world_up.z = 0;
	if (fabs(vec3_dot(world_up, camera->forward)) > 0.99)
	{
		right_ref.x = 1;
		right_ref.y = 0;
		right_ref.z = 0;
	}
	else
		right_ref = world_up;
	camera->right = vec3_normalize(vec3_cross(right_ref, camera->forward));
	camera->up = vec3_normalize(vec3_cross(camera->forward, camera->right));
}
