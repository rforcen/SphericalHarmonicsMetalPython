//
//  SphericalHarmonics.h
//  SphericalHarmonics
//
//  Created by asd on 03/10/2018.
//  Copyright © 2018 voicesync. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VectUtils.h"
#import "SHCodes.h"
#import "MetalDevice.h"

#define TWOPI           M_PI*2.


typedef struct {
    XYZ coords[4],  normals[4];
    Texture textures[4];
    Color colors[4];
} Quad;

NS_ASSUME_NONNULL_BEGIN

@interface SphericalHarmonics : NSObject {
    Vect8f m; // 'm' mgr the code array
    
    int resolution;
    int colourmap; // 1..25 types of colouring 7=rgb

@public
    char code[8+1]; // the parameter code
    int nCoords;
    XYZ *coords,  *normals;
    Texture *textures;
    Color *colors;
    
    MetalDevice*metalDevice;
    Quad *quads;
    uint res2;
}

+(id)init :(int)res colorMap:(int)colorMap code:(int)code;
-(void)readCode: (int)index;
-(void)randomCode;
-(void)nextCode;
-(void)setColorMap:(int)colMap;
-(void)genCoords;
-(NSString*)getCode:(int) ix;

+(void) randomize;
+(float) rnd :(float)range;
+(float) rndInt :(int)range;

@end


NS_ASSUME_NONNULL_END
