//
//  VectUtils.h
//  SphericalHarmonics
//
//  Created by asd on 03/10/2018.
//  Copyright © 2018 voicesync. All rights reserved.
//

#ifndef VectUtils_h
#define VectUtils_h

#include <math.h>
#include <simd/simd.h>


typedef float Vect8f[8];

typedef simd_float3 XYZ, Color;
typedef simd_float2 Texture;

inline void normalise(XYZ *p);
XYZ     calcNormals(XYZ p, XYZ p1, XYZ p2);
Color   calcColor(float v, float vmin, float vmax, int type);
Texture texture(float t, float u);
int     *triangularize(int nSides);

#endif /* VectUtils_h */
