#pragma once

#include "network/spk_server.hpp"
#include "widget/spk_abstract_widget.hpp"

namespace spk
{
	/** 
	 *  \class ServerWidget
	 *  \brief A widget that is used to manage a server. 
	 *
	 *  The ServerWidget class is a NoGraphicsWidget that controls the life cycle of a Server.
	 *  It provides an interface to set the Server to manage and access it.
	 */
	class ServerWidget : public NoGraphicsWidget
	{
	private:
		Server* _serverToManage; ///< The server that this manager is controlling.

		/** 
		 *  \brief Private method that is called every frame to update the server state.
		 *
		 *  \return A boolean that represents the state of the update. If the update is successful, returns true. Else, returns false.
		 */
		bool _onUpdate();
	public:
		/** 
		 *  \brief The constructor of the ServerWidget class.
		 *
		 *  \param p_name The name of the ServerWidget. This is used for identification purposes.
		 */
		ServerWidget(const std::wstring& p_name);

		/** 
		 *  \brief Set the server to be managed.
		 *
		 *  \param p_server The server that will be managed by this ServerWidget.
		 */
		void setServer(Server* p_server);

		/** 
		 *  @brief Get the server that is currently being managed.
		 *
		 *  @return A pointer to the server being managed.
		 */
		Server* server();

		/** 
		 *  \brief Get the server that is currently being managed, but disallow modification.
		 *
		 *  \return A const pointer to the server being managed.
		 */
		const Server* server() const;
	};
}
