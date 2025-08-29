/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:30:37 by sguan             #+#    #+#             */
/*   Updated: 2025/08/29 16:18:47 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	intersect_cone(t_ray ray, t_cone cone)
{
	t_quadratic	q;
	double		k;
	t_vec3		oc;
		
	k = tan(cone.angle);
	oc = vec3_subtract(ray.origin, cone.apex);
	q.a = ray.direction.x * ray.direction.x + 
          ray.direction.z * ray.direction.z - 
          k * k * ray.direction.y * ray.direction.y;
    q.b = 2.0 * (oc.x * ray.direction.x + oc.z * ray.direction.z - 
                 k * k * oc.y * ray.direction.y);       
    q.c = oc.x * oc.x + oc.z * oc.z - k * k * oc.y * oc.y;
	q.discriminant = q.b * q.b - 4 * q.a * q.c;
	if (q.discriminant < 0)
		return (-1.0);
	else
	{
        q.t1 = (-q.b - sqrt(q.discriminant)) / (2 * q.a);
        q.t2 = (-q.b + sqrt(q.discriminant)) / (2 * q.a);
        if (ray_valid_t(ray, q.t1))
            return (q.t1);
        else if (ray_valid_t(ray, q.t2))
            return (q.t2);
    }
    return (-1.0);
}
