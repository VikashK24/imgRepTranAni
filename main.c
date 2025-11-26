#include "header.h"

int main()
{
    create_simple_bmp();
while (true) {
    int input = 0;
    printf("Enter the function number you want to run\n1. Image Representation\t2. Image Transformation\n3. Image Animation\nChoose an option: ");
    scanf("%d", &input);
    if (input == 1)  image_representation();
    // else if (input == 2)  spider_man();
    else if (input == 2) image_transformation("./output.bmp");
    else if (input == 3) image_animation();
    else {
        printf("Your option is incorrect please choose it again!\n");
        continue;
    }
}
     return 0;
}