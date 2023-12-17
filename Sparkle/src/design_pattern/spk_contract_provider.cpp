#include "design_pattern/spk_callback_container.hpp"
#include "iostream/spk_iostream.hpp"

namespace spk
{
	CallbackContainer::Contract::Contract(Container::iterator p_iterator, CallbackContainer::Container* p_container) :
		_iterator(p_iterator),
		_container(p_container),
		_isOwner(true)
	{

	}

	CallbackContainer::Contract::Contract() :
		_iterator(),
		_container(nullptr),
		_isOwner(false)
	{

	}

	CallbackContainer::Contract::Contract(Contract&& other) noexcept :
		_iterator(other._iterator),
		_container(other._container),
		_isOwner(other._isOwner)
	{
		other._isOwner = false;
		other._container = nullptr;
	}

	CallbackContainer::Contract& CallbackContainer::Contract::operator=(Contract&& other) noexcept
	{
		if (this != &other) {
			resign();
			_iterator = other._iterator;
			_container = other._container;
			_isOwner = other._isOwner;

			other._isOwner = false;
			other._container = nullptr;
		}
		return *this;
	}

	CallbackContainer::Contract::~Contract()
	{
		resign();
	}

	void CallbackContainer::Contract::amend(std::function<void()> p_newCallback)
	{
		if (_container) {
			*_iterator = p_newCallback;
		}
	}

	void CallbackContainer::Contract::resign()
	{
		if (_isOwner && _container) {
			_container->erase(_iterator);
			_isOwner = false;
		}
	}

	size_t CallbackContainer::size() const
	{
		return (_callbacks.size());
	}

	CallbackContainer::Contract CallbackContainer::subscribe(std::function<void()> p_callback)
	{
		_callbacks.push_back(p_callback);

		return (std::move(Contract(--(_callbacks.end()), &_callbacks)));
	}

	void CallbackContainer::notify() const
	{
		for (auto& callback : _callbacks) {
			callback();
		}
	}
}
