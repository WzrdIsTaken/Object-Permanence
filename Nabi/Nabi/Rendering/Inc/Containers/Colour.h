#pragma once
#include "EngineCore.h"

#include <array>

#include "DirectXIncludes.h"

namespace nabi::Rendering
{
	/// <summary>
	/// A basic colour struct 
	/// Uses unsigned chars because they have a range of 0-255. Pretty neat for colours!
	/// </summary>
	struct Colour
	{
		template<typename T>
		using ColourFormat = typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, float>::value || std::is_same<T, unsigned char>::value>::type;

		enum class ColourRange
		{
			O_TO_255,
			O_TO_1,
			ENUM_COUNT
		}; // Todo - If I find out what the actual names of these ranges are, replace!

		Colour();
		Colour(int const r, int const g, const int b);
		Colour(int const r, const int g, int const b, int const a);
		Colour(float const r, float const g, float const b);
		Colour(float const r, float const g, float const b, float const a);
		Colour(unsigned char const r, unsigned char const g, unsigned char const b);
		Colour(unsigned char const r, unsigned char const g, unsigned char const b, unsigned char const a);

		/// <summary>
		/// Convenience method for colour -> float3 
		/// </summary>
		[[nodiscard]] explicit operator dx::XMFLOAT3() const;

		/// <summary>
		/// Converts the colour to an array (eg, useful for direct x)
		/// </summary>
		/// <typeparam name="T">The type to convert the colour array to. Limited to ints, floats and unsigned chars</typeparam>
		/// <param name="colourRange">- The format of the returned colour (see Colour::ColourRange)</param>
		/// <returns>The array converted to type T</returns>
		template<typename T, typename = ColourFormat<T>>
		[[nodiscard]] std::array<T, 4> ToArray(ColourRange const colourRange) const
		{
			std::array<T, 4> colour
			{
				static_cast<T>(m_R),
				static_cast<T>(m_G),
				static_cast<T>(m_B),
				static_cast<T>(m_A)
			};

			if (colourRange == ColourRange::O_TO_1)
			{
				colour[0] /= static_cast<T>(255);
				colour[1] /= static_cast<T>(255);
				colour[2] /= static_cast<T>(255);
				colour[3] /= static_cast<T>(255);
			}

			return colour;
		}

		/// <summary>
		/// Converts an array of rgba values to a colour. Note: Does not currently support ColourRange
		/// </summary>
		/// <typeparam name="colourArray">- The colour's rgba values in an array</typeparam>
		/// <returns>The colour</returns>
		template<typename T, typename = ColourFormat<T>>
		[[nodiscard]] static Colour FromArray(std::array<T, 4> const& colourArray)
		{
			return Colour(colourArray[0], colourArray[1], colourArray[2], colourArray[3]);
		}

		unsigned char m_R;
		unsigned char m_G;
		unsigned char m_B;
		unsigned char m_A;
	};

