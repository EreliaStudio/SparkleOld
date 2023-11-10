#pragma once

#include "math/spk_vector2.hpp"

namespace spk
{
    /**
     * @brief A class representing a 2D area in terms of an anchor point and size.
     * 
     * This class is used to define a rectangular area using anchor point and size vectors.
     * It also provides functions for modifying these attributes.
     */
	class Area
	{
	private:
        /// Anchor point for the area in 2D space.
		spk::Vector2Int _anchor;
        /// Size of the area in 2D space.
		spk::Vector2UInt _size;

	public:
        /**
         * @brief Default constructor.
         * 
         * Initializes the area with default anchor and size values.
         */
		Area();

        /**
         * @brief Constructor with anchor and size.
        */
        Area(const spk::Vector2Int& p_anchor, const spk::Vector2UInt& p_size);

        /**
         * @brief Sets the geometry of the area.
         * 
         * @param p_anchor The new anchor point for the area.
         * @param p_size The new size for the area.
         */
		void setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2UInt& p_size);

        /**
         * @brief Sets the anchor point of the area.
         * 
         * @param p_anchor The new anchor point for the area.
         */
		void setAnchor(const spk::Vector2Int& p_anchor);

        /**
         * @brief Sets the size of the area.
         * 
         * @param p_size The new size for the area.
         */
		void setSize(const spk::Vector2UInt& p_size);

        /**
         * @brief Moves the anchor point by a delta.
         * 
         * @param p_delta The delta by which the anchor point should be moved.
         */
		void move(const spk::Vector2Int& p_delta);

        /**
         * @brief Gets the anchor point of the area.
         * 
         * @return The current anchor point.
         */
		constexpr const spk::Vector2Int& anchor() const
		{
			return (_anchor);
		}

        /**
         * @brief Gets the size of the area.
         * 
         * @return The current size.
         */
		constexpr const spk::Vector2UInt& size() const
		{
			return (_size);
		}

        /**
         *  @brief Checks if a point lies inside this area.
         *  
         *  @tparam TType The numerical type of the point coordinates.
         *  @param p_point The point to check, specified as a vector.
         *  
         *  @return True if the point lies inside the area, False otherwise.
         */
        template <typename TType>
        bool isInside(const spk::IVector2<TType>& p_point) const
        {
            return (spk::Vector2Int::isInsideRectangle(p_point ,_anchor, _anchor + _size));
        }

        /**
         *  @brief Adds two Areas together.
         *  
         *  @param p_other The other Area to add.
         *  @return A new Area that is the sum of this and the other Area.
         */
        Area operator+(const Area& p_other) const;

        /**
         *  @brief Adds another Area to this Area.
         *  
         *  @param p_other The other Area to add.
         *  @return The modified Area after addition.
         */
        Area& operator+=(const Area& p_other);

        /**
         *  @brief Subtracts another Area from this Area.
         *  
         *  @param p_other The other Area to subtract.
         *  @return A new Area that is the result of the subtraction.
         */
        Area operator-(const Area& p_other) const;

        /**
         *  @brief Subtracts another Area from this Area and stores the result in this Area.
         *  
         *  @param p_other The other Area to subtract.
         *  @return The modified Area after subtraction.
         */
        Area& operator-=(const Area& p_other);

        /**
         *  @brief Multiplies two Areas together.
         *  
         *  @param p_other The other Area to multiply.
         *  @return A new Area that is the product of this and the other Area.
         */
        Area operator*(const Area& p_other) const;

        /**
         *  @brief Multiplies another Area with this Area and stores the result in this Area.
         *  
         *  @param p_other The other Area to multiply.
         *  @return The modified Area after multiplication.
         */
        Area& operator*=(const Area& p_other);

        /**
         *  @brief Divides this Area by another Area.
         *  
         *  @param p_other The other Area to divide by.
         *  @return A new Area that is the quotient of this and the other Area.
         */
        Area operator/(const Area& p_other) const;

        /**
         *  @brief Divides this Area by another Area and stores the result in this Area.
         *  
         *  @param p_other The other Area to divide by.
         *  @return The modified Area after division.
         */
        Area& operator/=(const Area& p_other);

        /**
         *  @brief Compares if this Area is not equal to another Area.
         *  
         *  @param p_other The other Area to compare against.
         *  @return True if the Areas are not equal, False otherwise.
         */
        bool operator!=(const Area& p_other) const;

        /**
         *  @brief Compares if this Area is equal to another Area.
         *  
         *  @param p_other The other Area to compare against.
         *  @return True if the Areas are equal, False otherwise.
         */
        bool operator==(const Area& p_other) const;

		friend std::wostream &operator<<(std::wostream &p_os, const Area &p_self)
		{
			p_os << L"Anchor : " << p_self._anchor << L" / Size : " << p_self._size;
			return p_os;
		}
	};

    /**
     * @brief Alias for Area, used to represent a scissor region for rendering.
     * 
     * The Scissor is a rectangular area where rendering will be limited to.
     * Outside this area, no pixels will be affected. It also uses an anchor point and a size.
     */
    using Scissor = Area;
}
