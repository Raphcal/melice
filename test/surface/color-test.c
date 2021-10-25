//
//  color-test.c
//  melice
//
//  Created by Raphaël Calabro on 24/10/2021.
//

#include <stdlib.h>
#include <melice.h>
#include <assert.h>

int main(int argc, char **argv) {
    MELColor color = MELColorMake(0.1f, 0.2f, 0.3f, 0.5f);
    MELUInt32Color uint32Color = MELColorToRGBAUInt32Color(color);
    MELUInt8Color uint8Color = *((MELUInt8Color *) &uint32Color);
    printf("MELColor -> MELUInt32Color -> MELUInt8Color\n");
    printf("red: %d -> %d, green: %d -> %d, blue: %d -> %d, alpha: %d -> %d\n",
           (int) (color.red * 255.0f), uint8Color.red,
           (int) (color.green * 255.0f), uint8Color.green,
           (int) (color.blue * 255.0f), uint8Color.blue,
           (int) (color.alpha * 255.0f), uint8Color.alpha);

    uint8Color = MELUInt8ColorMake(32, 64, 128, 192);
    uint32Color = *((MELUInt32Color *) &uint8Color);
    printf("%x\n", uint32Color);
}
