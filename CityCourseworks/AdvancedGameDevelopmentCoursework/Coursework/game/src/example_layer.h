#pragma once
#include <engine.h>
#include "player.h"
#include "engine/entities/bounding_box.h"
#include "enemy.h"
#include "npc.h"

class pickup;

enum class GameState {
	Intro,
	Gameplay
};

class example_layer : public engine::layer
{
public: 
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;
	void init_timer(float countdown_time) {
		m_countdown_time = countdown_time;
		m_timer = countdown_time;
	}

private:
	int m_bankroll = 100;
	float m_countdown_time;
	float m_timer;
	glm::vec4 m_time_color;
	GameState m_gamestate = GameState::Intro;
	void check_bounce();
	void check_walking();
	//void hexagon_shape();
	//engine::ref<engine::mesh>			m_hex_mesh{};

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_terrain_1{};
	engine::ref<engine::game_object>	m_cow{};
	engine::ref<engine::game_object>	m_card{};
	engine::ref<engine::game_object>	m_bankroll_box{};
	engine::ref<engine::game_object>	m_bankroll_box_1{};
	//engine::ref<engine::game_object>	m_groscasino{};
	engine::ref<engine::game_object>	m_tree{};
	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::game_object>	m_bullet{};
	engine::ref<engine::game_object>    m_tetrahedron{};
	engine::ref<engine::game_object>	m_roulete_table{};
	engine::ref<engine::game_object>	m_blackjack_table{};
	engine::ref<engine::game_object>	m_redbuilding{};
	engine::ref<engine::game_object>	m_auto_machinegun{};
	//engine::ref<engine::game_object>	m_cuboid{};
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_npc1{};
	engine::ref<engine::game_object>	m_npc2{};
	engine::ref<engine::game_object>	m_npc3{};
	engine::ref<engine::game_object>	m_npc4{};
	player								m_player{};
	enemy								m_enemy{};
	enemy								m_enemy_bullet{};
	npc								m_enemy_npc1{};
	npc								m_enemy_npc2{};
	npc								m_enemy_npc3{};
	npc								m_enemy_npc4{};
	//player								m_npc{};

	engine::ref<engine::material>		m_material{};
	engine::ref<engine::material>		m_mannequin_material{};
	engine::ref<engine::material>		m_tetrahedron_material{};

	engine::DirectionalLight            m_directionalLight;

	engine::bounding_box				m_player_box;
	engine::bounding_box				m_redbuilding_box;
	engine::bounding_box				m_roulete_table_box;
	engine::bounding_box				m_cow_box;
	engine::bounding_box				m_auto_machinegun_box;
	engine::bounding_box				m_blackjack_table_box;
	engine::bounding_box				m_bankroll_box_box;
	engine::bounding_box				m_bankroll_box_box_1;
	bool								m_bankroll_box_render_1;
	engine::bounding_box				m_npc1_box;
	engine::bounding_box				m_bullet_box;
	//engine::bounding_box				m_npc2_box;


	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

	

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera;

	engine::ref<pickup> m_pickup{};
};
