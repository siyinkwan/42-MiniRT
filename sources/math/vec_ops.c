/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sykwan <sykwan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:05:55 by sykwan            #+#    #+#             */
/*   Updated: 2025/08/25 00:06:21 by sykwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"
#define EPSILON 1e-9

t_vec3		vec3_create(double x, double y, double z)
{
    t_vec3  result;

    result.x = x;
    result.y = y;
    result.z = z;

    return (result);
}

t_vec3		vec3_add(t_vec3 a, t_vec3 b)
{
    t_vec3  result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    
    return (result);
}
t_vec3		vec3_subtract(t_vec3 a, t_vec3 b)
{
    t_vec3  result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    
    return (result);
}
t_vec3		vec3_multiply(t_vec3 v, double scalar)
{
    t_vec3  result;

    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    
    return (result);
}
t_vec3		vec3_divide(t_vec3 v, double scalar)
{
    t_vec3  result;

    if (fabs(scalar) < EPSILON)
        return (vec3_create(0.0, 0.0, 0.0));
    result.x = v.x / scalar;
    result.y = v.y / scalar;
    result.z = v.z / scalar;
    
    return (result);
}
