/**
 * IZG 23/24L - Computer Graphics Principles - BUT FIT
 * Lab 01 - Color Space Reduction
 *
 * Copyright (c) 2022-23 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *               2023-24 by Tibor Kubik, ikubik@fit.vut.cz
 *
 * This template is for study purposes only, distribution of the code is prohibited without the author's knowledge.
 */

#include "base.h"

/**
 * Example algorithm for color space reduction: Random Ditering.
 * 
 * 
 * Get inspired by this algorithm, the general structure is similar to those you will implement.
 * 
 * Press "R" on loaded image to call this function.
 */
void ImageTransform::randomDithering()
{
    grayscale();

    for (uint32_t y = 0; y < cfg->h; y++)
    {
        for (uint32_t x = 0; x < cfg->w; x++)
        {
            auto p = getPixel(x, y);
            uint8_t value = p.r > getRandom() ? 255 : 0;

            setPixel(x, y, RGB(value));
        }
    }
}

/**
 * Converts (color) image stored in the framebaffer into grayscale.
 * Use empiric equation, you can find the equation in lab slides (slide 8).
 * 
 * Press "G" on loaded image to call this function.
 * 
 * You should call this function in all dithering algorithms as first "preprocessing" step.
 * 
 * You will obtain 0.5 points for successfuly implementing this function.
 */
void ImageTransform::grayscale()
{
    for (uint32_t y = 0; y < cfg->h; y++)
    {
        for (uint32_t x = 0; x < cfg->w; x++)
        {
            auto p = getPixel(x, y);
            auto newVal = std::round(p.r * 0.299 + p.g * 0.587 + p.b * 0.114);

            setPixel(x, y, newVal);
        }
    }
    // Hints: iterate through image array. Extract pixel at each (x, y) position and multiply its individual components
    //        with proper constants.
    //        For the iteration, use cfg->h and cfg->w, representing dimensionality of loaded image.
    //        Do not forget to round the gray value before setting new value to frame buffer.
    //        Use setPixel(x, y, color) to update framebuffer.

    // TODO student's work goes here:
}

/**
 * Applies pixel-based dithering algorithm, Thresholding.
 * 
 * Press "T" on loaded image to call this function.
 * 
 * You will obtain 0.75 points for successfuly implementing thisfunction.
 */
void ImageTransform::threshold()
{
    uint8_t th = 128;
    grayscale();
    for (uint32_t y = 0; y < cfg->h; y++)
    {
        for (uint32_t x = 0; x < cfg->w; x++)
        {
            auto p = getPixel(x, y);

            if (p.g > th) {
                setPixel(x, y, COLOR_WHITE);
            }
            else {
                setPixel(x, y, COLOR_BLACK);
            }

            
        }
    }
    
    // Hints: similarly as in grayscale,  iterate through ys and xs of the image, extract pixel value and compare to threshold.
    //        set new value either to COLOR_WHITE or COLOR_BLACK.
    //        Be careful: getPixel returns RGB structure, but th is unsigned integer. Note that in grayscale images,
    //        all components (e.g. COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b) are the same, so it does not matter
    //        which channel you use.


    // TODO student's work goes here:
}

/**
 * Applies pixel-based dithering algorithm, Random Modulation.
 * 
 * Press "M" on loaded image to call this function.
 * 
 * You will obtain 0.5 points for successfuly implementing thisfunction.
 */
void ImageTransform::randomModulation()
{
    uint8_t th = 128;
    grayscale();
    for (uint32_t y = 0; y < cfg->h; y++)
    {
        for (uint32_t x = 0; x < cfg->w; x++)
        {
            auto p = getPixel(x, y);
            auto randomVal = getRandomFromRange(-p.g, 255 - p.g);

            if ((p.g - randomVal) > th) {
                setPixel(x, y, COLOR_WHITE);
            }
            else {
                setPixel(x, y, COLOR_BLACK);
            }

        }
    }
    // Hints: now you do not directly compare the itensity to 'th', but you have to substract random value from the intensity first.
    //        To generate the random number to substract, call 'getRandomFromRange(int lowerBound, int upperBound)'.
    //        Be aware that the bounds might be negative, too, so apply appropriate conversion to int.


    // TODO student's work goes here:


}

/**
 * Given pixel position 'x' and 'y', itensity of pixel (x, y) is amplified with value 'err'.
 * Function checks for accessing out of the framebuffer.
 * Function checks for color value underflow of overflow: it will always stay in range [0, 255].
 * 
 * Use this function to update neighboring pixels in Error Distribution and Tone-Dependent Error Distribution algorithms.
 */
void ImageTransform::updatePixelWithError(uint32_t x, uint32_t y, float err)
{
    if (x >= cfg->w || y >= cfg->h)
        return;

    auto p = getPixel(x, y);

    p.r = p.g = p.b = std::max(0.0f, std::min(std::round(p.r + err), 255.0f));
    setPixel(x, y, p);
}

