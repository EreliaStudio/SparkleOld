#pragma once

#include "network/spk_central_node.hpp"
#include "widget/spk_abstract_widget.hpp"

namespace spk
{    /**
     * @class CentralNodeWidget
     * @brief A no-graphics widget class for managing a spk::CentralNode.
     *
     * The CentralNodeWidget class is a graphical user interface widget that can manage an instance of spk::CentralNode. 
     * This includes rendering the central node status on the GUI, handling geometry changes, and performing periodic updates.
     */
	class CentralNodeWidget : public NoGraphicsWidget
	{
	private:
		CentralNode* _centralNodeToManage; /**< Pointer to the central node to manage */

        /**
         * @brief Method to handle updates.
         *
         * @return bool Returns false when the central node doesn't exist or no updates are required.
         */
		bool _onUpdate();

	public:
		/**
         * @brief Constructs a new CentralNodeWidget.
         *
         * @param p_name Name of the widget.
         */
		CentralNodeWidget(const std::wstring& p_name);

        /**
         * @brief Sets the central node to manage.
         *
         * @param p_centralNode Pointer to the central node to manage.
         */
		void setCentralNode(CentralNode* p_centralNode);

        /**
         * @brief Gets the central node being managed.
         *
         * @return CentralNode* Pointer to the central node being managed.
         */
		CentralNode* centralNode();

		/**
         * @brief Gets the central node being managed. Const variant.
         *
         * @return const CentralNode* Const pointer to the central node being managed.
         */
		const CentralNode* centralNode() const;
	};
}