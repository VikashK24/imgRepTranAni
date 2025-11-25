#include "jarvis_header.h"

BMPImage *bmp_read(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return NULL;
    }

    BMPImage *image = malloc(sizeof(BMPImage));
    if (image == NULL)
    {
        fclose(file);
        return NULL;
    }

    // Step 1: Read file header
    fread(&image->file_header, sizeof(BMPFileHeader), 1, file);

    // Step 2: Validate magic number
    if (image->file_header.type != 0x4D42)
    { // "BM"
        fprintf(stderr, "Not a valid BMP file\n");
        free(image);
        fclose(file);
        return NULL;
    }

    // Step 3: Read info header
    fread(&image->info_header, sizeof(BMPInfoHeader), 1, file);

    // Step 4: Store dimensions
    image->width = image->info_header.width;
    image->height = image->info_header.height;

    // Step 5: Read palette (for 8-bit grayscale)
    image->palette = malloc(256 * sizeof(BMPColorEntry));
    fread(image->palette, sizeof(BMPColorEntry), 256, file);

    // Step 6: Seek to pixel data
    fseek(file, image->file_header.offset, SEEK_SET);

    // Step 7: Allocate and read pixel data
    int data_size = image->width * image->height;
    image->pixel_data = malloc(data_size);

    // Handle row padding (rows must be multiple of 4 bytes)
    int row_padded_width = ((image->width + 3) / 4) * 4;
    unsigned char *row_buffer = malloc(row_padded_width);

    // BMP is stored bottom-to-top, so read in reverse
    for (int y = image->height - 1; y >= 0; y--)
    {
        fread(row_buffer, 1, row_padded_width, file);
        // Copy only the actual pixels (ignore padding)
        for (int x = 0; x < image->width; x++)
        {
            image->pixel_data[y * image->width + x] = row_buffer[x];
        }
    }

    free(row_buffer);
    fclose(file);
    return image;
}

int bmp_write(const char *filename, BMPImage *image)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        fprintf(stderr, "Cannot create file: %s\n", filename);
        return -1;
    }

    // Step 1: Calculate sizes
    int row_padded_width = ((image->width + 3) / 4) * 4; // multiple of 4
    int pixel_data_size = row_padded_width * image->height;
    int palette_size = 256 * 4; // 256 entries, 4 bytes each = 1024 bytes
    int file_size = 14 + 40 + palette_size + pixel_data_size;

    // Step 2: Update headers
    image->file_header.type = 0x4D42; // "BM"
    image->file_header.size = file_size;
    image->file_header.offset = 14 + 40 + palette_size;
    image->info_header.header_size = 40;
    image->info_header.width = image->width;
    image->info_header.height = image->height;
    image->info_header.planes = 1;
    image->info_header.bits_per_pixel = 8;
    image->info_header.compression = 0;
    image->info_header.image_size = pixel_data_size;
    // image->info_header.x_resolution = 2000;
    // image->info_header.y_resolution = 4000;

    // Step 3: Write file header
    fwrite(&image->file_header, sizeof(BMPFileHeader), 1, file);

    // Step 4: Write info header
    fwrite(&image->info_header, sizeof(BMPInfoHeader), 1, file);

    // Step 5: Write palette
    fwrite(image->palette, sizeof(BMPColorEntry), 256, file);

    // Step 6: Write pixel data (bottom-to-top)
    unsigned char *row_buffer = calloc(row_padded_width, 1);
    for (int y = image->height - 1; y >= 0; y--)
    {
        for (int x = 0; x < image->width; x++)
        {
            row_buffer[x] = image->pixel_data[y * image->width + x];
        }
        fwrite(row_buffer, 1, row_padded_width, file);
    }

    free(row_buffer);
    fclose(file);
    return 0;
}

void rotate_right(unsigned char *src, unsigned char *dst, int old_w, int old_h)
{
    // After rotation: width becomes old_h, height becomes old_w
    for (int y = 0; y < old_h; y++)
    {
        for (int x = 0; x < old_w; x++)
        {
            // Map (x, y) from original to (y, old_w - 1 - x) in rotated image
            dst[x * old_h + (old_h - y - 1)] = src[y * old_w + x];
        }
    }
}

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
            img.pixel_data[j + i] = 128; // Gray
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

void image_transformation()
{

    BMPImage *img = bmp_read("./output2.bmp");

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
        bmp_write("output4.bmp", &rotated_img);

        // Free memory for rotated image
        free(rotated_img.pixel_data);
        free(rotated_img.palette);
    }

    // Free memory
    free(img->pixel_data);
    free(img);
}