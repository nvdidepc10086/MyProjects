#include "pch.h"
#include "bankroll_box.h"

engine::bankroll_box::bankroll_box(float length, float depth, float width) : cuboid(glm::vec3(length, depth, width), false), m_length(length), m_depth(depth), m_width(width) {}

engine::bankroll_box::~bankroll_box() {}

engine::ref<engine::bankroll_box> engine::bankroll_box::create(float length, float depth, float width)
{
	return std::make_shared<engine::bankroll_box>(length, depth, width);
}
