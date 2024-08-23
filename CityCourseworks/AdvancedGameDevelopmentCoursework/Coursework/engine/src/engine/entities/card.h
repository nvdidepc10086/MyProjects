#pragma once
#include "shapes/cuboid.h"

namespace engine
{
	class cuboid;

	/// \brief Class creating a card object containing a cuboid shape of a specified size
	class card : public cuboid
	{
	public:
		/// \brief Constructor
		card(float length, float depth, float width);
		/// \brief Destructor
		~card();

		/// \brief Getter methods
		float width() const { return m_width; }
		float length() const { return m_length; }
		float depth() const { return m_depth; }

		static ref<card> create(float length, float depth, float width);
	private:
		/// \brief Fields
		// card's width
		float m_width;
		// card's depth
		float m_depth;
		// card's length
		float m_length;
	};
}
