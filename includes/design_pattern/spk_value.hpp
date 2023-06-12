#pragma once

#include "design_pattern/spk_contract_provider.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
    template <typename TType>
    class Value : public ContractProvider
    {
        friend class Default;

    public:
        class Default
        {
            friend class Value;

        private:
            TType _value;

            std::vector<Value<TType>*> _subscribers;

            void subscribe(Value<TType>* p_subscriber)
            {
                _subscribers.push_back(p_subscriber);
            }

            void unsubscribe(Value<TType>* p_subscriber)
            {
                _subscribers.erase(std::remove(_subscribers.begin(), _subscribers.end(), p_subscriber), _subscribers.end());
            }
            
            void _triggerSubscriberEditionCallbacks()
            {
                for (size_t i = 0; i < _subscribers.size(); i++)
                {
                    _subscribers[i]->_triggerEditionCallback();
                }
            }

        public:
            template <typename ... Args>
            Default(Args&& ... p_args) :
                _value(std::forward<Args>(p_args)...),
                _subscribers()
            {

            }
            
            Default(const Default& p_other) :
                _value(p_other._value),
                _subscribers()
            {

            }

            ~Default() noexcept(false)
            {
                if (_subscribers.empty() == false)
                    throw std::runtime_error("Destroying a Default value still used by at least one Value");
            }

            Default& operator = (const TType& p_newValue)
            {
                _value = p_newValue;
                _triggerSubscriberEditionCallbacks();
                return (*this);
            }

            Default& operator = (const Default& p_other)
            {
                _value = p_other._value;
                _triggerSubscriberEditionCallbacks();
                return (*this);
            }

            operator const TType&() const
            {
                return (_value);
            }
        };

    private:
        enum class State
        {
            Default,
            Custom
        };

        CallbackContainer _onEditionCallbacks;
        State _state;
        Default* _default;
        TType _value;

        void _triggerEditionCallback()
        {
            for (size_t i = 0; i < _onEditionCallbacks.size(); i++)
            {
                _onEditionCallbacks[i]();
            }
        }

    public:
        Value(const Default& p_defaultValue) : 
            _default(nullptr),
            _value(),
            _state(State::Default)
        {
            setDefaultValue(p_defaultValue);
        }

        template <typename ... Args>
        Value(const Default& p_defaultValue, Args&& ... p_args) : 
            _default(nullptr),
            _value(std::forward<Args>(p_args)...),
            _state(State::Custom)
        {
            setDefaultValue(p_defaultValue);
        }

        Value(const Value& p_other) : 
            _default(p_other._default),
            _value(p_other._value),
            _state(p_other._state)
        {
            _default->subscribe(this);
        }

        ~Value()
        {
            _default->unsubscribe(this);
        }

        Contract subscribe(Callback p_callback)
        {
            return (std::move(ContractProvider::subscribe(_onEditionCallbacks, p_callback)));
        }
        
        void reset()
        {
            _state = State::Default;
            _triggerEditionCallback();
        }

        void setDefaultValue(const Default& p_defaultValue)
        {
            Default* tmp = const_cast<Default*>(&p_defaultValue);

            if (_default != nullptr)
                _default->unsubscribe(this);
            _default = tmp;
            if (_default != nullptr)
                _default->subscribe(this);
        }

        Value& operator = (const Default& p_defaultValue)
        {
            setDefaultValue(p_defaultValue);
            _state = State::Default;
            _triggerEditionCallback();
            return (*this);
        }

        Value& operator = (const Value& p_other)
        {
            setDefaultValue(*p_other._default);
            _value = p_other._value;
            _state = p_other._state;
            _triggerEditionCallback();
            return (*this);
        }

        Value& operator = (const TType& p_newValue)
        {
            _value = p_newValue;
            _state = State::Custom;
            _triggerEditionCallback();
            return (*this);
        }

        operator const TType&() const
        {
            if (_state == State::Custom)
                return (_value);
            else
                return (*_default);
        }
    };
}