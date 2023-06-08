#pragma once

#include "design_pattern/spk_stated_object.hpp"

namespace spk
{
    class ActivableObject : protected StatedObject<bool>
    {
    private:

    public:
        void activate();
        void deactivate();
        const bool& isActive() const;

        Contract addActivationCallback(Callback p_callback);
        Contract addDeactivationCallback(Callback p_callback);
    };
}