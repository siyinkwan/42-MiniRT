#include "minirt.h"
#include <stdio.h>
#include <assert.h>

void test_sphere_intersection(void)
{
    printf("Testing sphere intersection...\n");
    
    // 创建测试球体
    t_sphere sphere;
    sphere.center = vec3_create(0, 0, -5);
    sphere.radius = 1.0;
    
    // 测试1: 正面命中
    t_ray ray1 = ray_create(vec3_create(0, 0, 0), vec3_create(0, 0, -1));
    t_hit hit1 = intersect_sphere(ray1, sphere);
    
    printf("  Hit: %s, t: %.2f\n", hit1.hit ? "YES" : "NO", hit1.t);
    assert(hit1.hit == true);
    assert(hit1.t > 0.0);
    
    // 测试2: 错过
    t_ray ray2 = ray_create(vec3_create(5, 0, 0), vec3_create(0, 0, -1));
    t_hit hit2 = intersect_sphere(ray2, sphere);
    
    printf("  Miss: %s\n", hit2.hit ? "NO (ERROR)" : "YES");
    assert(hit2.hit == false);
    
    printf("Sphere tests PASSED!\n\n");
}

void test_plane_intersection(void)
{
    printf("Testing plane intersection...\n");
    
    t_plane plane;
    plane.point = vec3_create(0, -1, 0);
    plane.normal = vec3_create(0, 1, 0);
    
    // 测试1: 从上方击中
    t_ray ray1 = ray_create(vec3_create(0, 5, 0), vec3_create(0, -1, 0));
    t_hit hit1 = intersect_plane(ray1, plane);
    
    printf("  Hit from above: %s, t: %.2f\n", hit1.hit ? "YES" : "NO", hit1.t);
    assert(hit1.hit == true);
    
    // 测试2: 平行射线
    t_ray ray2 = ray_create(vec3_create(0, 0, 0), vec3_create(1, 0, 0));
    t_hit hit2 = intersect_plane(ray2, plane);
    
    printf("  Parallel miss: %s\n", hit2.hit ? "NO (ERROR)" : "YES");
    assert(hit2.hit == false);
    
    printf("Plane tests PASSED!\n\n");
}

void test_cylinder_intersection(void)
{
    printf("Testing cylinder intersection...\n");
    
    t_cylinder cylinder;
    cylinder.center = vec3_create(0, 0, -5);
    cylinder.axis = vec3_create(0, 1, 0);
    cylinder.radius = 1.0;
    cylinder.height = 2.0;
    
    // 测试侧面命中
    t_ray ray1 = ray_create(vec3_create(-2, 0, -5), vec3_create(1, 0, 0));
    t_hit hit1 = intersect_cylinder(ray1, cylinder);
    
    printf("  Side hit: %s, t: %.2f\n", hit1.hit ? "YES" : "NO", hit1.t);
    
    // 测试端面命中（如果实现了）
    t_ray ray2 = ray_create(vec3_create(0, 5, -5), vec3_create(0, -1, 0));
    t_hit hit2 = intersect_cylinder(ray2, cylinder);
    
    printf("  Cap hit: %s, t: %.2f\n", hit2.hit ? "YES" : "NO", hit2.t);
    
    printf("Cylinder tests completed!\n\n");
}

void test_cone_intersection(void)
{
    printf("Testing cone intersection...\n");
    
    t_cone cone;
    cone.apex = vec3_create(0, 0, -5);
    cone.axis = vec3_create(0, 1, 0);
    cone.angle = PI / 6;
    cone.height = 2.0;
    
    // 测试曲面命中
    t_ray ray1 = ray_create(vec3_create(-2, 1, -5), vec3_create(1, 0, 0));
    t_hit hit1 = intersect_cone(ray1, cone);
    
    printf("  Surface hit: %s, t: %.2f\n", hit1.hit ? "YES" : "NO", hit1.t);
    
    printf("Cone tests completed!\n\n");
}

int main(void)
{
    printf("=== INTERSECTION TESTS ===\n\n");
    
    test_sphere_intersection();
    test_plane_intersection();
    test_cylinder_intersection();
    test_cone_intersection();
    
    printf("All basic tests completed!\n");
    return (0);
}