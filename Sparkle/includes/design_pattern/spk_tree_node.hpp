#pragma once 

#include <memory>
#include <vector>
#include <functional>
#include <iostream>

namespace spk
{
	/**
     * @class TreeNode
     * @brief A template class representing a node in a tree structure.
     *
     * @tparam TType The type of the value that the tree node holds.
     *
     * This class supports parent-child relationships, where each node can have multiple children
     * but only a single parent. It also allows for custom callback functions to be executed 
     * during linking (adding a child) and unlinking (removing a child) operations.
     */
	template <typename TType>
	class TreeNode
	{
	public:
		using Parent = TreeNode<TType>; ///< A using directing to an acceptable parent type
		using Child = TreeNode<TType>; ///< A using directing to an acceptable child type
		using LinkingCallback = std::function<void(TType*)>; ///< A using directing to a callback used when linking a TreeNode
		using UnlinkingCallback = std::function<void()>; ///< A using directing to a callback used when unlinking a TreeNode
		
	private:
		Parent* _parent; /**< Pointer to the parent node. */
        std::vector<TType*> _childrens; /**< Vector of pointers to the children nodes. */
        LinkingCallback _linkingCallback; /**< Callback function for linking a child node. */
        UnlinkingCallback _unlinkingCallback; /**< Callback function for unlinking a child node. */

        /**
         * @brief Internal method to insert a child into the children vector.
         * @param p_child Pointer to the child node to be inserted.
         */
		void _insertChildren(Child* p_child)
		{
			_childrens.push_back(static_cast<TType*>(p_child));
			p_child->_parent = this;
			if (p_child->_linkingCallback != nullptr)
				p_child->_linkingCallback(static_cast<TType*>(this));
		}

	public:
        /**
         * @brief Default constructor. Initializes the tree node with no parent.
         */
		TreeNode() : 
			_parent(nullptr)
		{

		}

        /**
         * @brief Destructor. Unlinks from parent and removes itself from parent's children, if applicable.
         */
		~TreeNode()
		{
			if (_parent != nullptr)
				_parent->removeChild(this);
		}

		TreeNode(const TreeNode& p_other) = delete;
		TreeNode& operator =(const TreeNode& p_other) = delete;

        /**
         * @brief Move constructor. Moves the node and its relationships from another node.
         * @param p_other The other TreeNode to move from.
         */
		TreeNode(TreeNode&& p_other)
		{
			if (p_other._parent != nullptr)
			{
				p_other._parent->addChild(this);
			}
			p_other.transferChildrens(this);
		}

        /**
         * @brief Move assignment operator. Moves the node and its relationships from another node.
         * @param p_other The other TreeNode to move from.
         * @return Reference to this TreeNode.
         */
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

        /**
         * @brief Sets the linking callback function.
         * @param p_callback The callback function to set for linking.
         */
		void setLinkingCallback(const LinkingCallback& p_callback)
		{
			_linkingCallback = p_callback;
		}

        /**
         * @brief Sets the unlinking callback function.
         * @param p_callback The callback function to set for unlinking.
         */
		void setUnlinkingCallback(const UnlinkingCallback& p_callback)
		{
			_unlinkingCallback = p_callback;
		}

        /**
         * @brief Gets the parent of this node.
         * @return Constant pointer to the parent node.
         */
		const TType* parent() const
		{
			return (static_cast<TType*>(_parent));
		}

        /**
         * @brief Gets the children of this node.
         * @return Vector of constant pointers to the children nodes.
         */
		const std::vector<TType*>& childrens() const
		{
			return (_childrens);
		}

        /**
         * @brief Adds a child node to this node.
         * @param p_child Pointer to the child node to be added.
         */
		void addChild(Child* p_child)
		{
			if (p_child->_parent != nullptr)
				p_child->_parent->removeChild(p_child);
			_insertChildren(p_child);
		}

        /**
         * @brief Removes a child node from this node.
         * @param p_child Pointer to the child node to be removed.
         */
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

        /**
         * @brief Transfers all children of this node to another parent node.
         * @param p_newParent Pointer to the new parent node to which children will be transferred.
         */
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