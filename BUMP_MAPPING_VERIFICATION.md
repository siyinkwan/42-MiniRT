# Bump Mapping 可视化验证指南

## 如何验证 Bump Mapping 是否正确实现

### 1. 基本概念
Bump mapping 通过修改表面法线来模拟凹凸不平的表面，而不改变实际几何形状。

### 2. 视觉验证要点

#### 应该看到的效果：
✓ 表面上有明暗变化的波纹/纹理
✓ 高光位置随着表面"凹凸"而变化
✓ 从不同角度看，纹理应该有视差效果
✓ 光照应该根据扰动后的法线计算（产生细节）

#### 不应该看到的：
✗ 物体轮廓改变（bump mapping不改变几何）
✗ 完全平滑的表面（如果启用了bump）
✗ 静态的、不随光照变化的纹理

### 3. 材质属性格式

**重要**：材质属性必须按以下顺序提供（空格分隔）：
```
ambient diffuse specular shininess pattern_type pattern_color pattern_scale bump
```

示例：
```
# 无 bump 的球体
sp 0,0,5  2  255,100,100  0.6 0.4 0.5 50 0 255,255,255 1.0 0

# 有 bump 的球体（最后一个参数是 1）
sp 0,0,5  2  255,100,100  0.6 0.4 0.5 50 0 255,255,255 1.0 1
```

参数说明：
- `ambient`: 环境光系数 (0.0-1.0)
- `diffuse`: 漫反射系数 (0.0-1.0)
- `specular`: 镜面反射系数 (0.0-1.0)
- `shininess`: 光泽度 (通常 2-200)
- `pattern_type`: 图案类型 (0=纯色)
- `pattern_color`: 图案颜色 (r,g,b)
- `pattern_scale`: 图案缩放 (通常 1.0)
- `bump`: Bump mapping 开关 (0=关闭, 1=开启)

### 4. 测试场景说明

#### test_bump.rt - 对比测试
- 左侧：无bump的光滑球体（参考）
- 右侧：有bump的波纹球体（测试）
- **验证点**：右侧球体应该显示sin/cos波纹，左侧完全光滑

运行命令：
```bash
./miniRT maps/test_bump.rt
```

#### test_bump_plane.rt - 平面测试
- 大型平面，最容易看清bump效果
- **验证点**：应该看到规律的波纹图案

运行命令：
```bash
./miniRT maps/test_bump_plane.rt
```

#### test_bump_gallery.rt - 画廊对比
- 上排：4个光滑球体
- 下排：4个带bump的球体
- **验证点**：下排所有球体都应该有纹理，上排完全光滑

运行命令：
```bash
./miniRT maps/test_bump_gallery.rt
```

### 5. 详细验证步骤

#### Step 1: 编译并运行
```bash
make
./miniRT maps/test_bump.rt
```

#### Step 2: 观察对比
比较左右两个球体：
- 左球：镜面高光应该是一个圆形光斑
- 右球：高光应该被"打碎"成波纹状

#### Step 3: 检查数学函数
当前bump函数：`10 * sin(u) * cos(v)`
- 应该产生网格状的凹凸
- 频率由 u, v 的系数控制
- 振幅由前面的 10 控制

#### Step 4: 调整参数测试
在 bump_mapping.c 中修改 bump_func：
```c
// 测试1: 增加频率
result = 10 * sin(20 * u) * cos(20 * v);  // 更密集的纹理

// 测试2: 不同图案
result = 5 * sin(u * u + v * v);  // 同心圆图案

// 测试3: 更强的效果
result = 20 * sin(u) * cos(v);  // 更明显的凹凸
```

### 6. 常见问题排查

#### 问题1: 看不到任何bump效果
检查点：
- [ ] 材质中 `b:1` 是否正确解析？
- [ ] `hit->material->bump` 是否被正确设置？
- [ ] `apply_bump()` 是否在 renderer 中被调用？
- [ ] 法线是否被归一化？

#### 问题2: bump效果太弱
解决方法：
- 增加 bump_func 中的振幅系数（10 → 20 或更大）
- 调整 B_EPSILON 的值
- 检查光照设置（需要足够强的光源）

#### 问题3: bump效果太强或失真
解决方法：
- 减小 bump_func 中的振幅系数
- 确保法线被正确归一化
- 检查切线空间是否正确计算

#### 问题4: 纹理方向错误
检查点：
- compute_tangent_space() 中的切线和副切线计算
- UV坐标映射是否正确

### 7. 高级验证

#### 创建自定义测试
修改 bump_func 来测试不同效果：

```c
// 棋盘格图案
double bump_func(double u, double v)
{
    double freq = 5.0;
    int checkerU = (int)(u * freq);
    int checkerV = (int)(v * freq);
    if ((checkerU + checkerV) % 2 == 0)
        return 2.0;
    return -2.0;
}

// 条纹图案
double bump_func(double u, double v)
{
    return 8.0 * sin(20.0 * u);
}

// 涟漪图案
double bump_func(double u, double v)
{
    double dist = sqrt(u * u + v * v);
    return 5.0 * sin(20.0 * dist);
}
```

### 8. 与参考对比

如果你有 reference 渲染器 (babonus)：
```bash
# 用你的渲染器
./miniRT maps/test_bump.rt

# 用参考渲染器
./babonus maps/test_bump.rt

# 对比两个输出图像
```

### 9. 预期效果描述

**正确的 bump mapping 应该：**
1. 球体表面有波浪状的明暗交替
2. 高光不再是完美的圆形，而是跟随波纹扭曲
3. 转动视角时，纹理看起来有"深度"
4. 不同的光照角度会强调不同的"凹凸"

**球体上的具体表现：**
- 赤道附近：横向波纹
- 两极附近：可能有扭曲（由于球面UV映射）
- 高光区域：高光被"打碎"成多个小亮点

### 10. Debug输出建议

在 apply_bump() 中添加调试输出：
```c
// 打印第一个像素的信息
static int debug_count = 0;
if (debug_count++ == 0)
{
    printf("Bump Debug:\n");
    printf("  UV: (%.3f, %.3f)\n", bump.u, bump.v);
    printf("  h0: %.3f, hu: %.3f, hv: %.3f\n", h0, hu, hv);
    printf("  Original normal: (%.3f, %.3f, %.3f)\n", 
           hit->normal.x, hit->normal.y, hit->normal.z);
    printf("  Perturbed normal: (%.3f, %.3f, %.3f)\n",
           perturbed_n.x, perturbed_n.y, perturbed_n.z);
}
```

### 11. 成功标准

✅ Bump mapping 实现正确的标志：
1. 启用和不启用 bump 的物体有明显视觉差异
2. 表面有细节但轮廓不变
3. 光照响应看起来自然（不是纯色贴图）
4. 从不同角度观察效果一致
5. 与参考实现（如果有）视觉效果相似

---

**快速测试命令：**
```bash
make && ./miniRT maps/test_bump.rt
make && ./miniRT maps/test_bump_plane.rt
make && ./miniRT maps/test_bump_gallery.rt
```
