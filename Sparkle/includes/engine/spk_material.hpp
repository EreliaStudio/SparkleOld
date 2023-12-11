#pragma once

namespace spk
{
    /**
     * @brief Represents the material properties of a surface in the rendering engine.
     *
     * This struct encapsulates the specular properties of a material, which define how shiny 
     * or reflective a surface appears in the rendered scene. The specular intensity and power
     * are used to calculate the specular reflection component in lighting calculations.
     */
    struct Material
    {
        float specularIntensity; ///< Intensity of the specular reflections.
        float specularPower;     ///< Power of the specular reflections, affecting the sharpness.

        /**
         * @brief Default constructor for Material.
         * 
         * Initializes the material with default specular properties.
         */
        Material();

        /**
         * @brief Parameterized constructor for Material.
         * 
         * @param p_specularIntensity The specular intensity of the material.
         * @param p_specularPower The specular power of the material.
         */
        Material(const float& p_specularIntensity, const float& p_specularPower);

        /**
         * @brief Equality operator.
         * 
         * Compares this material with another for equality based on their specular properties.
         * 
         * @param p_other The material to compare with.
         * @return true if both materials have the same specular properties.
         * @return false otherwise.
         */
        bool operator==(const Material& p_other) const;

        /**
         * @brief Inequality operator.
         * 
         * Compares this material with another for inequality based on their specular properties.
         * 
         * @param p_other The material to compare with.
         * @return true if the materials have different specular properties.
         * @return false otherwise.
         */
        bool operator!=(const Material& p_other) const;
    };
}
