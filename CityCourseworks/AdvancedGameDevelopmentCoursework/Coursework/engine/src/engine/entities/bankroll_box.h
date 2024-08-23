#pragma once
#include "shapes/cuboid.h"

namespace engine
{
	class cuboid;

	/// \brief Class creating a card object containing a cuboid shape of a specified size
	class bankroll_box : public cuboid
	{
	public:
		/// \brief Constructor
		bankroll_box(float length, float depth, float width);
		/// \brief Destructor
		~bankroll_box();

		/// \brief Getter methods
		float width() const { return m_width; }
		float length() const { return m_length; }
		float depth() const { return m_depth; }

		static ref<bankroll_box> create(float length, float depth, float width);
	private:
		/// \brief Fields
		// bankroll_box's width
		float m_width;
		// bankroll_box's depth
		float m_depth;
		// bankroll_box's length
		float m_length;
	};
}
