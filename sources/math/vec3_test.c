#include "../includes/minirt.h"
#include <stdio.h>

void test_vec3_dot() {
    t_vec3 a = {1, 0, 0};
    t_vec3 b = {0, 1, 0};
    t_vec3 c = {2, 0, 0};
    t_vec3 zero = {0, 0, 0};
    printf("dot(a, b) = %f (expect 0)\n", vec3_dot(a, b));
    printf("dot(a, c) = %f (expect 2)\n", vec3_dot(a, c));
    printf("dot(a, zero) = %f (expect 0)\n", vec3_dot(a, zero));
}

void test_vec3_cross() {
    t_vec3 a = {1, 0, 0};
    t_vec3 b = {0, 1, 0};
    t_vec3 c = {2, 0, 0};
    t_vec3 zero = {0, 0, 0};
    t_vec3 result = vec3_cross(a, b);
    printf("cross(a, b) = (%f, %f, %f) (expect 0,0,1)\n", result.x, result.y, result.z);
    result = vec3_cross(a, c);
    printf("cross(a, c) = (%f, %f, %f) (expect 0,0,0)\n", result.x, result.y, result.z);
    result = vec3_cross(a, zero);
    printf("cross(a, zero) = (%f, %f, %f) (expect 0,0,0)\n", result.x, result.y, result.z);
}

void test_vec3_length() {
    t_vec3 a = {1, 0, 0};
    t_vec3 b = {0, 1, 0};
    t_vec3 c = {0, 0, 1};
    t_vec3 d = {1, 1, 1};
    t_vec3 zero = {0, 0, 0};
    printf("length(a) = %f (expect 1)\n", vec3_length(a));
    printf("length(b) = %f (expect 1)\n", vec3_length(b));
    printf("length(c) = %f (expect 1)\n", vec3_length(c));
    printf("length(d) = %f (expect ~1.732)\n", vec3_length(d));
    printf("length(zero) = %f (expect 0)\n", vec3_length(zero));
}

void test_vec3_length_squared() {
    t_vec3 d = {1, 1, 1};
    t_vec3 zero = {0, 0, 0};
    printf("length_squared(d) = %f (expect 3)\n", vec3_length_squared(d));
    printf("length_squared(zero) = %f (expect 0)\n", vec3_length_squared(zero));
}

void test_vec3_normalize() {
    t_vec3 a = {2, 0, 0};
    t_vec3 b = {0, 0, 0};
    t_vec3 result = vec3_normalize(a);
    printf("normalize(2,0,0) = (%f, %f, %f) (expect 1,0,0)\n", result.x, result.y, result.z);
    result = vec3_normalize(b);
    printf("normalize(0,0,0) = (%f, %f, %f) (expect 0,0,0 or error)\n", result.x, result.y, result.z);
}

void test_vec3_distance() {
    t_vec3 a = {0, 0, 0};
    t_vec3 b = {1, 0, 0};
    t_vec3 c = {1, 2, 3};
    t_vec3 d = {4, 5, 6};
    printf("distance(a, b) = %f (expect 1)\n", vec3_distance(a, b));
    printf("distance(c, d) = %f (expect ~5.196)\n", vec3_distance(c, d));
}

int main() {
    test_vec3_dot();
    test_vec3_cross();
    test_vec3_length();
    test_vec3_length_squared();
    test_vec3_normalize();
    test_vec3_distance();
    return 0;
}
