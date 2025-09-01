/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_setup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 18:27:17 by sguan             #+#    #+#             */
/*   Updated: 2025/09/01 21:24:23 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	camera_setup(t_camera *camera)
{
	t_vec3	world_up;
	t_vec3	right_ref;  // 参考向量
	
	world_up.x = 0;
	world_up.y = 1;
	world_up.z = 0;
	//todo
	// 检查 forward 是否与 world_up 平行
	// 如果平行，选择不同的参考向量
	if (/* forward 与 world_up 接近平行 */)
	{
		// 使用 X 轴作为参考
		right_ref.x = 1;
		right_ref.y = 0;
		right_ref.z = 0;
	}
	else
	{
		right_ref = world_up;
	}
	
	camera->right = vec3_cross(camera->forward, right_ref);
	camera->up = vec3_cross(camera->right, camera->forward);
}