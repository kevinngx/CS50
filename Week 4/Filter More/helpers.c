#include <stdio.h>
#include <math.h>
#include "helpers.h"

int gx[3][3] =
{
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

int gy[3][3] =
{
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}
};

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            pixel = image[h][w];
            BYTE avg = round((pixel.rgbtRed + pixel.rgbtBlue + pixel.rgbtGreen) / 3.0);
            pixel.rgbtRed = avg;
            pixel.rgbtBlue = avg;
            pixel.rgbtGreen = avg;
            image[h][w] = pixel;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmpRow[width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            tmpRow[w] = image[h][w]; // copies image into tmpRow
        }

        for (int w = 0; w < width; w++)
        {
            image[h][w] = tmpRow[width - (w + 1)]; // copies tmp back in reverse
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE blurred_image[height][width];
    RGBTRIPLE pixel;

    float red = 0;
    float green = 0;
    float blue = 0;

    int surround_count = 0;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {

            // For a given pixel, take the avg of its surrounding pixels
            for (int ph = h - 1; ph <  h + 2; ph++)
            {
                for (int pw = w - 1; pw < w + 2; pw++)
                {
                    if (ph >= 0 && ph < height && pw >= 0 && pw < width)
                    {

                        red += image[ph][pw].rgbtRed;
                        green += image[ph][pw].rgbtGreen;
                        blue += image[ph][pw].rgbtBlue;
                        surround_count++;
                    }
                }
            }

            // Create new pixel in blurred_image using average colors of its surrounding elements
            if (surround_count > 0)
            {
                pixel.rgbtRed = round(red / surround_count);
                pixel.rgbtGreen = round(green / surround_count);
                pixel.rgbtBlue = round(blue / surround_count);
            }

            blurred_image[h][w] = pixel;

            // Reset variables
            pixel.rgbtRed = 0;
            pixel.rgbtGreen = 0;
            pixel.rgbtBlue = 0;
            red = 0;
            green = 0;
            blue = 0;
            surround_count = 0;
        }
    }

    //image = blurred_image;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = blurred_image[h][w];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edge_image[height][width];
    RGBTRIPLE pixel;

    // GX Values
    float xRed = 0;
    float xGreen = 0;
    float xBlue = 0;

    // GY Values
    float yRed = 0;
    float yGreen = 0;
    float yBlue = 0;

    int x;
    int y;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {

            // For a given pixel, perform calculations on its surround pairs
            for (int ph = h - 1; ph <  h + 2; ph++)
            {
                for (int pw = w - 1; pw < w + 2; pw++)
                {
                    if (ph >= 0 && ph < height && pw >= 0 && pw < width)
                    {
                        x = pw - (w - 1);
                        y = ph - (h - 1);

                        xRed    += image[ph][pw].rgbtRed * gx[y][x]; // pw - (w-1) gives us the coordinate in gx and yx accordingly
                        xGreen  += image[ph][pw].rgbtGreen * gx[y][x];
                        xBlue   += image[ph][pw].rgbtBlue * gx[y][x];

                        yRed    += image[ph][pw].rgbtRed * gy[y][x];
                        yGreen  += image[ph][pw].rgbtGreen * gy[y][x];
                        yBlue   += image[ph][pw].rgbtBlue * gy[y][x];

                    }
                }
            }

            pixel.rgbtRed   = fmin(round(sqrt(xRed * xRed + yRed * yRed)), 255);
            pixel.rgbtGreen = fmin(round(sqrt(xGreen * xGreen + yGreen * yGreen)), 255);
            pixel.rgbtBlue  = fmin(round(sqrt(xBlue * xBlue + yBlue * yBlue)), 255);

            edge_image[h][w] = pixel;

            // Reset variables
            pixel.rgbtRed = 0;
            pixel.rgbtGreen = 0;
            pixel.rgbtBlue = 0;

            xRed = 0;
            xGreen = 0;
            xBlue = 0;

            yRed = 0;
            yGreen = 0;
            yBlue = 0;

        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = edge_image[h][w];
        }
    }
    return;
}

