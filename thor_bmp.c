#include "jarvis_header.h"
// #include "create_test_bmp.c"

// Simple delay function - pauses for 'milliseconds' ms
// void delay_ms(int milliseconds)
// {
//     // Convert milliseconds to approximate loop iterations
//     // This is CPU-dependent; adjust multiplier for your system
//     long iterations = milliseconds * 1000L; // Rough estimate

//     for (long i = 0; i < iterations; i++)
//     {
//         // Do nothing - just burn CPU cycles
//         // Volatile prevents compiler optimization
//         volatile int dummy = 0;
//         dummy = dummy + 1;
//     }
// }

// extern void delay_ms(int milliseconds);

// Test function
int image_animation()
{
    printf("Starting...\n");

    for (int i = 0; i < 100; i++)
    {
        printf("Iteration %d\n", i);
        // create_simple_bmp();
        image_transformation("./output4.bmp");
        delay_ms(140000); // Wait 60 second between iterations
    }

    printf("Done!\n");
    return 0;
}