	namespace Colours
	{
		Colour const AliceBlue{ 240, 248, 255 };
		Colour const AntiqueWhite{ 250, 235, 215 };
		Colour const Aquamarine{ 127, 255, 212 };
		Colour const Azure{ 240, 255, 255 };
		Colour const Beige{ 245, 245, 220 };
		Colour const Bisque{ 255, 228, 196 };
		Colour const Black{ 0, 0, 0 };
		Colour const BlanchedAlmond{ 255, 235, 205 };
		Colour const Blue{ 0, 0, 255 };
		Colour const BlueViolet{ 138, 43, 226 };
		Colour const Brown{ 165, 42, 42 };
		Colour const BurlyWood{ 222, 184, 135 };
		Colour const CadetBlue{ 95, 158, 160 };
		Colour const Chartreuse{ 127, 255, 0 };
		Colour const Chocolate{ 210, 105, 30 };
		Colour const Coral{ 255, 127, 80 };
		Colour const CornflowerBlue{ 100, 149, 237 };
		Colour const Cornsilk{ 255, 248, 220 };
		Colour const Crimson{ 220, 20, 60 };
		Colour const Cyan{ 0, 255, 255 };
		Colour const DarkBlue{ 0, 0, 139 };
		Colour const DarkCyan{ 0, 139, 139 };
		Colour const DarkGoldenrod{ 184, 134, 11 };
		Colour const DarkGrey{ 169, 169, 169 };
		Colour const DarkGreen{ 0, 100, 0 };
		Colour const DarkKhaki{ 189, 183, 107 };
		Colour const DarkMagenta{ 139, 0, 139 };
		Colour const DarkOliveGreen{ 85, 107, 47 };
		Colour const DarkOrange{ 255, 140, 0 };
		Colour const DarkOrchid{ 153, 50, 204 };
		Colour const DarkRed{ 139, 0, 0 };
		Colour const DarkSalmon{ 233, 150, 122 };
		Colour const DarkSeaGreen{ 143, 188, 143 };
		Colour const DarkSlateBlue{ 72, 61, 139 };
		Colour const DarkSlateGrey{ 47, 79, 79 };
		Colour const DarkTurquoise{ 0, 206, 209 };
		Colour const DarkViolet{ 148, 0, 211 };
		Colour const DeepPink{ 255, 20, 147 };
		Colour const DeepSkyBlue{ 0, 191, 255 };
		Colour const DimGrey{ 105, 105, 105 };
		Colour const DodgerBlue{ 30, 144, 255 };
		Colour const FireBrick{ 178, 34, 34 };
		Colour const FloralWhite{ 255, 250, 240 };
		Colour const ForestGreen{ 34, 139, 34 };
		Colour const Gainsboro{ 220, 220, 220 };
		Colour const GhostWhite{ 248, 248, 255 };
		Colour const Gold{ 255, 215, 0 };
		Colour const Goldenrod{ 218, 165, 32 };
		Colour const Grey{ 128, 128, 128 };
		Colour const Green{ 0, 128, 0 };
		Colour const GreenYellow{ 173, 255, 47 };
		Colour const Honeydew{ 240, 255, 240 };
		Colour const HotPink{ 255, 105, 180 };
		Colour const IndianRed{ 205, 92, 92 };
		Colour const Indigo{ 75, 0, 130 };
		Colour const Ivory{ 255, 255, 240 };
		Colour const Khaki{ 240, 230, 140 };
		Colour const Lavender{ 230, 230, 250 };
		Colour const LavenderBlush{ 255, 240, 245 };
		Colour const LawnGreen{ 124, 252, 0 };
		Colour const LemonChiffon{ 255, 250, 205 };
		Colour const LightBlue{ 173, 216, 230 };
		Colour const LightCoral{ 240, 128, 128 };
		Colour const LightCyan{ 224, 255, 255 };
		Colour const LightGoldenrodYellow{ 250, 250, 210 };
		Colour const LightGrey{ 211, 211, 211 };
		Colour const LightGreen{ 144, 238, 144 };
		Colour const LightPink{ 255, 182, 193 };
		Colour const LightSalmon{ 255, 160, 122 };
		Colour const LightSeaGreen{ 32, 178, 170 };
		Colour const LightSkyBlue{ 135, 206, 250 };
		Colour const LightSlateGrey{ 119, 136, 153 };
		Colour const LightSteelBlue{ 176, 196, 222 };
		Colour const LightYellow{ 255, 255, 224 };
		Colour const Lime{ 0, 255, 0 };
		Colour const LimeGreen{ 50, 205, 50 };
		Colour const Linen{ 250, 240, 230 };
		Colour const Magenta{ 255, 0, 255 };
		Colour const Maroon{ 128, 0, 0 };
		Colour const MediumAquamarine{ 102, 205, 170 };
		Colour const MediumBlue{ 0, 0, 205 };
		Colour const MediumOrchid{ 186, 85, 211 };
		Colour const MediumPurple{ 147, 112, 219 };
		Colour const MediumSeaGreen{ 60, 179, 113 };
		Colour const MediumSlateBlue{ 123, 104, 238 };
		Colour const MediumSpringGreen{ 0, 250, 154 };
		Colour const MediumTurquoise{ 72, 209, 204 };
		Colour const MediumVioletRed{ 199, 21, 133 };
		Colour const MidnightBlue{ 25, 25, 112 };
		Colour const MintCream{ 245, 255, 250 };
		Colour const MistyRose{ 255, 228, 225 };
		Colour const Moccasin{ 255, 228, 181 };
		Colour const NavajoWhite{ 255, 222, 173 };
		Colour const Navy{ 0, 0, 128 };
		Colour const OldLace{ 253, 245, 230 };
		Colour const Olive{ 128, 128, 0 };
		Colour const OliveDrab{ 107, 142, 35 };
		Colour const Orange{ 255, 165, 0 };
		Colour const OrangeRed{ 255, 69, 0 };
		Colour const Orchid{ 218, 112, 214 };
		Colour const PaleGoldenrod{ 238, 232, 170 };
		Colour const PaleGreen{ 152, 251, 152 };
		Colour const PaleTurquoise{ 175, 238, 238 };
		Colour const PaleVioletRed{ 219, 112, 147 };
		Colour const PapayaWhip{ 255, 239, 213 };
		Colour const PeachPuff{ 255, 218, 185 };
		Colour const Peru{ 205, 133, 63 };
		Colour const Pink{ 255, 192, 203 };
		Colour const Plum{ 221, 160, 221 };
		Colour const PowderBlue{ 176, 224, 230 };
		Colour const Purple{ 128, 0, 128 };
		Colour const RebeccaPurple{ 102, 51, 153 };
		Colour const Red{ 255, 0, 0 };
		Colour const RosyBrown{ 188, 143, 143 };
		Colour const RoyalBlue{ 65, 105, 225 };
		Colour const SaddleBrown{ 139, 69, 19 };
		Colour const Salmon{ 250, 128, 114 };
		Colour const SandyBrown{ 244, 164, 96 };
		Colour const SeaGreen{ 46, 139, 87 };
		Colour const SeaShell{ 255, 245, 238 };
		Colour const Sienna{ 160, 82, 45 };
		Colour const Silver{ 192, 192, 192 };
		Colour const SkyBlue{ 135, 206, 235 };
		Colour const SlateBlue{ 106, 90, 205 };
		Colour const SlateGrey{ 112, 128, 144 };
		Colour const Snow{ 255, 250, 250 };
		Colour const SpringGreen{ 0, 255, 127 };
		Colour const SteelBlue{ 70, 130, 180 };
		Colour const Tan{ 210, 180, 140 };
		Colour const Teal{ 0, 128, 128 };
		Colour const Thistle{ 216, 191, 216 };
		Colour const Tomato{ 255, 99, 71 };
		Colour const Turquoise{ 64, 224, 208 };
		Colour const Violet{ 238, 130, 238 };
		Colour const Wheat{ 245, 222, 179 };
		Colour const White{ 255, 255, 255 };
		Colour const WhiteSmoke{ 245, 245, 245 };
		Colour const Yellow{ 255, 255, 0 };
		Colour const YellowGreen{ 154, 205, 50 };
		//
		Colour const Clear{ 0, 0, 0, 0 };
	} // namespace nabi::Rendering::Colours
} // namespace nabi::Rendering
