#pragma once

#include "graphics/frame/spk_abstract_frame.hpp"

namespace spk
{
    /**
     * @brief An abstract class representing a surface for rendering.
     *
     * This class serves as the base class for all surface types,
     * providing the essential interfaces for resizing, clearing, and rendering.
     */
    class AbstractSurface
    {
    protected:
        /// A pointer to the associated frame.
        const spk::AbstractFrame *_frame;

    public:
        /**
         * @brief Constructs an AbstractSurface associated with the given frame.
         *
         * @param p_frame A pointer to the frame to be associated with this surface.
         */
        AbstractSurface(const spk::AbstractFrame *p_frame);

        /**
         * @brief Deleted copy constructor for AbstractSurface.
         *
         * Copying instances of this class is not permitted to ensure that resources
         * managed by AbstractSurface are not duplicated or improperly managed.
         */
        AbstractSurface(const AbstractSurface &) = delete;

        /**
         * @brief Deleted copy assignment operator for AbstractSurface.
         *
         * Copy assignment is not permitted to ensure that resources managed by the
         * AbstractSurface are not duplicated or improperly managed.
         */
        AbstractSurface &operator=(const AbstractSurface &) = delete;

        /**
         * @brief Default move constructor for AbstractSurface.
         *
         * Allows instances of this class to be moved, transferring ownership of resources
         * from the source instance to the new instance.
         */
        AbstractSurface(AbstractSurface &&) = default;

        /**
         * @brief Default move assignment operator for AbstractSurface.
         *
         * Allows move assignment, transferring ownership of resources from the source
         * instance to this instance, replacing the current resources.
         */
        AbstractSurface &operator=(AbstractSurface &&) = default;

        /**
         * @brief Virtual function for resizing the surface.
         */
        virtual void resize() = 0;

        /**
         * @brief Virtual function for clearing the surface.
         */
        virtual void clear() = 0;

        /**
         * @brief Virtual function for rendering to the surface.
         */
        virtual void render() = 0;
    };
}
