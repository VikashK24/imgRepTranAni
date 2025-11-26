#include "bmp_img_crud.c"

// void rotate_right(unsigned char *src, unsigned char *dst, int old_w, int old_h)
// {
//     // After rotation: width becomes old_h, height becomes old_w
//     for (int y = 0; y < old_h; y++)
//     {
//         for (int x = 0; x < old_w; x++)
//         {
//             // Map (x, y) from original to (y, old_w - 1 - x) in rotated image
//             dst[x * old_h + (old_h - y - 1)] = src[y * old_w + x];
//         }
//     }
// }

// Sriya
void image_representation()
{
    printf("FileHeader: %zu bytes\n", sizeof(BMPFileHeader));   // Should print 14
    printf("InfoHeader: %zu bytes\n", sizeof(BMPInfoHeader));   // Should print 40
    printf("PaletteEntry: %zu bytes\n", sizeof(BMPColorEntry)); // Should print 4

    // 1. CREATE IMAGE STRUCTURE
    // width and Height of the file
    BMPImage img;
    int H = 200, W = 200;
    img.width = W;
    img.height = H;

    // 2. ALLOCATE MEMORY FOR PIXELS
    img.pixel_data = calloc(H, W);
    // img.pixel_data = calloc(H, W);

    int pxLen = sizeof(img.pixel_data);

    printf("the len: %ld\n", sizeof(img.pixel_data));

    // 3. FILL PIXELS WITH VALUES (0-255)
    for (int j = 0; j < H * W; j += W)
    {
        int k = 0;
        printf("J increment: %d\n", j % 10);
        for (int i = pxLen; i < j / W; i++)
        {
            // if (i <= 100)
            // {

            img.pixel_data[1600 + k + pxLen] = 255;
            k++;
            // }
            // else
            // {
            img.pixel_data[j + i] = 128; // Black Shade
            // }
        }
    }

    int cx = 100, cy = 50, r = 25;

    for (int y = cy - r; y <= cy + r; y++)
    {
        for (int x = cx - r; x <= cx + r; x++)
        {
            if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r)
            {
                img.pixel_data[y * W + x] = 128;
            }
        }
    }

    // 4. ALLOCATE AND CREATE PALETTE
    img.palette = calloc(256, sizeof(BMPColorEntry));
    for (int i = 0; i < 256; i++)
    {
        img.palette[i].b = 25;
        img.palette[i].g = 255;
        img.palette[i].r = 255;
        // img.palette[0].reserved = 0;
    }

    // 5. CALL bmp_write()
    // this function is written in bmp_img_crud.c
    int result = bmp_write("output.bmp", &img);

    // 6. CHECK RESULT
    if (result == 0)
    {
        printf("Success! File created.\n");
    }
    else
    {
        printf("Error! File not created.\n");
    }

    // 7. FREE MEMORY
    free(img.pixel_data);
    free(img.palette);
}

// Dhanraj
void image_transformation(const char *filename)
{

    // this function is coming from bmp_img_crud.c file
    BMPImage *img = bmp_read(filename);

    if (!img)
    {
        printf("Error loading img.\n");
    }
    else
    {
        printf("Width: %d Height: %d\n", img->width, img->height);

        // Print a small region of pixels
        for (int y = 0; y < 3; y++)
            for (int x = 0; x < 3; x++)
                printf("Pixel at (%d,%d): %d\n", x, y, img->pixel_data[y * img->width + x]);

        // Dimensions for the rotated image
        int old_w = img->width, old_h = img->height;
        int new_w = old_h, new_h = old_w;

        // Allocate buffer for rotated image
        unsigned char *rotated = malloc(new_w * new_h);

        // Perform rotation
        rotate_right(img->pixel_data, rotated, old_w, old_h);

        // Create new BMPImage structure for rotated image
        BMPImage rotated_img;
        rotated_img.width = new_w;
        rotated_img.height = new_h;
        rotated_img.pixel_data = rotated;
        rotated_img.palette = malloc(256 * sizeof(BMPColorEntry));
        for (int i = 0; i < 256; i++)
        {
            rotated_img.palette[i].b = img->palette[i].b;
            rotated_img.palette[i].g = img->palette[i].g;
            rotated_img.palette[i].r = img->palette[i].r;
            rotated_img.palette[i].reserved = img->palette->reserved;
        }

        // Write the rotated image to file
        // this function is coming from bmp_img_crud.c file
        bmp_write("output4.bmp", &rotated_img);

        // Free memory for rotated image
        free(rotated_img.pixel_data);
        free(rotated_img.palette);
    }

    // Free memory
    free(img->pixel_data);
    free(img);
}