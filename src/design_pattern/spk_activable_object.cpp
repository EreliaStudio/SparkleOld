#include "design_pattern/spk_activable_object.hpp"

namespace spk
{
    ActivableObject::ActivableObject(bool p_activable) : StatedObject<bool>(p_activable)
    {
    }

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

    ActivableObject::Contract ActivableObject::addActivationCallback(const Callback& p_callback)
    {
        return (std::move(addStateCallback(true, p_callback)));
    }
    ActivableObject::Contract ActivableObject::addDeactivationCallback(const Callback& p_callback)
    {
        return (std::move(addStateCallback(false, p_callback)));
    }
}