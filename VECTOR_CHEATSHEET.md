# Vector Math Cheat Sheet

Quick reference for 3D vector operations used in ray tracing.

---

## 1. Vector Basics

### 1.1 Definition
A 3D vector represents a direction and magnitude:
$$\vec{v} = (x, y, z) = x\hat{i} + y\hat{j} + z\hat{k}$$

**In code:**
```c
typedef struct s_vec3 {
    double x;
    double y;
    double z;
} t_vec3;
```

### 1.2 Common Vectors
- **Zero vector:** $\vec{0} = (0, 0, 0)$
- **Unit vectors:** $\hat{i} = (1,0,0)$, $\hat{j} = (0,1,0)$, $\hat{k} = (0,0,1)$
- **World up:** typically $(0, 1, 0)$ (positive Y-axis)

---

## 2. Basic Operations

### 2.1 Addition
$$\vec{a} + \vec{b} = (a_x + b_x, \quad a_y + b_y, \quad a_z + b_z)$$

**Geometric meaning:** Head-to-tail, resultant vector

**Code:**
```c
t_vec3 vec3_add(t_vec3 a, t_vec3 b)
{
    return (vec3_create(a.x + b.x, a.y + b.y, a.z + b.z));
}
```

### 2.2 Subtraction
$$\vec{a} - \vec{b} = (a_x - b_x, \quad a_y - b_y, \quad a_z - b_z)$$

**Geometric meaning:** Vector from $\vec{b}$ to $\vec{a}$

**Use case:** Direction from point A to point B
```c
direction = vec3_subtract(B, A);  // A → B
```

### 2.3 Scalar Multiplication
$$k\vec{v} = (kv_x, \quad kv_y, \quad kv_z)$$

**Effects:**
- $k > 1$: Stretch (longer)
- $0 < k < 1$: Shrink (shorter)
- $k < 0$: Reverse direction
- $k = 0$: Zero vector

**Code:**
```c
t_vec3 vec3_scale(t_vec3 v, double k)
{
    return (vec3_create(v.x * k, v.y * k, v.z * k));
}
```

### 2.4 Negation
$$-\vec{v} = (-v_x, -v_y, -v_z)$$

**Geometric meaning:** Opposite direction, same magnitude

---

## 3. Vector Properties

### 3.1 Magnitude (Length)
$$|\vec{v}| = \sqrt{v_x^2 + v_y^2 + v_z^2}$$

**Code:**
```c
double vec3_length(t_vec3 v)
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}
```

**Special case:** Squared length (faster, no sqrt)
$$|\vec{v}|^2 = v_x^2 + v_y^2 + v_z^2$$

### 3.2 Normalization (Unit Vector)
$$\hat{v} = \frac{\vec{v}}{|\vec{v}|} = \frac{(v_x, v_y, v_z)}{|\vec{v}|}$$

**Result:** Vector with same direction but length = 1

**Properties:**
- $|\hat{v}| = 1$
- Direction unchanged
- **Never normalize zero vector!** (division by zero)

**Code:**
```c
t_vec3 vec3_normalize(t_vec3 v)
{
    double len = vec3_length(v);
    if (len < EPSILON)
        return (vec3_create(0, 0, 0));  // Handle edge case
    return (vec3_scale(v, 1.0 / len));
}
```

---

## 4. Dot Product (Scalar Product)

### 4.1 Definition
$$\vec{a} \cdot \vec{b} = a_x b_x + a_y b_y + a_z b_z$$

**Alternative form:**
$$\vec{a} \cdot \vec{b} = |\vec{a}| |\vec{b}| \cos(\theta)$$

Where $\theta$ = angle between vectors

**Code:**
```c
double vec3_dot(t_vec3 a, t_vec3 b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}
```

### 4.2 Geometric Interpretation

**Result:** Scalar (single number, not a vector!)

**Meaning:**
- **How aligned are the vectors?**
- Projection of $\vec{a}$ onto $\vec{b}$ (multiplied by $|\vec{b}|$)

### 4.3 Key Properties

