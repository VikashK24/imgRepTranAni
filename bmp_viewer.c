// red_square_viewer.c
#include <stdio.h>

int main() {
    FILE* f = fopen("test.bmp", "rb");
    if (!f) {                  //shorthand for if(f == NULL)
        printf("Error: Can't open test.bmp\n");
        return 1;
    }
    
    // Read BMP width and height
    int width, height;
    fseek(f, 18, SEEK_SET);
    fread(&width, 4, 1, f);
    fread(&height, 4, 1, f);
    
    printf(" TEST BMP: %dx%d pixels\n", width, height);
    printf("Preview:\n");
    
    // Jump to pixel data
    fseek(f, 54, SEEK_SET);
    
    // Display the 8x8 red square
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char b, g, r;
            fread(&b, 1, 1, f);
            fread(&g, 1, 1, f);
            fread(&r, 1, 1, f);
            
            // Check if pixel is RED
            if (r == 255 && g == 0 && b == 0) {
                printf("R ");  // Red pixel
            } else {
                printf(". ");  // Other color
            }
        }
        printf("\n");
    }
    
    fclose(f);
    printf("\nR = TEST BMP (8x8 TEST BMP)\n");
    return 0;
}