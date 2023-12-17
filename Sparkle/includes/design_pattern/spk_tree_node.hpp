#pragma once 

#include <memory>
#include <vector>
#include <functional>
#include <iostream>

namespace spk
{
	template <typename TType>
	class TreeNode
	{
	public:
		using Parent = TreeNode<TType>;
		using Child = TreeNode<TType>;
		using LinkingCallback = std::function<void(TType*)>;
		using UnlinkingCallback = std::function<void()>;
		
	private:
		Parent* _parent;
		std::vector<TType*> _childrens;
		LinkingCallback _linkingCallback;
		UnlinkingCallback _unlinkingCallback;

		void _insertChildren(Child* p_child)
		{
			_childrens.push_back(static_cast<TType*>(p_child));
			p_child->_parent = this;
			if (p_child->_linkingCallback != nullptr)
				p_child->_linkingCallback(static_cast<TType*>(this));
		}

	public:
		TreeNode() : 
			_parent(nullptr)
		{

		}

		~TreeNode()
		{
			if (_parent != nullptr)
				_parent->removeChild(this);
		}

		TreeNode(const TreeNode& p_other) = delete;
		TreeNode& operator =(const TreeNode& p_other) = delete;

		TreeNode(TreeNode&& p_other)
		{
			if (p_other._parent != nullptr)
			{
				p_other._parent->addChild(this);
			}
			p_other.transferChildrens(this);
		}

		TreeNode& operator =(TreeNode&& p_other)
		{
			if (p_other._parent != nullptr)
			{
				p_other._parent->addChild(this);
				p_other._parent->removeChild(&p_other);
			}
			p_other.transferChildrens(this);
			return (*this);
		}

		void setLinkingCallback(const LinkingCallback& p_callback)
		{
			_linkingCallback = p_callback;
		}

		void setUnlinkingCallback(const UnlinkingCallback& p_callback)
		{
			_unlinkingCallback = p_callback;
		}

		const TType* parent() const
		{
			return (static_cast<TType*>(_parent));
		}

		const std::vector<TType*> childrens() const
		{
			return (_childrens);
		}

		void addChild(Child* p_child)
		{
			if (p_child->_parent != nullptr)
				p_child->_parent->removeChild(p_child);
			_insertChildren(p_child);
		}

		void removeChild(Child* p_child)
		{
			auto it = std::find(_childrens.begin(), _childrens.end(), p_child);
			if (it != _childrens.end())
			{
				_childrens.erase(it);
				p_child->_parent = nullptr;
				if (p_child->_unlinkingCallback != nullptr)
					p_child->_unlinkingCallback();
			}
		}

		void transferChildrens(Parent* p_newParent)
		{
			for (size_t i = 0; i < _childrens.size(); i++)
			{
				p_newParent->_insertChildren(_childrens[i]);
			}
			_childrens.clear();
		}
	};
}