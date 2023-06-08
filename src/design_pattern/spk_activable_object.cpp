#include "design_pattern/spk_activable_object.hpp"

namespace spk
{
    void ActivableObject::activate()
    {
        setState(true);
    }
    void ActivableObject::deactivate()
    {
        setState(false);
    }
    const bool& ActivableObject::isActive() const
    {
        return (state());
    }

    ActivableObject::Contract ActivableObject::addActivationCallback(Callback p_callback)
    {
        return (std::move(addStateCallback(true, p_callback)));
    }
    ActivableObject::Contract ActivableObject::addDeactivationCallback(Callback p_callback)
    {
        return (std::move(addStateCallback(false, p_callback)));
    }
}