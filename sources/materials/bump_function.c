/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:14:43 by sguan             #+#    #+#             */
/*   Updated: 2025/11/07 17:19:04 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static double	smooth_interp(double a, double b, double t)
{
	double	ft;

	ft = t * 3.14159265;
	ft = (1.0 - cos(ft)) * 0.5;
	return (a * (1.0 - ft) + b * ft);
}

static double	hash2d(int x, int y)
{
	double	n;

	n = sin(x * 127.1 + y * 311.7) * 43758.5453;
	return (n - floor(n));
}

static double	smooth_noise(double x, double y)
{
	int		ix;
	int		iy;
	double	fx;
	double	fy;
	double	v;

	ix = (int)floor(x);
	iy = (int)floor(y);
	fx = x - ix;
	fy = y - iy;
	v = smooth_interp(hash2d(ix, iy), hash2d(ix + 1, iy), fx);
	v = smooth_interp(v, smooth_interp(hash2d(ix, iy + 1),
				hash2d(ix + 1, iy + 1), fx), fy);
	return (v);
}

double	bump_func(double u, double v)
{
	double	result;
	double	amp;
	double	freq;
	int		i;

	result = 0.0;
	amp = 1.0;
	freq = 1.0;
	i = 0;
	while (i < 6)
	{
		result += smooth_noise(u * freq * 5.0, v * freq * 5.0) * amp;
		amp *= 0.5;
		freq *= 2.0;
		i++;
	}
	return ((result - 0.5) * 20.0);
}
