// create_test_bmp.c - Create a simple BMP to test with
#include "header.h"

void create_simple_bmp() {
    FILE* f = fopen("test.bmp", "wb");
    int w = 64, h = 64;
    int row_size = (w * 3 + 3) & ~3;
    int img_size = row_size * h;
    
    // BMP header
    unsigned char header[54] = {
        0x42,0x4D,0,0,0,0,0,0,0,0,54,0,0,0,40,0,0,0,
        w,0,0,0, h,0,0,0,1,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    
    // Update size fields
    *(int*)(header+2) = 54 + img_size;
    *(int*)(header+18) = w;
    *(int*)(header+22) = h;
    *(int*)(header+34) = img_size;
    
    fwrite(header, 1, 54, f);
    
    // Create gradient pattern
    unsigned char* pixels = malloc(img_size);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int pos = y * row_size + x * 3;
            pixels[pos] = x * 4;       // Blue
            pixels[pos+1] = y * 4;     // Green  
            pixels[pos+2] = 128;       // Red
        }
    }
    
    fwrite(pixels, 1, img_size, f);
    free(pixels);
    fclose(f);
    printf("Created test.bmp - use it with the viewer!\n");
}

