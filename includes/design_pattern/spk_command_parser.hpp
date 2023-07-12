#pragma once
#include <map>
#include <functional>
#include <deque>
#include <cstddef>
#include <stdexcept>

namespace spk
{
	/**
	 * @brief The CommandParser class is responsible for parsing and processing commands.
	 */
	class CommandParser
	{
	public:
		/**
		 * @brief Type representing a command callback function.
		 * The callback function takes a command parameter of type std::wstring.
		 */
		using CommandCallback = std::function<void(const std::wstring&)>;

		std::wstring _commandsIndicator;            /**< The command indicator string. */
		std::wstring _commandsSeparator;            /**< The command separator string. */
		std::map<std::wstring, CommandCallback> _commandsCallback;    /**< Map storing command callbacks. */
		CommandCallback _noCommandCallback;         /**< Callback function for when no command is found. */

	public:
		/**
		 * @brief Default constructor for the CommandParser class.
		 * Creates a CommandParser object with default settings.
		 * The commands separator is set at space (" ");
		 */
		CommandParser();

		/**
		 * @brief Sets the command indicator.
		 *
		 * @param p_commandIndicator The command indicator to set.
		 */
		void setCommandsIndicator(const std::wstring& p_commandIndicator);


		/**
		 * @brief Sets the command separator.
		 *
		 * @param p_commandSeparator The command separator to set.
		 */
		void setCommandsSeparator(const std::wstring& p_commandSeparator);

		/**
		 * @brief Sets the callback function to be executed when no command is found.
		 *
		 * @param p_callback The callback function to set.
		 */
		void setNoCommandCallback(const CommandCallback& p_callback);

		/**
		 * @brief Adds a callback function for a specific command.
		 *
		 * @param p_command The command to associate the callback function with.
		 * @param p_callback The callback function to associate with the command.
		 */
		void addCommandCallback(const std::wstring& p_command, const CommandCallback& p_callback);

		/**
		 * @brief Parses the command line and executes the corresponding callback function.
		 *
		 * @param p_commandLine The command line to parse.
		 */
		void parse(const std::wstring& p_commandLine);
	};

}