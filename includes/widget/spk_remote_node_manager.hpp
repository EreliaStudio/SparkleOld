#pragma once

#include "network/spk_remote_node.hpp"
#include "widget/spk_abstract_widget.hpp"

namespace spk
{
	/**
	 * \brief Manager class for RemoteNode. Inherits from NoGraphicsWidget.
	 */
	class RemoteNodeManager : public NoGraphicsWidget
	{
	private:
		RemoteNode* _remoteNodeToManage; ///< The RemoteNode object to manage.

		/**
		 * \brief Called each frame. Updates the internal state of the widget.
		 * \return Boolean value. True if the update was successful, false otherwise.
		 */
		bool _onUpdate();

	public:
		/**
		 * \brief Constructor for RemoteNodeManager class.
		 * \param p_name Name of the RemoteNodeManager object.
		 */
		RemoteNodeManager(const std::wstring& p_name);

		/**
		 * \brief Sets the RemoteNode object to manage.
		 * \param p_client Pointer to the RemoteNode object.
		 */
		void setRemoteNode(RemoteNode* p_client);

		/**
		 * \brief Gets the RemoteNode object being managed.
		 * \return Pointer to the RemoteNode object being managed.
		 */
		RemoteNode* remoteNode();

		/**
		 * \brief Gets the RemoteNode object being managed.
		 * \return Constant pointer to the RemoteNode object being managed.
		 */
		const RemoteNode* remoteNode() const;
	};
}
