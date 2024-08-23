#include "pch.h"
#include "card.h"

engine::card::card(float length, float depth, float width) : cuboid(glm::vec3(length, depth, width), false), m_length(length), m_depth(depth), m_width(width) {}

engine::card::~card() {}

engine::ref<engine::card> engine::card::create(float length, float depth, float width)
{
	return std::make_shared<engine::card>(length, depth, width);
}
