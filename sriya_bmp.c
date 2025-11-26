#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1) // Ensure structs have exact memory layout

// BMP file header (14 bytes)
struct BMPFileHeader {
    uint16_t file_type;   // "BM" in ASCII
    uint32_t file_size;   // Total file size
    uint16_t reserved1;   // Must be 0
    uint16_t reserved2;   // Must be 0
    uint32_t offset_data; // Pixel data offset
};

// BMP info header (40 bytes)
struct BMPInfoHeader {
    uint32_t size;              // Header size (40 bytes)
    int32_t width;              // Image width
    int32_t height;             // Image height
    uint16_t planes;            // Must be 1
    uint16_t bit_count;         // Bits per pixel (24 = RGB)
    uint32_t compression;       // 0 = no compression
    uint32_t size_image;        // Pixel data size
    int32_t x_pixels_per_meter; // Horizontal resolution
    int32_t y_pixels_per_meter; // Vertical resolution
    uint32_t colors_used;       // Number of colors
    uint32_t colors_important;  // Important colors
};

#pragma pack(pop)

int main() {
    int width = 800, height = 600;

    // Each row must be multiple of 4 bytes (padding)
    int row_size = (width * 3 + 3) & ~3;
    int image_size = row_size * height;

    // Allocate memory for all pixels
    unsigned char *pixels = malloc(image_size);
    if (!pixels) return 1;

    // Fill image with white pixels
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * row_size + x * 3;
            pixels[index]     = 255; // Blue
            pixels[index + 1] = 255; // Green
            pixels[index + 2] = 255; // Red
        }
        // Fill padding bytes with 0
        for (int p = width * 3; p < row_size; p++) {
            pixels[y * row_size + p] = 0;
        }
    }

    // Initialize BMP headers
    struct BMPFileHeader file_header = {0};
    struct BMPInfoHeader info_header = {0};

    file_header.file_type = 0x4D42; // "BM"
    file_header.file_size = sizeof(struct BMPFileHeader) + sizeof(struct BMPInfoHeader) + image_size;
    file_header.offset_data = sizeof(struct BMPFileHeader) + sizeof(struct BMPInfoHeader);

    info_header.size = sizeof(struct BMPInfoHeader);
    info_header.width = width;
    info_header.height = height;
    info_header.planes = 1;
    info_header.bit_count = 24;
    info_header.compression = 0;
    info_header.size_image = image_size;

    // Write BMP file
    FILE *f = fopen("output2.bmp", "wb");
    if (!f) return 1;

    fwrite(&file_header, sizeof(file_header), 1, f);
    fwrite(&info_header, sizeof(info_header), 1, f);
    fwrite(pixels, image_size, 1, f);

    fclose(f);
    free(pixels);

    printf("White BMP created: output2.bmp\n");
    return 0;
}
