#pragma once

#include <iostream>  // for std::wostream

namespace spk
{
    /**
     * @brief A structure representing RGBA color.
     */
    struct Color
    {
        /// Red component of the color.
        float r;
        /// Green component of the color.
        float g;
        /// Blue component of the color.
        float b;
        /// Alpha (transparency) component of the color.
        float a;

        /**
         * @brief Default constructor initializes color to transparent black.
         */
        Color();

        /**
         * @brief Constructor to initialize color with grayscale value.
         *
         * @param p_value The grayscale value [0, 255].
         */
        Color(int p_value);

        /**
         * @brief Constructor to initialize color from integer values.
         *
         * @param p_r Red component [0, 255].
         * @param p_g Green component [0, 255].
         * @param p_b Blue component [0, 255].
         * @param p_a Alpha component [0, 255] (default is 255).
         */
        Color(int p_r, int p_g, int p_b, int p_a = 255);

        /**
         * @brief Constructor to initialize color from float values.
         *
         * @param p_r Red component [0.0f, 1.0f].
         * @param p_g Green component [0.0f, 1.0f].
         * @param p_b Blue component [0.0f, 1.0f].
         * @param p_a Alpha component [0.0f, 1.0f] (default is 1.0f).
         */
        Color(float p_r, float p_g, float p_b, float p_a = 1.0f);

        /**
         * @brief Addition operator to add two colors.
         *
         * @param p_color The color to add to the current color.
         * @return A new color resulting from the addition.
         */
        Color operator + (const Color& p_color) const;

        /**
         * @brief Subtraction operator to subtract a color from the current color.
         *
         * @param p_color The color to subtract from the current color.
         * @return A new color resulting from the subtraction.
         */
        Color operator - (const Color& p_color) const;

        /**
         * @brief Stream insertion operator to print the color values.
         *
         * @param p_os The output stream.
         * @param p_values The color to print.
         * @return The output stream.
         */
        friend std::wostream& operator << (std::wostream& p_os, const Color& p_values);
    };
}
