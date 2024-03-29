//
//  VectUtils.c
//  SphericalHarmonics
//
//  Created by asd on 03/10/2018.
//  Copyright © 2018 voicesync. All rights reserved.
//

#include "VectUtils.h"

Texture texture(float t, float u) {
    Texture _tx={t,u};
    return _tx;
}

inline void normalise(XYZ *p) {
    *p = simd_normalize(*p);
}

XYZ calcNormals(XYZ p, XYZ p1, XYZ p2) {
    return simd_normalize(simd_cross(p1-p2, p1-p));
}

int*triangularize(int nSides) { // generate nSides polygon set of trig index coords
    int *res=calloc((nSides-2)*3, sizeof(int));
    for (int i=1; i<nSides-1; i++) {// 0, i, i+1 : i=1..ns-1, for quad=4: 0 1 2 0 2 3
        int vi[]={0, i, i+1};
        for (int j=0; j<3; j++) res[(i-1)*3+j]=vi[j];
    }
    return res;
}

Color calcColor(float v, float vmin, float vmax, int type) {
    float dv, vmid;
    Color c = {1.0, 1.0, 1.0};
    Color c1, c2, c3;
    float ratio;
    
    if (vmax < vmin) {
        dv = vmin;
        vmin = vmax;
        vmax = dv;
    }
    if (vmax - vmin < 0.000001) {
        vmin -= 1;
        vmax += 1;
    }
    if (v < vmin)
        v = vmin;
    if (v > vmax)
        v = vmax;
    dv = vmax - vmin;
    
    switch (type) {
        case 1:
            if (v < (vmin + 0.25 * dv)) {
                c.r = 0;
                c.g = 4 * (v - vmin) / dv;
                c.b = 1;
            }
            else if (v < (vmin + 0.5 * dv)) {
                c.r = 0;
                c.g = 1;
                c.b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
            }
            else if (v < (vmin + 0.75 * dv)) {
                c.r = 4 * (v - vmin - 0.5 * dv) / dv;
                c.g = 1;
                c.b = 0;
            }
            else {
                c.r = 1;
                c.g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
                c.b = 0;
            }
            break;
        case 2:
            c.r = (v - vmin) / dv;
            c.g = 0;
            c.b = (vmax - v) / dv;
            break;
        case 3:
            c.r = (v - vmin) / dv;
            c.b = c.r;
            c.g = c.r;
            break;
        case 4:
            if (v < (vmin + dv / 6.0)) {
                c.r = 1;
                c.g = 6 * (v - vmin) / dv;
                c.b = 0;
            }
            else if (v < (vmin + 2.0 * dv / 6.0)) {
                c.r = 1 + 6 * (vmin + dv / 6.0 - v) / dv;
                c.g = 1;
                c.b = 0;
            }
            else if (v < (vmin + 3.0 * dv / 6.0)) {
                c.r = 0;
                c.g = 1;
                c.b = 6 * (v - vmin - 2.0 * dv / 6.0) / dv;
            }
            else if (v < (vmin + 4.0 * dv / 6.0)) {
                c.r = 0;
                c.g = 1 + 6 * (vmin + 3.0 * dv / 6.0 - v) / dv;
                c.b = 1;
            }
            else if (v < (vmin + 5.0 * dv / 6.0)) {
                c.r = 6 * (v - vmin - 4.0 * dv / 6.0) / dv;
                c.g = 0;
                c.b = 1;
            }
            else {
                c.r = 1;
                c.g = 0;
                c.b = 1 + 6 * (vmin + 5.0 * dv / 6.0 - v) / dv;
            }
            break;
        case 5:
            c.r = (v - vmin) / (vmax - vmin);
            c.g = 1;
            c.b = 0;
            break;
        case 6:
            c.r = (v - vmin) / (vmax - vmin);
            c.g = (vmax - v) / (vmax - vmin);
            c.b = c.r;
            break;
        case 7:
            if (v < (vmin + 0.25 * dv)) {
                c.r = 0;
                c.g = 4 * (v - vmin) / dv;
                c.b = 1 - c.g;
            }
            else if (v < (vmin + 0.5 * dv)) {
                c.r = 4 * (v - vmin - 0.25 * dv) / dv;
                c.g = 1 - c.r;
                c.b = 0;
            }
            else if (v < (vmin + 0.75 * dv)) {
                c.g = 4 * (v - vmin - 0.5 * dv) / dv;
                c.r = 1 - c.g;
                c.b = 0;
            }
            else {
                c.r = 0;
                c.b = 4 * (v - vmin - 0.75 * dv) / dv;
                c.g = 1 - c.b;
            }
            break;
        case 8:
            if (v < (vmin + 0.5 * dv)) {
                c.r = 2 * (v - vmin) / dv;
                c.g = c.r;
                c.b = c.r;
            }
            else {
                c.r = 1 - 2 * (v - vmin - 0.5 * dv) / dv;
                c.g = c.r;
                c.b = c.r;
            }
            break;
        case 9:
            if (v < (vmin + dv / 3)) {
                c.b = 3 * (v - vmin) / dv;
                c.g = 0;
                c.r = 1 - c.b;
            }
            else if (v < (vmin + 2 * dv / 3)) {
                c.r = 0;
                c.g = 3 * (v - vmin - dv / 3) / dv;
                c.b = 1;
            }
            else {
                c.r = 3 * (v - vmin - 2 * dv / 3) / dv;
                c.g = 1 - c.r;
                c.b = 1;
            }
            break;
        case 10:
            if (v < (vmin + 0.2 * dv)) {
                c.r = 0;
                c.g = 5 * (v - vmin) / dv;
                c.b = 1;
            }
            else if (v < (vmin + 0.4 * dv)) {
                c.r = 0;
                c.g = 1;
                c.b = 1 + 5 * (vmin + 0.2 * dv - v) / dv;
            }
            else if (v < (vmin + 0.6 * dv)) {
                c.r = 5 * (v - vmin - 0.4 * dv) / dv;
                c.g = 1;
                c.b = 0;
            }
            else if (v < (vmin + 0.8 * dv)) {
                c.r = 1;
                c.g = 1 - 5 * (v - vmin - 0.6 * dv) / dv;
                c.b = 0;
            }
            else {
                c.r = 1;
                c.g = 5 * (v - vmin - 0.8 * dv) / dv;
                c.b = 5 * (v - vmin - 0.8 * dv) / dv;
            }
            break;
        case 11:
            c1.r = 200 / 255.0;
            c1.g = 60 / 255.0;
            c1.b = 0 / 255.0;
            c2.r = 250 / 255.0;
            c2.g = 160 / 255.0;
            c2.b = 110 / 255.0;
            c.r = (c2.r - c1.r) * (v - vmin) / dv + c1.r;
            c.g = (c2.g - c1.g) * (v - vmin) / dv + c1.g;
            c.b = (c2.b - c1.b) * (v - vmin) / dv + c1.b;
            break;
        case 12:
            c1.r = 55 / 255.0;
            c1.g = 55 / 255.0;
            c1.b = 45 / 255.0;
            /* c2.r = 200 / 255.0; c2.g =  60 / 255.0; c2.b =   0 / 255.0; */
            c2.r = 235 / 255.0;
            c2.g = 90 / 255.0;
            c2.b = 30 / 255.0;
            c3.r = 250 / 255.0;
            c3.g = 160 / 255.0;
            c3.b = 110 / 255.0;
            ratio = 0.4;
            vmid = vmin + ratio * dv;
            if (v < vmid) {
                c.r = (c2.r - c1.r) * (v - vmin) / (ratio * dv) + c1.r;
                c.g = (c2.g - c1.g) * (v - vmin) / (ratio * dv) + c1.g;
                c.b = (c2.b - c1.b) * (v - vmin) / (ratio * dv) + c1.b;
            }
            else {
                c.r = (c3.r - c2.r) * (v - vmid) / ((1 - ratio) * dv) + c2.r;
                c.g = (c3.g - c2.g) * (v - vmid) / ((1 - ratio) * dv) + c2.g;
                c.b = (c3.b - c2.b) * (v - vmid) / ((1 - ratio) * dv) + c2.b;
            }
            break;
        case 13:
            c1.r = 0 / 255.0;
            c1.g = 255 / 255.0;
            c1.b = 0 / 255.0;
            c2.r = 255 / 255.0;
            c2.g = 150 / 255.0;
            c2.b = 0 / 255.0;
            c3.r = 255 / 255.0;
            c3.g = 250 / 255.0;
            c3.b = 240 / 255.0;
            ratio = 0.3;
            vmid = vmin + ratio * dv;
            if (v < vmid) {
                c.r = (c2.r - c1.r) * (v - vmin) / (ratio * dv) + c1.r;
                c.g = (c2.g - c1.g) * (v - vmin) / (ratio * dv) + c1.g;
                c.b = (c2.b - c1.b) * (v - vmin) / (ratio * dv) + c1.b;
            }
            else {
                c.r = (c3.r - c2.r) * (v - vmid) / ((1 - ratio) * dv) + c2.r;
                c.g = (c3.g - c2.g) * (v - vmid) / ((1 - ratio) * dv) + c2.g;
                c.b = (c3.b - c2.b) * (v - vmid) / ((1 - ratio) * dv) + c2.b;
            }
            break;
        case 14:
            c.r = 1;
            c.g = 1 - (v - vmin) / dv;
            c.b = 0;
            break;
        case 15:
            if (v < (vmin + 0.25 * dv)) {
                c.r = 0;
                c.g = 4 * (v - vmin) / dv;
                c.b = 1;
            }
            else if (v < (vmin + 0.5 * dv)) {
                c.r = 0;
                c.g = 1;
                c.b = 1 - 4 * (v - vmin - 0.25 * dv) / dv;
            }
            else if (v < (vmin + 0.75 * dv)) {
                c.r = 4 * (v - vmin - 0.5 * dv) / dv;
                c.g = 1;
                c.b = 0;
            }
            else {
                c.r = 1;
                c.g = 1;
                c.b = 4 * (v - vmin - 0.75 * dv) / dv;
            }
            break;
        case 16:
            if (v < (vmin + 0.5 * dv)) {
                c.r = 0.0;
                c.g = 2 * (v - vmin) / dv;
                c.b = 1 - 2 * (v - vmin) / dv;
            }
            else {
                c.r = 2 * (v - vmin - 0.5 * dv) / dv;
                c.g = 1 - 2 * (v - vmin - 0.5 * dv) / dv;
                c.b = 0.0;
            }
            break;
        case 17:
            if (v < (vmin + 0.5 * dv)) {
                c.r = 1.0;
                c.g = 1 - 2 * (v - vmin) / dv;
                c.b = 2 * (v - vmin) / dv;
            }
            else {
                c.r = 1 - 2 * (v - vmin - 0.5 * dv) / dv;
                c.g = 2 * (v - vmin - 0.5 * dv) / dv;
                c.b = 1.0;
            }
            break;
        case 18:
            c.r = 0;
            c.g = (v - vmin) / (vmax - vmin);
            c.b = 1;
            break;
        case 19:
            c.r = (v - vmin) / (vmax - vmin);
            c.g = c.r;
            c.b = 1;
            break;
        case 20:
            c1.r = 0 / 255.0;
            c1.g = 160 / 255.0;
            c1.b = 0 / 255.0;
            c2.r = 180 / 255.0;
            c2.g = 220 / 255.0;
            c2.b = 0 / 255.0;
            c3.r = 250 / 255.0;
            c3.g = 220 / 255.0;
            c3.b = 170 / 255.0;
            ratio = 0.3;
            vmid = vmin + ratio * dv;
            if (v < vmid) {
                c.r = (c2.r - c1.r) * (v - vmin) / (ratio * dv) + c1.r;
                c.g = (c2.g - c1.g) * (v - vmin) / (ratio * dv) + c1.g;
                c.b = (c2.b - c1.b) * (v - vmin) / (ratio * dv) + c1.b;
            }
            else {
                c.r = (c3.r - c2.r) * (v - vmid) / ((1 - ratio) * dv) + c2.r;
                c.g = (c3.g - c2.g) * (v - vmid) / ((1 - ratio) * dv) + c2.g;
                c.b = (c3.b - c2.b) * (v - vmid) / ((1 - ratio) * dv) + c2.b;
            }
            break;
        case 21:
            c1.r = 255 / 255.0;
            c1.g = 255 / 255.0;
            c1.b = 200 / 255.0;
            c2.r = 150 / 255.0;
            c2.g = 150 / 255.0;
            c2.b = 255 / 255.0;
            c.r = (c2.r - c1.r) * (v - vmin) / dv + c1.r;
            c.g = (c2.g - c1.g) * (v - vmin) / dv + c1.g;
            c.b = (c2.b - c1.b) * (v - vmin) / dv + c1.b;
            break;
        case 22:
            c.r = 1 - (v - vmin) / dv;
            c.g = 1 - (v - vmin) / dv;
            c.b = (v - vmin) / dv;
            break;
        case 23:
            if (v < (vmin + 0.5 * dv)) {
                c.r = 1;
                c.g = 2 * (v - vmin) / dv;
                c.b = c.g;
            }
            else {
                c.r = 1 - 2 * (v - vmin - 0.5 * dv) / dv;
                c.g = c.r;
                c.b = 1;
            }
            break;
        case 24:
            if (v < (vmin + 0.5 * dv)) {
                c.r = 2 * (v - vmin) / dv;
                c.g = c.r;
                c.b = 1 - c.r;
            }
            else {
                c.r = 1;
                c.g = 1 - 2 * (v - vmin - 0.5 * dv) / dv;
                c.b = 0;
            }
            break;
        case 25:
            if (v < (vmin + dv / 3)) {
                c.r = 0;
                c.g = 3 * (v - vmin) / dv;
                c.b = 1;
            }
            else if (v < (vmin + 2 * dv / 3)) {
                c.r = 3 * (v - vmin - dv / 3) / dv;
                c.g = 1 - c.r;
                c.b = 1;
            }
            else {
                c.r = 1;
                c.g = 0;
                c.b = 1 - 3 * (v - vmin - 2 * dv / 3) / dv;
            }
            break;
    }
    return (c);
}

