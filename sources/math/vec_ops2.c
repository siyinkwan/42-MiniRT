/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_ops2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:45:37 by marvin            #+#    #+#             */
/*   Updated: 2025/08/25 13:45:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

double		vec3_dot(t_vec3 a, t_vec3 b)
{
    double  result;

    result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    return (result);
}
t_vec3		vec3_cross(t_vec3 a, t_vec3 b)
{
    
}