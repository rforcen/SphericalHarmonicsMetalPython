//
//  SphericalHarmonics.m
//  SphericalHarmonics
//
//  Created by asd on 03/10/2018.
//  Copyright © 2018 voicesync. All rights reserved.
//

#import "SphericalHarmonics.h"




@implementation SphericalHarmonics {
    id<MTLBuffer>quadBuff;
}

+(void) randomize { srand((unsigned)time(NULL)); }
+(float) rnd :(float)range {  return (float)(range * rand())/RAND_MAX; }
+(float) rndInt :(int)range {  return rand() % range; }


+(id)init :(int)res colorMap:(int)colorMap code:(int)code {
    SphericalHarmonics*sh=[[SphericalHarmonics alloc] init];
    
    sortCodes();
    
    sh->colourmap = colorMap; // 7;
    sh->resolution = res; // 256
    
    sh->res2 = res*res;
    
    // alloc results in quads -> x 4
    sh->nCoords = sh->res2 * 4;
    sh->coords = calloc(sh->nCoords, sizeof(*sh->coords));
    sh->normals= calloc(sh->nCoords, sizeof(*sh->normals));
    sh->colors = calloc(sh->nCoords, sizeof(*sh->colors));
    sh->textures=calloc(sh->nCoords, sizeof(*sh->textures));
    
    // metal init
    sh->metalDevice=[MetalDevice init];
    
    sh->quads=[sh->metalDevice newData:sh->res2*sizeof(Quad)];
    sh->quadBuff=[sh->metalDevice copyBuffer:sh->quads length:sh->res2*sizeof(Quad)];
    
    [sh readCode:code];
    
    return sh;
}


-(NSString*)getCode:(int)ix {
    return [NSString stringWithFormat:@"%08d", SphericHarmCodes[ix]];
}
-(void)readCode: (int)index { // read a code from the set. default SEQ. and generate coords
    int n = N_SH_CODES, r;
    
    switch (index) {
        case -2: {
            static int c = 0;
            r = c % n;
            c++;
        };
        break; // seq
        case -1: r = [SphericalHarmonics rndInt:n];     break; // read random code
        default: r = index % n;                         break;
    }
    // generate m vect.
    sprintf(code,"%08d", SphericHarmCodes[r]);
    for (int i = 0; i < 8; i++)  m[i] = code[i] - '0';
}

-(void)randomCode { [self readCode:-1]; }
-(void)nextCode   { [self readCode:-2]; }

-(void)setColorMap:(int)colMap {
    self->colourmap=colMap;
    [self genCoords];
}

-(void)genCoords {
    return [self genCoordsMetal];
}


-(void)genCoordsMetal {
    
    // metal init
    [metalDevice compileFunc:@"sphericalHarmonics"];
    
    [metalDevice setBufferParam:quadBuff index:0]; // shader parameters: out:quadBuffer, in:(resolution, m, colourmap)
    [metalDevice setBytesParam:&resolution         length:sizeof(resolution)      index:1];
    [metalDevice setBytesParam:m                   length:sizeof(m)               index:2];
    [metalDevice setBytesParam:&colourmap          length:sizeof(colourmap)       index:3];
    
    [metalDevice runThreadsWidth:resolution height:resolution]; // run in a res x res gpu grid
    
    // generate mesh
    for (int i=0, nc=0; i<res2; i++) {
        for (int j=0; j<4; j++, nc++) { // 1 quad == 4 coords
            coords[nc]   = quads[i].coords[j];       colors[nc]   = quads[i].colors[j];
            normals[nc]  = quads[i].normals[j];      textures[nc] = quads[i].textures[j];
        }
    }
}

@end
