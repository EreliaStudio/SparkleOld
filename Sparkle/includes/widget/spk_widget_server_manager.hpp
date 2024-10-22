#pragma once

#include "network/spk_network_server.hpp"
#include "widget/spk_widget_interface.hpp"

namespace spk::Widget
{
	/** 
	 *  \class ServerManager
	 *  \brief A widget that is used to manage a server. 
	 *
	 *  The ServerManager class is a NoGraphics that controls the life cycle of a Server.
	 *  It provides an interface to set the Server to manage and access it.
	 */
	class ServerManager : public NoGraphics
	{
	private:
		spk::Network::Server* _serverToManage; ///< The server that this manager is controlling.

		/** 
		 *  \brief Private method that is called every frame to update the server state.
		 *
		 *  \return A boolean that represents the state of the update. If the update is successful, returns true. Else, returns false.
		 */
		bool _onUpdate();
	public:
		/** 
		 *  \brief The constructor of the Server class.
		 *
		 *  \param p_name The name of the Server. This is used for identification purposes.
		 */
		ServerManager(const std::wstring& p_name);

		/** 
		 *  \brief Set the server to be managed.
		 *
		 *  \param p_server The server that will be managed by this Server.
		 */
		void setServer(spk::Network::Server* p_server);

		/** 
		 *  @brief Get the server that is currently being managed.
		 *
		 *  @return A pointer to the server being managed.
		 */
		spk::Network::Server* server();

		/** 
		 *  \brief Get the server that is currently being managed, but disallow modification.
		 *
		 *  \return A const pointer to the server being managed.
		 */
		const spk::Network::Server* server() const;
	};
}
