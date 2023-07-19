#pragma once

#include "network/spk_client.hpp"
#include "widget/spk_widget_interface.hpp"

namespace spk::Widget
{
	/**
	 * \brief Manager class for Client. Inherits from NoGraphics.
	 */
	class ClientManager : public NoGraphics
	{
	private:
		spk::Client* _clientToManage; ///< The Client object to manage.

		/**
		 * \brief Called each frame. Updates the internal state of the widget.
		 * \return Boolean value. True if the update was successful, false otherwise.
		 */
		bool _onUpdate();

	public:
		/**
		 * \brief Constructor for Client class.
		 * \param p_name Name of the Client object.
		 */
		ClientManager(const std::wstring& p_name);

		/**
		 * \brief Sets the Client object to manage.
		 * \param p_client Pointer to the Client object.
		 */
		void setClient(spk::Client* p_client);

		/**
		 * \brief Gets the Client object being managed.
		 * \return Pointer to the Client object being managed.
		 */
		spk::Client* client();

		/**
		 * \brief Gets the Client object being managed.
		 * \return Constant pointer to the Client object being managed.
		 */
		const spk::Client* client() const;
	};
}
