#pragma once

#include "design_pattern/spk_stated_object.hpp"

namespace spk
{
    class ActivableObject : protected StatedObject<bool>
    {
    private:

    public:
        void activate()
        {
            setState(true);
        }
        void deactivate()
        {
            setState(false);
        }
        const bool& isActive() const {return (state());}

        Contract addActivationCallback(Callback p_callback)
        {
            return (std::move(addStateCallback(true, p_callback)));
        }
        Contract addDeactivationCallback(Callback p_callback)
        {
            return (std::move(addStateCallback(false, p_callback)));
        }
    };
}