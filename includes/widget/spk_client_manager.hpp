#pragma once

#include "network/spk_client.hpp"
#include "interface/spk_abstract_widget.hpp"

namespace spk
{
	/**
	 * \brief Manager class for Client. Inherits from NoGraphicsWidget.
	 */
	class ClientManager : public NoGraphicsWidget
	{
	private:
		Client* _clientToManage; ///< The Client object to manage.

		/**
		 * \brief Called each frame. Updates the internal state of the widget.
		 * \return Boolean value. True if the update was successful, false otherwise.
		 */
		bool _onUpdate();

	public:
		/**
		 * \brief Constructor for ClientManager class.
		 * \param p_name Name of the ClientManager object.
		 */
		ClientManager(const std::wstring& p_name);

		/**
		 * \brief Sets the Client object to manage.
		 * \param p_client Pointer to the Client object.
		 */
		void setClient(Client* p_client);

		/**
		 * \brief Gets the Client object being managed.
		 * \return Pointer to the Client object being managed.
		 */
		Client* client();

		/**
		 * \brief Gets the Client object being managed.
		 * \return Constant pointer to the Client object being managed.
		 */
		const Client* client() const;
	};
}