| Condition | $\vec{a} \cdot \vec{b}$ | $\theta$ | Meaning |
|-----------|-------------------------|----------|---------|
| Same direction | $> 0$ | $0° \leq \theta < 90°$ | Acute angle |
| Perpendicular | $= 0$ | $\theta = 90°$ | Orthogonal |
| Opposite direction | $< 0$ | $90° < \theta \leq 180°$ | Obtuse angle |

**Special cases (unit vectors):**
- $\hat{a} \cdot \hat{b} = 1$ → Same direction ($\theta = 0°$)
- $\hat{a} \cdot \hat{b} = 0$ → Perpendicular ($\theta = 90°$)
- $\hat{a} \cdot \hat{b} = -1$ → Opposite direction ($\theta = 180°$)

### 4.4 Common Uses

**1. Finding angle:**
$$\cos(\theta) = \frac{\vec{a} \cdot \vec{b}}{|\vec{a}| |\vec{b}|}$$

If normalized: $\cos(\theta) = \hat{a} \cdot \hat{b}$

**2. Projection:**
Length of $\vec{a}$ projected onto $\hat{b}$:
$$\text{proj}_{\hat{b}} \vec{a} = \vec{a} \cdot \hat{b}$$

**3. Component extraction:**
Get component along axis:
```c
double y_component = vec3_dot(vector, vec3_create(0, 1, 0));
```

**4. Perpendicularity test:**
```c
if (fabs(vec3_dot(a, b)) < EPSILON)
    // Vectors are perpendicular
```

**5. Lambert's Law (diffuse lighting):**
$$\text{brightness} = \max(0, N \cdot L)$$

---

## 5. Cross Product (Vector Product)

### 5.1 Definition
$$\vec{a} \times \vec{b} = \begin{vmatrix} \hat{i} & \hat{j} & \hat{k} \\ a_x & a_y & a_z \\ b_x & b_y & b_z \end{vmatrix}$$

$$= (a_y b_z - a_z b_y, \quad a_z b_x - a_x b_z, \quad a_x b_y - a_y b_x)$$

**Code:**
```c
t_vec3 vec3_cross(t_vec3 a, t_vec3 b)
{
    return (vec3_create(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    ));
}
```

### 5.2 Geometric Interpretation

**Result:** Vector (not a scalar!)

**Direction:** Perpendicular to BOTH $\vec{a}$ and $\vec{b}$

**Magnitude:**
$$|\vec{a} \times \vec{b}| = |\vec{a}| |\vec{b}| \sin(\theta)$$

**Geometric meaning:** Area of parallelogram formed by $\vec{a}$ and $\vec{b}$

### 5.3 Key Properties

**1. Anti-commutative:**
$$\vec{a} \times \vec{b} = -(\vec{b} \times \vec{a})$$
⚠️ **Order matters!** Reverses direction

**2. Right-hand rule:**
- Point fingers along $\vec{a}$
- Curl towards $\vec{b}$
- Thumb points in direction of $\vec{a} \times \vec{b}$

**3. Parallel vectors:**
$$\vec{a} \times \vec{b} = \vec{0} \iff \vec{a} \parallel \vec{b}$$

**4. Perpendicularity:**
$$(\vec{a} \times \vec{b}) \cdot \vec{a} = 0$$
$$(\vec{a} \times \vec{b}) \cdot \vec{b} = 0$$

### 5.4 Common Uses

**1. Build orthonormal basis:**
```c
// Given forward vector, construct right and up
t_vec3 forward = normalize(direction);
t_vec3 right = normalize(cross(world_up, forward));
t_vec3 up = cross(forward, right);  // Already normalized
```

**2. Surface normal from two edges:**
```c
t_vec3 edge1 = vec3_subtract(B, A);
t_vec3 edge2 = vec3_subtract(C, A);
t_vec3 normal = normalize(cross(edge1, edge2));
```

**3. Perpendicular vector:**
Get ANY vector perpendicular to $\vec{v}$:
```c
t_vec3 perp = cross(v, vec3_create(1, 0, 0));
// If v is parallel to (1,0,0), use different axis
if (vec3_length(perp) < EPSILON)
    perp = cross(v, vec3_create(0, 1, 0));
```

