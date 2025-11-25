#include "common.h"

// __attribute__((packed))
// BMP header might be longer than 14 bytes (adding garbage padding)
// Packed struct = correct byte layout on disk
// Unpacked struct = wrong layout, corrupted file
typedef struct __attribute__((packed))
{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BMPFileHeader;

typedef struct __attribute__((packed))
{
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_resolution;
    int32_t y_resolution;
    uint32_t colors_used;
    uint32_t important_colors;
} BMPInfoHeader;

typedef struct __attribute__((packed))
{
    uint8_t b, g, r, reserved;
} BMPColorEntry;

// Complete image structure
typedef struct
{
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    BMPColorEntry *palette;    // 256 entries for 8-bit grayscale
    unsigned char *pixel_data; // The actual image bytes
    int width;
    int height;
} BMPImage;

int bmp_write(const char *filename, BMPImage *image);
BMPImage *bmp_read(const char *filename);
void rotate_right(unsigned char *src, unsigned char *dst, int old_w, int old_h);
void image_representation();
void image_transformation();
