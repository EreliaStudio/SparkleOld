#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "input/spk_input_status.hpp"
#include <map>
#include <vector>
#include <string>

namespace spk
{
	/**
	 * @class Keyboard
	 * @brief Singleton class for keyboard inputs.
	 * @details This class handles keyboard inputs and provides key mapping and layout configuration.
	 */
	class Keyboard
	{
		friend class KeyboardModule;

	public:
		/**
		 * @enum Key
		 * @brief Enum for representing keyboard keys.
		 */
		enum Key
		{
			Unknown = -1,  // Represents an unknown key
			Backspace,
			Tab,
			Clear,
			Return,
			Pause,
			Capslock,
			Escape,
			Convert,
			Non_convert,
			Accept,
			Mode_change,
			Space,
			Prior,
			Next,
			End, Home,
			LeftArrow, UpArrow, RightArrow, DownArrow,
			Select,
			Print,
			Execute,
			Snapshot,
			Insert, Delete,
			Help,
			Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, 
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			LeftWindows, RightWindows,
			Apps,
			Sleep,
			Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
			NumpadMultiply, NumpadPlus, NumpadMinus, NumpadDivide,
			NumpadSeparator, NumpadDecimal, NumpadReturn,
			NumLock,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
			Scroll,
			Shift, LeftShift, RightShift,
			Control, LeftControl, RightControl,
			Alt, LeftAlt, RightAlt,
			Menu, LeftMenu, RightMenu,
			Plus, Minus, Equal,
			Comma, Period, SemiColon,
			QuestionMark,
			Tilde,
			LeftBracket, RightBracket,
			VerticalLine,
			Quote,
			Unknow,
			AngleBracket,
			Process,
			Backslash, Slash,

			SIZE // Represents the total number of keys
		};

	private:
		/**
		 * @struct Mapping
		 * @brief Structure to handle key mappings.
		 * @details This structure handles key bindings for specific layouts.
		 */
		struct Mapping
		{
			Key keys[Key::SIZE];  // Array to store key mapping

			Mapping();
						/**
			 * @brief Binds a key to an index.
			 * @param p_index The index to bind the key to.
			 * @param p_key The key to bind.
			 */
			void bindKey(size_t p_index, Key p_key);
			Key operator[](size_t p_index);
		};

		static std::map<Keyboard::Key, std::wstring> KeyToStringMap;
		static const std::wstring UnknowKeyName;

		Mapping _mapping;
		std::atomic_uint8_t _keys[Key::SIZE];
		std::vector<Key> _keysToUpdate;
	public:
		Keyboard();

	    /**
		 * @brief Registers a key press event.
		 * @param p_key The key that was pressed.
		 */
		void pressKey(uint32_t p_key);
		
		/**
		 * @brief Registers a key release event.
		 * @param p_key The key that was released.
		 */
		void releaseKey(uint32_t p_key);

		/**
		 * @brief Updates the state of the keyboard.
		 */
		void update();

		/**
		 * @brief Returns the input status of a key.
		 * @param p_key The key to get the input status of.
		 * @return The input status of the key.
		 */
		InputState inputStatus(Key p_key) const
		{
			return (static_cast<InputState>(_keys[static_cast<size_t>(p_key)].load()));
		}

		/**
		 * @brief Converts a key to a string.
		 * @param p_key The key to convert to a string.
		 * @return The string representation of the key.
		 */
		static const std::wstring &keyToString(Keyboard::Key p_key);
	};


	/**
	 * @brief Converts a Keyboard::Key to a string.
	 * @param p_key The Keyboard::Key to convert to a string.
	 * @return The string representation of the Keyboard::Key.
	 */
	const std::wstring &to_wstring(Keyboard::Key p_key);
}