**4. Plane test:**
Which side of plane formed by $\vec{a}, \vec{b}$?
```c
t_vec3 n = cross(a, b);
double side = dot(point, n);  // positive/negative sides
```

---

## 6. Advanced Operations

### 6.1 Reflection
Reflect vector $\vec{I}$ across normal $\vec{N}$:

$$\vec{R} = \vec{I} - 2(\vec{I} \cdot \vec{N})\vec{N}$$

**Assumes:** $\vec{N}$ is normalized

**Visual:**
```
    ^N
    |
    |  
I ↙   ↗ R
————————— surface
```

**Code:**
```c
t_vec3 reflect(t_vec3 I, t_vec3 N)
{
    return (vec3_subtract(I, vec3_scale(N, 2.0 * vec3_dot(I, N))));
}
```

### 6.2 Linear Interpolation (Lerp)
$$\text{lerp}(\vec{a}, \vec{b}, t) = (1-t)\vec{a} + t\vec{b} = \vec{a} + t(\vec{b} - \vec{a})$$

Where $t \in [0, 1]$:
- $t = 0$: result = $\vec{a}$
- $t = 0.5$: result = midpoint
- $t = 1$: result = $\vec{b}$

### 6.3 Distance Between Points
$$d = |\vec{B} - \vec{A}| = \sqrt{(B_x - A_x)^2 + (B_y - A_y)^2 + (B_z - A_z)^2}$$

**Code:**
```c
double distance = vec3_length(vec3_subtract(B, A));
```

### 6.4 Component-wise Multiplication
$$\vec{a} \odot \vec{b} = (a_x b_x, \quad a_y b_y, \quad a_z b_z)$$

**Use:** Color blending, texture mapping

**Code:**
```c
t_vec3 vec3_multiply(t_vec3 a, t_vec3 b)
{
    return (vec3_create(a.x * b.x, a.y * b.y, a.z * b.z));
}
```

---

## 7. Quick Reference Table

| Operation | Symbol | Returns | Use Case |
|-----------|--------|---------|----------|
| Addition | $\vec{a} + \vec{b}$ | Vector | Combine vectors |
| Subtraction | $\vec{a} - \vec{b}$ | Vector | Direction A→B |
| Scale | $k\vec{v}$ | Vector | Resize vector |
| Length | $\|\vec{v}\|$ | Scalar | Distance, magnitude |
| Normalize | $\hat{v}$ | Vector | Unit direction |
| Dot product | $\vec{a} \cdot \vec{b}$ | Scalar | Angle, projection |
| Cross product | $\vec{a} \times \vec{b}$ | Vector | Perpendicular, normal |
| Reflection | $\vec{R}$ | Vector | Mirror bounce |

---

## 8. Ray Tracing Specific

### 8.1 Ray at Distance t
$$P(t) = O + tD$$

### 8.2 Check Ray-Sphere Intersection
$$\vec{oc} = O - C$$
$$\text{discriminant} = (\vec{oc} \cdot D)^2 - (|\vec{oc}|^2 - r^2)$$

If discriminant $\geq 0$ → Intersection exists

### 8.3 Surface Normal (Sphere)
$$N = \frac{P_{hit} - C_{sphere}}{r}$$

### 8.4 Lambert Shading
$$\text{intensity} = \max(0, N \cdot L)$$

Where:
- $N$ = surface normal
- $L$ = light direction (to light)

### 8.5 Specular Highlight
$$\text{intensity} = (\max(0, R \cdot V))^{shininess}$$

Where:
- $R$ = reflected light direction
- $V$ = view direction (to camera)

---

**Common values to expect:**
- Normalized vectors: length ≈ 1.0
- Perpendicular vectors: dot ≈ 0.0
- Opposite vectors: dot ≈ -1.0
- Same vectors: dot ≈ 1.0

---

## Remember

💡 **Dot product** → Scalar → "How aligned?"  
💡 **Cross product** → Vector → "Perpendicular direction"  
💡 **Normalize** → Unit vector → "Direction only, length=1"  
💡 **Subtract** → Vector → "Direction from A to B"  

**EPSILON = 1e-6** for all floating-point comparisons!
