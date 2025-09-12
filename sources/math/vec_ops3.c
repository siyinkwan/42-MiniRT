/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_ops3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 20:00:45 by marvin            #+#    #+#             */
/*   Updated: 2025/08/25 20:00:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double		vec3_length(t_vec3 v)
{
    double  result;

    result = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return (result);
}

double		vec3_len_squared(t_vec3 v)
{
    double  result;

    result = v.x * v.x + v.y * v.y + v.z * v.z;
    return (result);
}
t_vec3		vec3_normalize(t_vec3 v)
{
    t_vec3  result;
    double  len = vec3_length(v);

    result.x = v.x / len;
    result.y = v.y / len;
    result.z = v.z / len;
    return (result);
}

bool	is_normalized(t_vec3 vec)
{
	double	length_squared;

	length_squared = vec3_len_squared(vec);
	return (fabs(length_squared - 1.0) < EPSILON);
}

double		vec3_distance(t_vec3 a, t_vec3 b)
{
    double  result;

    result = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
    return (result);
}
