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