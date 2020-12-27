#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"

/******************************************************************************

  Compile the program:
    nvcc -o ImageBlur ImageBlur.cu 

  Run the program:
	./ImageBlur

******************************************************************************/

__global__ void blur(unsigned char *gpu_image_output, unsigned char *gpu_image_input, int width, int height)
{

    int count = 1;

    int idx = blockDim.x * blockIdx.x + threadIdx.x;

    int i = blockIdx.x;
    int j = threadIdx.x;

    int t_red = 0;
    int t_green = 0;
    int t_blue = 0;
    int t_alpha = 0;

    if (i + 1 && j - 1)
    {

        int pos = blockDim.x * (blockIdx.x + 1) + threadIdx.x - 1;
        int pixel = pos * 4;

        t_red = gpu_image_input[idx * 4];
        t_green = gpu_image_input[idx * 4 + 1];
        t_blue = gpu_image_input[idx * 4 + 2];
        t_alpha = gpu_image_input[idx * 4 + 3];

        t_red += gpu_image_input[pixel];
        t_green += gpu_image_input[1 + pixel];
        t_blue += gpu_image_input[2 + pixel];
        t_alpha += gpu_image_input[3 + pixel];

        count++;
    }

    if (j + 1)
    {

        int pos = blockDim.x * (blockIdx.x) + threadIdx.x + 1;

        int pixel = pos * 4;

        t_red = gpu_image_input[idx * 4];
        t_green = gpu_image_input[idx * 4 + 1];
        t_blue = gpu_image_input[idx * 4 + 2];
        t_alpha = gpu_image_input[idx * 4 + 3];

        t_red += gpu_image_input[pixel];
        t_green += gpu_image_input[1 + pixel];
        t_blue += gpu_image_input[2 + pixel];
        t_alpha += gpu_image_input[3 + pixel];

        count++;
    }

    if (i + 1 && j + 1)
    {

        int pos = blockDim.x * (blockIdx.x + 1) + threadIdx.x + 1;

        int pixel = pos * 4;

        t_red = gpu_image_input[idx * 4];
        t_green = gpu_image_input[idx * 4 + 1];
        t_blue = gpu_image_input[idx * 4 + 2];
        t_alpha = gpu_image_input[idx * 4 + 3];

        t_red += gpu_image_input[pixel];
        t_green += gpu_image_input[1 + pixel];
        t_blue += gpu_image_input[2 + pixel];
        t_alpha += gpu_image_input[3 + pixel];

        count++;
    }

    if (i + 1)
    {

        int pos = blockDim.x * (blockIdx.x + 1) + threadIdx.x;

        int pixel = pos * 4;

        t_red = gpu_image_input[idx * 4];
        t_green = gpu_image_input[idx * 4 + 1];
        t_blue = gpu_image_input[idx * 4 + 2];
        t_alpha = gpu_image_input[idx * 4 + 3];

        t_red += gpu_image_input[pixel];
        t_green += gpu_image_input[1 + pixel];
        t_blue += gpu_image_input[2 + pixel];
        t_alpha += gpu_image_input[3 + pixel];

        count++;
    }

    if (j - 1)
    {

        int pos = blockDim.x * (blockIdx.x) + threadIdx.x - 1;

        int pixel = pos * 4;

        t_red = gpu_image_input[idx * 4];
        t_green = gpu_image_input[idx * 4 + 1];
        t_blue = gpu_image_input[idx * 4 + 2];
        t_alpha = gpu_image_input[idx * 4 + 3];

        t_red += gpu_image_input[pixel];
        t_green += gpu_image_input[1 + pixel];
        t_blue += gpu_image_input[2 + pixel];
        t_alpha += gpu_image_input[3 + pixel];

        count++;
    }

    if (i - 1)
    {

        int pos = blockDim.x * (blockIdx.x - 1) + threadIdx.x;

        int pixel = pos * 4;

        t_red = gpu_image_input[idx * 4];
        t_green = gpu_image_input[idx * 4 + 1];
        t_blue = gpu_image_input[idx * 4 + 2];
        t_alpha = gpu_image_input[idx * 4 + 3];

        t_red += gpu_image_input[pixel];
        t_green += gpu_image_input[1 + pixel];
        t_blue += gpu_image_input[2 + pixel];
        t_alpha += gpu_image_input[3 + pixel];

        count++;
    }

    int current_pixel = idx * 4;

    gpu_image_output[current_pixel] = t_red / count;
    gpu_image_output[1 + current_pixel] = t_green / count;
    gpu_image_output[2 + current_pixel] = t_blue / count;
    gpu_image_output[3 + current_pixel] = gpu_image_input[3 + current_pixel];
}

int main(int argc, char **argv)
{

    unsigned int error;
    unsigned int encError;
    unsigned char *image;
    unsigned int width;
    unsigned int height;
    const char *filename = "cat.png";
    const char *new_file_name = "blurred_cat.png";

    error = lodepng_decode32_file(&image, &width, &height, filename);
    if (error)
    {
        printf("error %u: %s\n", error, lodepng_error_text(error));
    }

    const int ARRAY_SIZE = width * height * 4;
    const int ARRAY_BYTES = ARRAY_SIZE * sizeof(unsigned char);

    unsigned char *host_image_input = (unsigned char *)malloc(ARRAY_SIZE * 4);
    unsigned char *host_image_output = (unsigned char *)malloc(ARRAY_SIZE * 4);

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        host_image_input[i] = image[i];
    }

    // declare GPU memory pointers
    unsigned char *d_in;
    unsigned char *d_out;

    // allocate GPU memory
    cudaMalloc((void **)&d_in, ARRAY_BYTES);
    cudaMalloc((void **)&d_out, ARRAY_BYTES);

    cudaMemcpy(d_in, host_image_input, ARRAY_BYTES, cudaMemcpyHostToDevice);

    // launch the kernel
    blur<<<height, width>>>(d_out, d_in, width, height);

    // copy back the result array to the CPU
    cudaMemcpy(host_image_output, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost);

    encError = lodepng_encode32_file(new_file_name, host_image_output, width, height);
    if (encError)
    {
        printf("error %u: %s\n", error, lodepng_error_text(encError));
    }

    //free(image);
    //free(host_image_input);
    cudaFree(d_in);
    cudaFree(d_out);

    return 0;
}
