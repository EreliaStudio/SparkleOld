#pragma once

namespace spk
{
    template <typename TType>
    class Value
    {
    public:
        class Default
        {
        private:
            TType _value;
            size_t _count;

            void subscribe()
            {
                _count++;
            }

            void unsubscribe()
            {
                _count--;
            }

        public:
            template <typename ... Args>
            Default(Args&& ... p_args) :
                _value(std::forward<Args>(p_args)...),
                _count(0)
            {

            }
            
            Default(const Default& p_other) :
                _value(p_other._value),
                _count(0)
            {

            }

            ~Default()
            {
                if (_count != 0)
                    throw std::runtime_error("Destroying a Default value still used by at least one Value");
            }

            Default& operator = (const TType& p_newValue)
            {
                _value = p_newValue;
            }

            Default& operator = (const Default& p_other)
            {
                _value = p_other._value;
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

        State _state;
        Default* _default;
        TType _value;

    public:
        Value(const Default& p_defaultValue) : 
            _default(nullptr),
            _value({}),
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

        void setDefaultValue(const Default& p_newValue)
        {
            Default* tmp = const_cast<Default*>(&p_defaultValue);

            if (_default != nullptr)
                *_default->unsubscribe();
            _default = tmp;
            if (_default != nullptr)
                *_default->subscribe();
        }

        Value& operator = (const Default& p_newValue)
        {
            _value = p_newValue;
            _state = State::Custom;
        }

        Value& operator = (const TType& p_newValue)
        {
            _value = p_newValue;
            _state = State::Custom;
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