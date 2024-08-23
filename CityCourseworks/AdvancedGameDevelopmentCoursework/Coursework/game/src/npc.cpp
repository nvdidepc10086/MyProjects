#include "pch.h"
#include "npc.h"

npc::npc() {}

npc::~npc() {}

void npc::initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward)
{
	m_object = object;
	m_object->set_forward(forward);
	m_object->set_position(position);
}

void npc::on_update(const engine::timestep& time_step, const glm::vec3& player_position)
{
	float distance_to_player = glm::distance(m_object->position(), player_position);

	// check which state is the enemy in, then execute the matching behaviour
	if (m_state == state::idle)
	{
		patrol(time_step);
	}
	
}

// move forwards until the timer runs out, then switch direction to move the other way
void npc::patrol(const engine::timestep& time_step)
{
	m_switch_direction_timer -= (float)time_step;
	if (m_switch_direction_timer < 0.f)
	{
		m_object->set_forward(m_object->forward() * -1.f);
		m_switch_direction_timer = m_default_time;
	}

	m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}

