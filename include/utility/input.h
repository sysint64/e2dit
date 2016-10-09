/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UTILITY.

 * E2DIT-UTILITY is free library: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * E2DIT-UTILITY is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-UTILITY.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */
 
#ifndef E2DIT_UTILITY_INPUT_H
#define E2DIT_UTILITY_INPUT_H

/* Mouse */

#include <map>

#define mouseLeft			sf::Mouse::Left
#define mouseRight			sf::Mouse::Right
#define mouseMiddle			sf::Mouse::Middle
#define mouseWheellUp		0
#define mouseWheellDown		0
#define mouseNone		   -1

/* Keyboard */

#define keyUnknown			sf::Keyboard::Unknown
#define keyBackspace		sf::Keyboard::BackSpace
#define keyEnd				sf::Keyboard::End
#define keyHome				sf::Keyboard::Home
#define keyLeftArrow		sf::Keyboard::Left
#define keyUpArrow			sf::Keyboard::Up
#define keyRightArrow		sf::Keyboard::Right
#define keyDownArrow		sf::Keyboard::Down
#define keyDelete			sf::Keyboard::Delete
#define keyPageUp			sf::Keyboard::PageUp
#define keyPageDown			sf::Keyboard::PageDown
#define keyLAlt				sf::Keyboard::LAlt
#define keyRAlt				sf::Keyboard::RAlt
#define keyAlt				256
#define keyLShift			sf::Keyboard::LShift
#define keyRShift			sf::Keyboard::RShift
#define keyShift			257
#define keyLCtrl			sf::Keyboard::LControl
#define keyRCtrl			sf::Keyboard::RControl
#define keyCtrl				258
#define keyEnter			sf::Keyboard::Return
#define keyEscape			sf::Keyboard::Escape
#define keyF2				sf::Keyboard::F2
#define keyRSuper			sf::Keyboard::RSystem
#define keyLSuper			sf::Keyboard::LSystem
#define keyTab				sf::Keyboard::Tab

/* Letters */

#define keyA				sf::Keyboard::A
#define keyB				sf::Keyboard::B
#define keyC				sf::Keyboard::C
#define keyD				sf::Keyboard::D
#define keyE				sf::Keyboard::E
#define keyF				sf::Keyboard::F
#define keyG				sf::Keyboard::G
#define keyH				sf::Keyboard::H
#define keyI				sf::Keyboard::I
#define keyJ				sf::Keyboard::J
#define keyK				sf::Keyboard::K
#define keyL				sf::Keyboard::L
#define keyM				sf::Keyboard::M
#define keyN				sf::Keyboard::N
#define keyO				sf::Keyboard::O
#define keyP				sf::Keyboard::P
#define keyQ				sf::Keyboard::Q
#define keyR				sf::Keyboard::R
#define keyS				sf::Keyboard::S
#define keyT				sf::Keyboard::T
#define keyU				sf::Keyboard::U
#define keyV				sf::Keyboard::V
#define keyW				sf::Keyboard::W
#define keyX				sf::Keyboard::X
#define keyY				sf::Keyboard::Y
#define keyZ				sf::Keyboard::Z

/* Numbers */

#define keyAdd				sf::Keyboard::Add
#define keySub				sf::Keyboard::Subtract

