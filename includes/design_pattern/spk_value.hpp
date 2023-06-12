#pragma once

namespace spk
{
    template <typename TType>
    class Value
    {
    public:
        class Default
        {
            friend class Value;

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

            ~Default() noexcept(false)
            {
                if (_count != 0)
                    throw std::runtime_error("Destroying a Default value still used by at least one Value");
            }

            Default& operator = (const TType& p_newValue)
            {
                _value = p_newValue;
                return (*this);
            }

            Default& operator = (const Default& p_other)
            {
                _value = p_other._value;
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

        State _state;
        Default* _default;
        TType _value;

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

        ~Value()
        {
            _default->unsubscribe();
        }
        
        void reset()
        {
            _state = State::Default;
        }

        void setDefaultValue(const Default& p_defaultValue)
        {
            Default* tmp = const_cast<Default*>(&p_defaultValue);

            if (_default != nullptr)
                _default->unsubscribe();
            _default = tmp;
            if (_default != nullptr)
                _default->subscribe();
        }

        Value& operator = (const Default& p_defaultValue)
        {
            setDefaultValue(p_defaultValue);
            _state = State::Default;
            return (*this);
        }

        Value& operator = (const TType& p_newValue)
        {
            _value = p_newValue;
            _state = State::Custom;
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