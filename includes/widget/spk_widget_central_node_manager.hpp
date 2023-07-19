#pragma once

#include "network/spk_central_node.hpp"
#include "widget/spk_widget_interface.hpp"

namespace spk::Widget
{    /**
     * @class CentralNode
     * @brief A no-graphics widget class for managing a spk::CentralNode.
     *
     * The CentralNode class is a graphical user interface widget that can manage an instance of spk::CentralNode. 
     * This includes rendering the central node status on the GUI, handling geometry changes, and performing periodic updates.
     */
	class CentralNodeManager : public NoGraphics
	{
	private:
		spk::CentralNode* _centralNodeToManage; /**< Pointer to the central node to manage */

        /**
         * @brief Method to handle updates.
         *
         * @return bool Returns false when the central node doesn't exist or no updates are required.
         */
		bool _onUpdate();

	public:
		/**
         * @brief Constructs a new CentralNode.
         *
         * @param p_name Name of the widget.
         */
		CentralNodeManager(const std::wstring& p_name);

        /**
         * @brief Sets the central node to manage.
         *
         * @param p_centralNode Pointer to the central node to manage.
         */
		void setCentralNode(spk::CentralNode* p_centralNode);

        /**
         * @brief Gets the central node being managed.
         *
         * @return CentralNode* Pointer to the central node being managed.
         */
		spk::CentralNode* centralNode();

		/**
         * @brief Gets the central node being managed. Const variant.
         *
         * @return const CentralNode* Const pointer to the central node being managed.
         */
		const spk::CentralNode* centralNode() const;
	};
}