/**
 * Applies neighborhood-based algorithm, Error Distribution.
 * 
 * Press "E" on loaded image to call this function.
 * 
 * You will obtain 0.75 points for successfuly implementing thisfunction.
 */
void ImageTransform::errorDistribution()
{

    // Hints: iterate through pixels, get new value as in thresholding, but compute error as well (see lab slides).
    //        Distribute the error to the surrounding pixels according to the figure in lab slides.

    uint8_t th = 128;

    // TODO student's work goes here:
    grayscale();
    for (uint32_t y = 0; y < cfg->h; y++)
    {
        for (uint32_t x = 0; x < cfg->w; x++)
        {
            auto p = getPixel(x, y);

            if (p.g > th) {
                float error = p.g - 255;
                updatePixelWithError(x + 1, y, error * 3 / 8);
                updatePixelWithError(x, y + 1, error * 3 / 8);
                updatePixelWithError(x + 1, y + 1, error * 2 / 8);
                setPixel(x, y, COLOR_WHITE);
            }
            else {
                float error = p.g;
                updatePixelWithError(x + 1, y, error * 3 / 8);
                updatePixelWithError(x, y + 1, error * 3 / 8);
                updatePixelWithError(x + 1, y + 1, error * 2 / 8);
                setPixel(x, y, COLOR_BLACK);
            }

        }
    }

}

/**
 * Applies neighborhood-based algorithm, Tone-Dependent Error Distribution.
 * 
 * Press "W" on loaded image to call this function.
 * 
 * You will obtain 0.5 points for successfuly implementing thisfunction.
 */
void ImageTransform::toneDependentErrorDistribution()
{
    // Hints: iterate through pixels, get new value as in thresholding, but compute error as well (see lab slides).
    //        Distribute the error to the surrounding pixels according to the figure in lab slides.
    //        This time, however, various distributions are used based on the extracted pixel value (you want to
    //        call updatePixelWithError() differently according to intensity ranges, as described in lab slides).

    uint8_t th = 128;

    // TODO student's work goes here:
    grayscale();
    for (uint32_t y = 0; y < cfg->h; y++)
    {
        for (uint32_t x = 0; x < cfg->w; x++)
        {
            auto p = getPixel(x, y);

            if (p.g >= 0 && p.g <= 40) {
                float error = p.g;
                updatePixelWithError(x + 1, y, error * 7 / 16);
                updatePixelWithError(x, y + 1, error * 5 / 16);
                updatePixelWithError(x + 1, y + 1, error * 1 / 16);
                updatePixelWithError(x - 1, y + 1, error * 3 / 16);
                setPixel(x, y, COLOR_BLACK);
            }
            else if (p.g >= 215 && p.g <= 255) {
                float error = p.g - COLOR_WHITE.g;
                updatePixelWithError(x + 1, y, error * 7 / 16);
                updatePixelWithError(x, y + 1, error * 5 / 16);
                updatePixelWithError(x + 1, y + 1, error * 1 / 16);
                updatePixelWithError(x - 1, y + 1, error * 3 / 16);
                setPixel(x, y, COLOR_WHITE);
            }
            else {
                if (p.g > th) {
                    float error = p.g - COLOR_WHITE.g;
                    updatePixelWithError(x + 1, y, error * 7 / 24);
                    updatePixelWithError(x + 2, y, error * 2 / 24);

                    updatePixelWithError(x - 2, y + 1, error * 1 / 24);
                    updatePixelWithError(x - 1, y + 1, error * 3 / 24);
                    updatePixelWithError(x, y + 1, error * 5 / 24);
                    updatePixelWithError(x + 1, y + 1, error * 1 / 24);
                    updatePixelWithError(x + 2, y + 1, error * 0.5 / 24);

                    updatePixelWithError(x - 2, y + 2, error * 1 / 24);
                    updatePixelWithError(x - 1, y + 2, error * 1 / 24);
                    updatePixelWithError(x, y + 2, error * 2 / 24);
                    updatePixelWithError(x + 1, y + 2, error * 0.5 / 24);
                    setPixel(x, y, COLOR_WHITE);
                }
                else {
                    float error = p.g;
                    updatePixelWithError(x + 1, y, error * 7 / 24);
                    updatePixelWithError(x + 2, y, error * 2 / 24);

                    updatePixelWithError(x - 2, y + 1, error * 1 / 24);
                    updatePixelWithError(x - 1, y + 1, error * 3 / 24);
                    updatePixelWithError(x, y + 1, error * 5 / 24);
                    updatePixelWithError(x + 1, y + 1, error * 1 / 24);
                    updatePixelWithError(x + 2, y + 1, error * 0.5 / 24);

                    updatePixelWithError(x - 2, y + 2, error * 1 / 24);
                    updatePixelWithError(x - 1, y + 2, error * 1 / 24);
                    updatePixelWithError(x, y + 2, error * 2 / 24);
                    updatePixelWithError(x + 1, y + 2, error * 0.5 / 24);
                    setPixel(x, y, COLOR_BLACK);
                }

            }


            // TODO student's work goes here:
        }
    }
}