namespace keyboard {
	static std::map<int, std::string> map = {
		{sf::Keyboard::A, "A"},
		{sf::Keyboard::B, "B"},
		{sf::Keyboard::C, "C"},
		{sf::Keyboard::D, "D"},
		{sf::Keyboard::E, "E"},
		{sf::Keyboard::F, "F"},
		{sf::Keyboard::G, "G"},
		{sf::Keyboard::H, "H"},
		{sf::Keyboard::I, "I"},
		{sf::Keyboard::J, "J"},
		{sf::Keyboard::K, "K"},
		{sf::Keyboard::L, "L"},
		{sf::Keyboard::M, "M"},
		{sf::Keyboard::N, "N"},
		{sf::Keyboard::O, "O"},
		{sf::Keyboard::P, "P"},
		{sf::Keyboard::Q, "Q"},
		{sf::Keyboard::R, "R"},
		{sf::Keyboard::S, "S"},
		{sf::Keyboard::T, "T"},
		{sf::Keyboard::U, "U"},
		{sf::Keyboard::V, "V"},
		{sf::Keyboard::W, "W"},
		{sf::Keyboard::X, "X"},
		{sf::Keyboard::Y, "Y"},
		{sf::Keyboard::Z, "Z"},

		{sf::Keyboard::Num0, "0"},
		{sf::Keyboard::Num1, "1"},
		{sf::Keyboard::Num2, "2"},
		{sf::Keyboard::Num3, "3"},
		{sf::Keyboard::Num4, "4"},
		{sf::Keyboard::Num5, "5"},
		{sf::Keyboard::Num6, "6"},
		{sf::Keyboard::Num7, "7"},
		{sf::Keyboard::Num8, "8"},
		{sf::Keyboard::Num9, "9"},

		{sf::Keyboard::Escape   , "Esc"},
		{sf::Keyboard::LControl , "LCtrl"},
		{sf::Keyboard::LShift   , "LShift"},
		{sf::Keyboard::LAlt     , "LAlt"},
		{sf::Keyboard::LSystem  , "LSystem"},
		{sf::Keyboard::RControl , "RCtrl"},
		{sf::Keyboard::RShift   , "RShift"},
		{sf::Keyboard::RAlt     , "RAlt"},
		{sf::Keyboard::RSystem  , "RSystem"},
		{sf::Keyboard::Menu     , "Menu"},
		{sf::Keyboard::LBracket , "["},
		{sf::Keyboard::RBracket , "]"},
		{sf::Keyboard::SemiColon, ";"},
		{sf::Keyboard::Comma    , ","},
		{sf::Keyboard::Period   , "."},
		{sf::Keyboard::Quote    , "'"},
		{sf::Keyboard::Slash    , "/"},
		{sf::Keyboard::BackSlash, "\\"},
		{sf::Keyboard::Tilde    , "~"},
		{sf::Keyboard::Equal    , "="},
		{sf::Keyboard::Dash     , "-"},
		{sf::Keyboard::Space    , "Space"},
		{sf::Keyboard::Return   , "Return"},
		{sf::Keyboard::BackSpace, "BackSpace"},
		{sf::Keyboard::Tab      , "Tab"},
		{sf::Keyboard::PageUp   , "PageUp"},
		{sf::Keyboard::PageDown , "PageDown"},
		{sf::Keyboard::End      , "End"},
		{sf::Keyboard::Home     , "Home"},
		{sf::Keyboard::Insert   , "Ins"},
		{sf::Keyboard::Delete   , "Delete"},
		{sf::Keyboard::Add      , "+"},
		{sf::Keyboard::Subtract , "-"},
		{sf::Keyboard::Multiply , "*"},
		{sf::Keyboard::Divide   , "/"},
		{sf::Keyboard::Left     , "Left"},
		{sf::Keyboard::Right    , "Right"},
		{sf::Keyboard::Up       , "Up"},
		{sf::Keyboard::Down     , "Down"},
		{sf::Keyboard::Numpad0  , "Numpad0"},
		{sf::Keyboard::Numpad1  , "Numpad1"},
		{sf::Keyboard::Numpad2  , "Numpad2"},
		{sf::Keyboard::Numpad3  , "Numpad3"},
		{sf::Keyboard::Numpad4  , "Numpad4"},
		{sf::Keyboard::Numpad5  , "Numpad5"},
		{sf::Keyboard::Numpad6  , "Numpad6"},
		{sf::Keyboard::Numpad7  , "Numpad7"},
		{sf::Keyboard::Numpad8  , "Numpad8"},
		{sf::Keyboard::Numpad9  , "Numpad9"},
		{sf::Keyboard::F1       , "F1"},
		{sf::Keyboard::F2       , "F2"},
		{sf::Keyboard::F3       , "F3"},
		{sf::Keyboard::F4       , "F4"},
		{sf::Keyboard::F5       , "F5"},
		{sf::Keyboard::F6       , "F6"},
		{sf::Keyboard::F7       , "F7"},
		{sf::Keyboard::F8       , "F8"},
		{sf::Keyboard::F9       , "F9"},
		{sf::Keyboard::F10      , "F10"},
		{sf::Keyboard::F11      , "F11"},
		{sf::Keyboard::F12      , "F12"},
		{sf::Keyboard::F13      , "F13"},
		{sf::Keyboard::F14      , "F14"},
		{sf::Keyboard::F15      , "F15"},
		{sf::Keyboard::Pause    , "Pause"}
	};

	inline bool pressed (int key) {

		if (key == keyShift) return sf::Keyboard::isKeyPressed (keyLShift) || sf::Keyboard::isKeyPressed (keyRShift);
		if (key == keyCtrl)  return sf::Keyboard::isKeyPressed (keyLCtrl)  || sf::Keyboard::isKeyPressed (keyRCtrl);
		if (key == keyAlt)   return sf::Keyboard::isKeyPressed (keyLAlt)   || sf::Keyboard::isKeyPressed (keyRAlt);

		sf::Keyboard::Key skey = static_cast<sf::Keyboard::Key>(key);
		return sf::Keyboard::isKeyPressed (skey);

	}
};

#endif
