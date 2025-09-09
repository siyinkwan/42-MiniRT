# Material Options Documentation for parse_material_options()

## Format Overview
Material options appear after the required object parameters as positional arguments:
```
<object_type> <required_params> <base_color> [ambient] [diffuse] [specular] [shininess] [pattern_type] [pattern_color2] [pattern_scale] [bump]
```

## Parameter Order (All Optional)
1. `ambient` - Ambient reflection coefficient [0.0-1.0] (default: 0.1)
2. `diffuse` - Diffuse reflection coefficient [0.0-1.0] (default: 0.9)  
3. `specular` - Specular reflection coefficient [0.0-1.0] (default: 0.9)
4. `shininess` - Specular exponent [1-1000] (default: 200)
5. `pattern_type` - Pattern type: 0=solid, 1=checkerboard, 2=stripes, 3=gradient (default: 0)
6. `pattern_color2` - Second color for patterns as r,g,b (default: 255,255,255)
7. `pattern_scale` - Pattern scale factor [0.1-10.0] (default: 1.0)
8. `bump` - Bump mapping: 0=false, 1=true (default: 0)

## Parsing Rules
1. All material options are positional and optional
2. Parameters must appear in the specified order
3. You can specify any number of parameters from 0 to 8
4. Missing parameters use default values
5. Color values use comma-separated RGB: `r,g,b`
6. Boolean values: `0` (false) or `1` (true)
7. Numeric values: decimal numbers (e.g., 0.5, 200, 1.0)

## Examples by Complexity

### Basic Objects (No Material Options)
```
sp 0,0,20 12.6 255,0,0                    # Red sphere with all defaults
pl 0,0,0 0,1,0 128,128,128                # Gray plane with all defaults
```

### Partial Material Specification
```
sp 0,0,20 12.6 255,0,0 0.5                # Custom ambient only
sp 0,0,20 12.6 255,0,0 0.3 0.8            # Custom ambient and diffuse
sp 0,0,20 12.6 255,0,0 0.2 0.6 0.9 300    # Full Phong lighting parameters
```

### With Patterns
```
sp 0,0,20 12.6 255,0,0 0.1 0.9 0.9 200 1                    # Checkerboard with defaults
sp 0,0,20 12.6 255,0,0 0.1 0.9 0.9 200 1 0,255,0 2.0        # Checkerboard with custom color/scale
sp 0,0,20 12.6 255,0,0 0.1 0.9 0.9 200 2 255,255,0 1.5      # Stripes pattern
```

### Complete Specification
```
cy 0,0,20 0,1,0 14.2 21.42 255,128,0 0.2 0.6 0.8 120 2 0,255,255 1.5 1
# All parameters: ambient=0.2, diffuse=0.6, specular=0.8, shininess=120, 
# stripes pattern with cyan secondary color, scale=1.5, bump enabled

co 0,0,10 0,1,0 30.0 15.0 255,100,100 0.25 0.7 0.9 200 1 100,255,100 1.0 1
# Cone: apex=(0,0,10), axis=(0,1,0), angle=30°, height=15.0, color=red
# Material: ambient=0.25, diffuse=0.7, specular=0.9, shininess=200
# Pattern: checkerboard with green secondary color, scale=1.0, bump enabled
```

## Cone Format (BONUS)
```
co <apex_x,apex_y,apex_z> <axis_x,axis_y,axis_z> <angle_degrees> <height> <r,g,b> [material_options...]
```

**Parameters:**
- `apex`: Position of cone tip (x,y,z)
- `axis`: Normalized direction from tip toward base (x,y,z) 
- `angle`: Half-angle in degrees [0.1-89.9] (determines cone width)
- `height`: Distance from apex to base [> 0.0]
- `color`: Base color (r,g,b)
- `material_options`: Same as other objects

## Implementation Hints for parse_material_options()

1. **Function Signature**: 
   ```c
   bool parse_material_options(t_material *material, char **tokens, int start_index)
   ```
   
2. **Token Processing**:
   ```c
   int i = start_index; // Usually 4 (after object type, position, size, color)
   
   // Parse in order: ambient, diffuse, specular, shininess, pattern_type, pattern_color2, pattern_scale, bump
   if (tokens[i] != NULL) material->ambient = ft_atof(tokens[i++]);
   if (tokens[i] != NULL) material->diffuse = ft_atof(tokens[i++]);
   if (tokens[i] != NULL) material->specular = ft_atof(tokens[i++]);
   if (tokens[i] != NULL) material->shininess = ft_atof(tokens[i++]);
   if (tokens[i] != NULL) material->pattern_type = (t_pattern_type)atoi(tokens[i++]);
   if (tokens[i] != NULL) parse_color(tokens[i], &material->pattern_color2), i++;
   if (tokens[i] != NULL) material->pattern_scale = ft_atof(tokens[i++]);
   if (tokens[i] != NULL) material->bump = (atoi(tokens[i]) != 0);
   ```

3. **Default Initialization**: Always initialize material with defaults first:
   ```c
   material->ambient = 0.1;
   material->diffuse = 0.9;
   material->specular = 0.9;
   material->shininess = 200.0;
   material->pattern_type = PATTERN_SOLID;
   material->pattern_color2 = (t_vec3){255, 255, 255};
   material->pattern_scale = 1.0;
   material->bump = false;
   ```

4. **Error Handling**:
   - Validate ranges for each parameter
   - Handle malformed color values
   - Check for invalid pattern types

## Testing Sequence
1. Start with `simple_test.rt` - basic positional parsing
2. Test `basic_materials.rt` - Phong lighting parameters in order
3. Test `pattern_materials.rt` - Pattern functionality
4. Test `advanced_materials.rt` - Complex combinations
5. Test `edge_cases.rt` - Boundary conditions and partial specifications
