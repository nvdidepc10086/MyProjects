#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include "engine/core/input.h"
#include "engine/entities/card.h"
#include "engine/entities/bankroll_box.h"
#include <engine/entities/shapes/tetrahedron.h>
//#include "pickup.h"

example_layer::example_layer()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())
{
	// Hide the mouse and lock it inside the window
	//engine::input::anchor_mouse(true);
	engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance(); 
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/bounce.wav", engine::sound_type::spatialised, "bounce"); // Royalty free sound from https://freesound.org/people/JustInvoke/sounds/446100/
	m_audio_manager->load_sound("assets/audio/walking.wav", engine::sound_type::track, "walking"); // Royalty free sound from https://freesound.org/people/markagames/sounds/444666/
	m_audio_manager->load_sound("assets/audio/last-time.mp3", engine::sound_type::track, "music");  // Royalty free music from https://freesound.org/people/UNIVERSFIELD/sounds/712306/
	m_audio_manager->play("music");

	//m_audio_manager->pause("music");


	// Initialise the shaders, materials and lights
	auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.25f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gColorMap", 0);
	m_directionalLight.submit(mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gMatSpecularIntensity", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("transparency", 1.0f);

	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
			(float)engine::application::window().height()));
	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);
	
	m_mannequin_material = engine::material::create(1.0f, glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	// Skybox texture from https://opengameart.org/content/night-skyboxes
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/city_ft.jpg", true),
		  engine::texture_2d::create("assets/textures/skybox/city_lf.jpg", true),
		  engine::texture_2d::create("assets/textures/skybox/city_bk.jpg", true),
		  engine::texture_2d::create("assets/textures/skybox/city_rt.jpg", true),
		  engine::texture_2d::create("assets/textures/skybox/city_up.jpg", true),
		  engine::texture_2d::create("assets/textures/skybox/city_dn.jpg", true)
		});

	/*
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/SkyboxFront.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxRight.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxBack.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxLeft.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxTop.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxBottom.bmp", true)
		});
*/

	//engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/Man_with_suit.dae");
	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh->switch_root_movement(false);

	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	mannequin_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(3.0f, 0.5f, -5.0f);
	mannequin_props.type = 0;
	//mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	mannequin_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x / 2.f, m_skinned_mesh->size().y, m_skinned_mesh->size().x / 2.f);
	m_mannequin = engine::game_object::create(mannequin_props);
	//player view change
	m_player.initialise(m_mannequin);
	m_player_box.set_box(mannequin_props.bounding_shape.x * mannequin_props.scale.x, mannequin_props.bounding_shape.y * mannequin_props.scale.x, mannequin_props.bounding_shape.z * mannequin_props.scale.x, mannequin_props.position);

	/*
	//load npc mesh
	engine::ref<engine::skinned_mesh> m_npc1_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/npcs/Ch07_nonPBR.dae");
	m_npc1_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_npc1_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_npc1_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_npc1_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_npc1_skinned_mesh->switch_root_movement(false);
	*/
	//add first npc
	//add npc with model: failed to do
	/*
	engine::ref <engine::model> NPC1_model = engine::model::create("assets/models/animated/npcs/Maria_JJ.dae");
	engine::game_object_properties NPC1_props;
	NPC1_props.meshes = NPC1_model->meshes();
	NPC1_props.textures = NPC1_model->textures();
	float NPC1_scale = 10.f / glm::max(NPC1_model->size().x, glm::max(NPC1_model->size().y, NPC1_model->size().z));
	NPC1_props.position = { 4.f,0.5f, 0.f };
	NPC1_props.scale = glm::vec3(NPC1_scale);
	NPC1_props.bounding_shape = NPC1_model->size();
	m_npc1 = engine::game_object::create(NPC1_props);
	*/
	// add npc through mannequin at red building pos
	engine::game_object_properties NPC1_props;
	NPC1_props.animated_mesh = m_skinned_mesh;
	NPC1_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	NPC1_props.position = glm::vec3(-2.f, 0.5f, 0.f);
	NPC1_props.type = 0;
	//mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	NPC1_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x / 2.f, m_skinned_mesh->size().y, m_skinned_mesh->size().x / 2.f);
	m_npc1 = engine::game_object::create(NPC1_props);
	m_enemy_npc1.initialise(m_npc1, NPC1_props.position, glm::vec3(1.f, 0.f, 0.f));
	//m_npc1_box.set_box(NPC1_props.bounding_shape.x* NPC1_props.scale.x, NPC1_props.bounding_shape.y* NPC1_props.scale.x, NPC1_props.bounding_shape.z* NPC1_props.scale.x, NPC1_props.position);

	
	
	//add second npc at machine gun pos
	engine::game_object_properties NPC2_props;
	NPC2_props.animated_mesh = m_skinned_mesh;
	NPC2_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	NPC2_props.position = glm::vec3(3.5f, 0.5f, 5.f);
	NPC2_props.type = 0;
	//mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	NPC2_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x / 2.f, m_skinned_mesh->size().y, m_skinned_mesh->size().x / 2.f);
	m_npc2 = engine::game_object::create(NPC2_props);
	m_enemy_npc2.initialise(m_npc2, NPC2_props.position, glm::vec3(1.f, 0.f, 0.f));
	//m_npc2_box.set_box(NPC2_props.bounding_shape.x* NPC2_props.scale.x, NPC2_props.bounding_shape.y* NPC2_props.scale.x, NPC2_props.bounding_shape.z* NPC2_props.scale.x, NPC2_props.position);


	//add third npc at black jack table pos
	engine::game_object_properties NPC3_props;
	NPC3_props.animated_mesh = m_skinned_mesh;
	NPC3_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	NPC3_props.position = glm::vec3(3.5f, 0.5f, 1.5f);
	NPC3_props.type = 0;
	//mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	NPC3_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x / 2.f, m_skinned_mesh->size().y, m_skinned_mesh->size().x / 2.f);
	m_npc3 = engine::game_object::create(NPC3_props);
	m_enemy_npc3.initialise(m_npc3, NPC3_props.position, glm::vec3(1.f, 0.f, 0.f));

	//add fourth npc at roulete table pos
	engine::game_object_properties NPC4_props;
	NPC4_props.animated_mesh = m_skinned_mesh;
	NPC4_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	NPC4_props.position = glm::vec3(4.5f, 0.5f, -2.f);
	NPC4_props.type = 0;
	//mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	NPC4_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x / 2.f, m_skinned_mesh->size().y, m_skinned_mesh->size().x / 2.f);
	m_npc4 = engine::game_object::create(NPC4_props);
	m_enemy_npc4.initialise(m_npc4, NPC4_props.position, glm::vec3(1.f, 0.f, 0.f));

	

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	// terrain texture from https://ambientcg.com/view?id=Marble017
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/Marble017.bmp", false) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 100.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	// load card texture and create a card mesh, create card object and set properties
	// card texture from https://sketchfab.com/3d-models/poker-set-841e271f392b4b3187b19fbdbb81ef62
	std::vector<engine::ref<engine::texture_2d>> card_textures = { engine::texture_2d::create("assets/textures/duce.bmp", false)};
	engine::ref<engine::card> card_shape = engine::card::create(0.2f, 0.4f, 0.f);
	engine::game_object_properties card_props;
	card_props.meshes = { card_shape->mesh() };
	card_props.textures = card_textures;
	card_props.is_static = true;
	card_props.position = { 8.f, 1.f, 0.f };
	card_props.type = 0;
	card_props.bounding_shape = glm::vec3(0.2f, 0.4f, 0.f);
	card_props.restitution = 1.f;
	m_card = engine::game_object::create(card_props);

	// load bankroll box texture and create a bankroll box mesh, create bankroll box object and set properties
	// bankroll box texture from https://www.bing.com/th?id=OIP.Uchf4hxIToFIMloS0vugcgHaGB&w=155&h=200&c=8&rs=1&qlt=90&o=6&dpr=1.5&pid=3.1&rm=2
	std::vector<engine::ref<engine::texture_2d>> bankroll_box_textures = { engine::texture_2d::create("assets/textures/bankroll_box.bmp", false) };
	engine::ref<engine::bankroll_box> bankroll_box_shape = engine::bankroll_box::create(0.15f, 0.15f, 0.15f); // bankroll box size
	engine::game_object_properties bankroll_box_props;
	bankroll_box_props.meshes = { bankroll_box_shape->mesh() };
	bankroll_box_props.textures = bankroll_box_textures;
	bankroll_box_props.is_static = true;
	//float bankroll_box_scale = 1.f / glm::max(bankroll_box_shape->length(), glm::max(bankroll_box_shape->width(), bankroll_box_shape->depth()));
	bankroll_box_props.position = { 3.f, 1.f, 8.f };
	//bankroll_box_props.scale = glm::vec3(bankroll_box_scale);
	bankroll_box_props.bounding_shape = glm::vec3(0.15f, 0.15f, 0.15f);
	bankroll_box_props.restitution = 1.f;
	m_bankroll_box = engine::game_object::create(bankroll_box_props);
	//m_bankroll_box_box.set_box(bankroll_box_props.bounding_shape.x* bankroll_box_scale, bankroll_box_props.bounding_shape.y* bankroll_box_scale, bankroll_box_props.bounding_shape.z* bankroll_box_scale, bankroll_box_props.position);
	m_bankroll_box_box.set_box(bankroll_box_shape->width(), bankroll_box_shape->length(), bankroll_box_shape->width(), bankroll_box_props.position);

	// load bankroll box texture and create a bankroll box mesh, create bankroll box object and set properties
	// bankroll box texture from https://www.bing.com/th?id=OIP.Uchf4hxIToFIMloS0vugcgHaGB&w=155&h=200&c=8&rs=1&qlt=90&o=6&dpr=1.5&pid=3.1&rm=2
	std::vector<engine::ref<engine::texture_2d>> bankroll_box_textures_1 = { engine::texture_2d::create("assets/textures/bankroll_box.bmp", false) };
	engine::ref<engine::bankroll_box> bankroll_box_shape_1 = engine::bankroll_box::create(0.15f, 0.15f, 0.15f); // bankroll box size
	engine::game_object_properties bankroll_box_props_1;
	bankroll_box_props_1.meshes = { bankroll_box_shape_1->mesh() };
	bankroll_box_props_1.textures = bankroll_box_textures_1;
	bankroll_box_props_1.is_static = true;
	bankroll_box_props_1.position = { 3.f, 1.f, 9.f };
	bankroll_box_props_1.bounding_shape = glm::vec3(0.15f, 0.15f, 0.15f);
	bankroll_box_props_1.restitution = 1.f;
	m_bankroll_box_1 = engine::game_object::create(bankroll_box_props_1);
	m_bankroll_box_box_1.set_box(bankroll_box_shape_1->width(), bankroll_box_shape_1->length(), bankroll_box_shape_1->width(), bankroll_box_props_1.position);

	// Load the cow model. Create a cow object. Set its properties
	engine::ref <engine::model> cow_model = engine::model::create("assets/models/static/cow4.3ds");
	engine::game_object_properties cow_props;
	cow_props.meshes = cow_model->meshes();
	cow_props.textures = cow_model->textures();
	float cow_scale = 1.f / glm::max(cow_model->size().x, glm::max(cow_model->size().y, cow_model->size().z));
	cow_props.position = { 10.f,0.5f, 0.f };
	cow_props.scale = glm::vec3(cow_scale);
	cow_props.bounding_shape = cow_model->size();
	m_cow = engine::game_object::create(cow_props);
	m_enemy.initialise(m_cow, cow_props.position, glm::vec3(1.f, 0.f, 0.f));
	m_cow->set_offset(cow_model->offset());
	m_cow_box.set_box(cow_props.bounding_shape.x* cow_scale * 2.5f, cow_props.bounding_shape.y* cow_scale, cow_props.bounding_shape.z* cow_scale * .5f, { 2.f, 0.5f, 0.f });
	
	//load the auto308 machine gun model. create object and set properties
	// from https://sketchfab.com/3d-models/308-machine-gun-4ca5f52eef6244629770c56752b67583 03-11-2023
	engine::ref<engine::model> auto_machinegun_model = engine::model::create("assets/models/static/Auto308.obj");
	engine::game_object_properties auto_machinegun_props;
	auto_machinegun_props.meshes = auto_machinegun_model->meshes();
	auto_machinegun_props.textures = auto_machinegun_model->textures();
	float auto_machinegun_scale = 0.5f / glm::max(auto_machinegun_model->size().x, glm::max(auto_machinegun_model->size().y, auto_machinegun_model->size().z));
	//auto_machinegun_props.position = { 3.f,0.5f, 5.f };
	auto_machinegun_props.position = NPC2_props.position;
	auto_machinegun_props.scale = glm::vec3(auto_machinegun_scale);
	auto_machinegun_props.bounding_shape = auto_machinegun_model->size();
	m_auto_machinegun = engine::game_object::create(auto_machinegun_props);
	m_auto_machinegun->set_offset(auto_machinegun_model->offset());
	m_auto_machinegun_box.set_box(auto_machinegun_props.bounding_shape.x* auto_machinegun_scale, auto_machinegun_props.bounding_shape.y* auto_machinegun_scale, auto_machinegun_props.bounding_shape.z* auto_machinegun_scale, auto_machinegun_props.position);
	// Load the tree model. Create a tree object. Set its properties
	/*
	engine::ref <engine::model> tree_model = engine::model::create("assets/models/static/elm.3ds");
	engine::game_object_properties tree_props;
	tree_props.meshes = tree_model->meshes();
	tree_props.textures = tree_model->textures();
	float tree_scale = 3.f / glm::max(tree_model->size().x, glm::max(tree_model->size().y, tree_model->size().z));
	tree_props.position = { 4.f, 0.5f, -5.f };
	tree_props.bounding_shape = tree_model->size() / 2.f * tree_scale;
	tree_props.scale = glm::vec3(tree_scale);
	m_tree = engine::game_object::create(tree_props);
	*/

	//load roulete table model, from https://sketchfab.com/3d-models/roulette-table-f276b7fc198e4556932c7b52761e409c
	engine::ref<engine::model> roulete_table_model = engine::model::create("assets/models/static/roulete_table2.obj");
	engine::game_object_properties roulete_table_props;
	roulete_table_props.meshes = roulete_table_model->meshes();
	roulete_table_props.textures = roulete_table_model->textures();
	float roulete_table_scale = 1.f;
	roulete_table_props.position = { 1.f, 0.f, 0.f };
	roulete_table_props.bounding_shape = roulete_table_model->size() / 4.f;
	roulete_table_props.scale = glm::vec3(roulete_table_scale) / 4.f;
	m_roulete_table = engine::game_object::create(roulete_table_props);

	m_roulete_table->set_offset(roulete_table_model->offset());
	m_roulete_table_box.set_box(roulete_table_props.bounding_shape.x* roulete_table_scale, roulete_table_props.bounding_shape.y* roulete_table_scale, roulete_table_props.bounding_shape.z* roulete_table_scale, {3.5f, 0.5f, -2.5f});

	
	//load black jack table model, from https://sketchfab.com/3d-models/casino-poker-table-f36fc75d825148618aa6e5cbfb43f28e
	engine::ref<engine::model> blackjack_table_model = engine::model::create("assets/models/static/blackjack_table_1.obj");
	engine::game_object_properties balckjack_table_props;
	balckjack_table_props.meshes = blackjack_table_model->meshes();
	balckjack_table_props.textures = blackjack_table_model->textures();
	float blackjack_table_scale = 0.5f;
	balckjack_table_props.position = { 3.f, 0.5f, 1.5f };
	balckjack_table_props.scale = glm::vec3(blackjack_table_scale);
	balckjack_table_props.bounding_shape = blackjack_table_model->size();
	m_blackjack_table = engine::game_object::create(balckjack_table_props);
	m_blackjack_table->set_offset(blackjack_table_model->offset());
	m_blackjack_table_box.set_box(balckjack_table_props.bounding_shape.x* blackjack_table_scale, balckjack_table_props.bounding_shape.y* blackjack_table_scale, balckjack_table_props.bounding_shape.z* blackjack_table_scale, balckjack_table_props.position);

	
	
	//Load red building model , from https://opengameart.org/content/red-building
	engine::ref<engine::model> redbuilding_model = engine::model::create("assets/models/static/building1.obj");
	engine::game_object_properties redbuilding_props;
	redbuilding_props.meshes = redbuilding_model->meshes();
	redbuilding_props.textures = redbuilding_model->textures();
	//model size
	float redbuilding_scale = 1.f;
	//float redbuilding_scale = 1.f / glm::max(redbuilding_model->size().x, glm::max(redbuilding_model->size().y, redbuilding_model->size().z));
	//float groscasino_scale = 1.f / glm::max(groscasino_model->size().x, glm::max(groscasino_model->size().y, groscasino_model->size().z));
	redbuilding_props.position = { 3.f, 1.f, 0.f };
	//redbuilding_props.bounding_shape = redbuilding_model->size() / 2.f * redbuilding_scale;
	redbuilding_props.bounding_shape = redbuilding_model->size();
	redbuilding_props.scale = glm::vec3(redbuilding_scale);
	m_redbuilding = engine::game_object::create(redbuilding_props);
	//add collision box to object
	m_redbuilding->set_offset(redbuilding_model->offset());
	m_redbuilding_box.set_box(redbuilding_props.bounding_shape.x* redbuilding_scale / 1.f - 4.f, redbuilding_props.bounding_shape.y* redbuilding_scale / 1.5f, redbuilding_props.bounding_shape.z* redbuilding_scale + 2.f, {-10.f, 0.5f, 0.f});

	//pause the music before load items with sound to load in objects with sound successfully
	m_audio_manager->pause("music");
	m_audio_manager->play("walking");
	m_audio_manager->loop("walking", 20);

	// add tetrahedron object
	std::vector<glm::vec3> tetrahedron_vertices;
	tetrahedron_vertices.push_back(glm::vec3(0.f, 5.f, 0.f));	//0
	tetrahedron_vertices.push_back(glm::vec3(0.f, 0.f, 5.f));	//1
	tetrahedron_vertices.push_back(glm::vec3(-5.f, 0.f, -5.f));	//2
	tetrahedron_vertices.push_back(glm::vec3(5.f, 0.f, -5.f));	//3


	engine::ref<engine::tetrahedron> tetrahedron_shape = engine::tetrahedron::create(tetrahedron_vertices);
	engine::game_object_properties tetrahedron_props;
	tetrahedron_props.position = { 0.f, 0.5f, -20.f };
	tetrahedron_props.meshes = { tetrahedron_shape->mesh() };
	// texture from https://ambientcg.com/view?id=Tiles130
	std::vector<engine::ref<engine::texture_2d>> tetrahedron_textures = { engine::texture_2d::create("assets/textures/Tiles1.bmp", false) };
	tetrahedron_props.textures = tetrahedron_textures;
	m_tetrahedron = engine::game_object::create(tetrahedron_props);

	//new bullet
	engine::ref<engine::sphere> bullet_shape = engine::sphere::create(10, 20, 0.05f);
	engine::game_object_properties bullet_props;
	bullet_props.position = auto_machinegun_props.position;
	bullet_props.meshes = { bullet_shape->mesh() };
	bullet_props.type = 1;
	bullet_props.bounding_shape = glm::vec3(0.5f);
	bullet_props.restitution = 0.92f;
	bullet_props.mass = 0.000001f;
	m_bullet = engine::game_object::create(bullet_props);
	//m_enemy_bullet.initialise(m_bullet, bullet_props.position, glm::vec3(1.f, 0.f, 0.f));
	m_bullet_box.set_box(0.1f, 0.1f, 0.1f, bullet_props.position);

	//new ball with bounce sound
	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(10, 20, 0.5f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { 0.f, 5.f, -5.f };
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.000001f;
	m_ball = engine::game_object::create(sphere_props);

	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_card);
	m_game_objects.push_back(m_bankroll_box);
	m_game_objects.push_back(m_bankroll_box_1);
	m_game_objects.push_back(m_ball);
	//m_game_objects.push_back(m_bullet);
	//m_game_objects.push_back(m_cow);
	//m_game_objects.push_back(m_tree);
	//m_game_objects.push_back(m_pickup);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();

	m_skinned_mesh->switch_animation(1);
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step)
{
	if (m_gamestate == GameState::Intro) {

	}
	else if (m_gamestate == GameState::Gameplay) {

		glm::vec3 pos = m_player.object()->position();
		glm::vec3 enemy_pos = m_enemy.object()->position();
		//glm::vec3 enemy_bullet_pos = m_enemy_bullet.object()->position();
		glm::vec3 npc1_enemy_pos = m_enemy_npc1.object()->position();
		glm::vec3 npc2_enemy_pos = m_enemy_npc2.object()->position();
		//glm::vec3 auto308_pos = m_enemy_npc2.object()->position();
		glm::vec3 npc3_enemy_pos = m_enemy_npc3.object()->position();
		glm::vec3 npc4_enemy_pos = m_enemy_npc4.object()->position();
		glm::vec3 auto_machinegun_box_pos = glm::vec3(npc2_enemy_pos.x, npc2_enemy_pos.y + 0.5f, npc2_enemy_pos.z);
		glm::vec3 bullet_box_pos = glm::vec3(npc2_enemy_pos.x + 0.2f, npc2_enemy_pos.y + 0.5f, npc2_enemy_pos.z);

		/*
		//no third person
		m_3d_camera.on_update(time_step);
		m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));
		m_mannequin->animated_mesh()->on_update(time_step);
		*/

		m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

		//third person view
		m_player.on_update(time_step);
		m_player.update_camera(m_3d_camera);

		//update boxes positon
		m_player_box.on_update(m_player.object()->position());
		m_enemy.on_update(time_step, m_player.object()->position());
		//bullet enemy
		//m_enemy_bullet.on_update(time_step, bullet_box_pos);
		m_enemy_npc1.on_update(time_step, m_player.object()->position());
		m_enemy_npc2.on_update(time_step, m_player.object()->position());
		m_auto_machinegun_box.on_update(auto_machinegun_box_pos);
		m_bullet_box.on_update(bullet_box_pos);
		m_enemy_npc3.on_update(time_step, m_player.object()->position());
		m_enemy_npc4.on_update(time_step, m_player.object()->position());
		m_cow_box.on_update(m_enemy.object()->position());
		//m_npc2_box.on_update(m_npc.object()->position());
		if (m_redbuilding_box.collision(m_player_box)) {
			m_player.object()->set_position(pos);
		}
		else if (m_roulete_table_box.collision(m_player_box)) {
			m_player.object()->set_position(pos);
		}
		else if (m_cow_box.collision(m_player_box)) {
			m_player.object()->set_position(pos);
			m_enemy.object()->set_position(enemy_pos);
		}
		else if (m_blackjack_table_box.collision(m_player_box)) {
			m_player.object()->set_position(pos);
		}
		else if (m_auto_machinegun_box.collision(m_player_box)) {
			m_player.object()->set_position(pos);
		}
		else if (m_bankroll_box_box.collision(m_player_box)) {
			m_player.object()->set_position(pos);
			m_bankroll += 100;
		}
		else if (m_bankroll_box_box_1.collision(m_player_box)) {
			//m_player.object()->set_position(pos);
			m_bankroll_box_box_1.on_update(glm::vec3(100.f, 100.f, 100.f));
			m_bankroll += 100;
			m_bankroll_box_render_1 = false;
		}
		else if (m_bullet_box.collision(m_player_box)) {
			m_player.object()->set_position(pos);
			/*
			if (m_bankroll >= 400) {
				m_enemy_bullet.object()->set_position(enemy_bullet_pos);
			}
			*/
		}

		m_audio_manager->update_with_camera(m_3d_camera);

		m_timer -= time_step;
		//time texutre color update
		if (m_timer <= 0.f) {
			m_timer = 120.f;
		}
		else if (m_timer <= 120.f && m_timer > 30.f) {
			m_time_color = glm::vec4(0.05f, 0.9f, 0.05f, 1.f);
		}
		else if (m_timer <= 30.f) {
			m_time_color = glm::vec4(1.f, 0.f, 0.f, 1.f);
		}

		check_bounce();
	}
}

void example_layer::on_render()
{
	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();

	if (m_gamestate == GameState::Intro) {
		const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
		m_text_manager->render_text(text_shader, "No More Gambling", (float)engine::application::window().width() / 2 - 400.f, (float)engine::application::window().height() - 150.f, 2.0f, glm::vec4(255.f, 0.f, 127.f, 1.f));
		m_text_manager->render_text(text_shader, "Press Enter to Start", (float)engine::application::window().width() / 2, (float)engine::application::window().height() / 3, 1.0f, glm::vec4(1.f, 1.f, 1.f, 1.f));
		m_text_manager->render_text(text_shader, "W: Walk Forward", (float)engine::application::window().width() / 2 - 600.f, (float)engine::application::window().height() / 2, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));
		m_text_manager->render_text(text_shader, "S: Walk Backward", (float)engine::application::window().width() / 2 - 600.f, (float)engine::application::window().height() / 2 - 50.f, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));
		m_text_manager->render_text(text_shader, "A: Camera turn left", (float)engine::application::window().width() / 2 - 600.f, (float)engine::application::window().height() / 2 - 100.f, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));
		m_text_manager->render_text(text_shader, "D: Camera turn right", (float)engine::application::window().width() / 2 - 600.f, (float)engine::application::window().height() / 2 - 150.f, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));
		m_text_manager->render_text(text_shader, "Space: Jump", (float)engine::application::window().width() / 2 - 600.f, (float)engine::application::window().height() / 2 - 200.f, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));
		//m_text_manager->render_text(text_shader, "E: ...", (float)engine::application::window().width() / 2 - 600.f, (float)engine::application::window().height() / 2 - 250.f, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_gamestate == GameState::Gameplay) {

		// Set up  shader. (renders textures and materials)
		const auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
		const auto textured_material_shader = engine::renderer::shaders_library()->get("textured_material");
		engine::renderer::begin_scene(m_3d_camera, mesh_shader);

		// Set up some of the scene's parameters in the shader
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

		// Enable fog effect
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_on", true);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_colour", glm::vec3(0.5f, 0.5f, 0.5f)); // Set fog color to grey
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_factor_type", 2); // Example: exponential squared fog
		//std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("rho", 0.03f); // Example density for exponential fog

		// Position the skybox centred on the player and render it
		glm::mat4 skybox_tranform(1.0f);
		skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
		for (const auto& texture : m_skybox->textures())
		{
			texture->bind();
		}
		engine::renderer::submit(mesh_shader, m_skybox, skybox_tranform);

		engine::renderer::submit(mesh_shader, m_terrain);

		engine::renderer::submit(mesh_shader, m_card);
		engine::renderer::submit(mesh_shader, m_bankroll_box);
		engine::renderer::submit(mesh_shader, m_bankroll_box_1, m_bankroll_box_render_1);

		//render boxes
		m_player_box.on_render(2.5f, 0.f, 0.f, mesh_shader, true);
		m_redbuilding_box.on_render(2.5f, 0.f, 0.f, mesh_shader, true);
		m_roulete_table_box.on_render(2.5f, 0.f, 0.f, mesh_shader, true);
		m_cow_box.on_render(2.5f, 0.f, 0.f, mesh_shader, true);
		m_blackjack_table_box.on_render(2.5f, 0.f, 0.f, mesh_shader, true);
		m_auto_machinegun_box.on_render(2.5f, 2.5f, 2.5f, mesh_shader, true);
		m_bullet_box.on_render(3.f, 1.f, 9.f, mesh_shader, true);
		m_bankroll_box_box.on_render(2.5f, 0.f, 0.f, mesh_shader, true);
		m_bankroll_box_box_1.on_render(2.5f, 0.f, 0.f, mesh_shader, m_bankroll_box_render_1);
		//m_npc1_box.on_render(2.5f, 0.f, 0.f, mesh_shader);
		//m_npc2_box.on_render(2.5f, 0.f, 0.f, mesh_shader);

		//new no mesh texture
		engine::renderer::submit(mesh_shader, m_tetrahedron);

		/*
		glm::mat4 tree_transform(1.0f);
		tree_transform = glm::translate(tree_transform, glm::vec3(8.f, 0.5, -5.0f));
		tree_transform = glm::rotate(tree_transform, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform = glm::scale(tree_transform, m_tree->scale());
		engine::renderer::submit(mesh_shader, tree_transform, m_tree);
		*/

		glm::mat4 cow_transform(1.0f);
		cow_transform = glm::translate(cow_transform, m_cow->position() - glm::vec3(m_cow->offset().x, 0.f, m_cow->offset().z) * m_cow->scale().x);
		cow_transform = glm::rotate(cow_transform, m_cow->rotation_amount(), m_cow->rotation_axis());
		cow_transform = glm::scale(cow_transform, m_cow->scale());
		engine::renderer::submit(mesh_shader, cow_transform, m_cow);


		glm::mat4 NPC1_transform(1.0f);
		NPC1_transform = glm::translate(NPC1_transform, m_npc1->position() - glm::vec3(m_cow->offset().x, 0.f, m_npc1->offset().z) * m_npc1->scale().x);
		NPC1_transform = glm::rotate(NPC1_transform, m_npc1->rotation_amount(), m_npc1->rotation_axis());
		NPC1_transform = glm::scale(NPC1_transform, m_npc1->scale());
		engine::renderer::submit(mesh_shader, NPC1_transform, m_npc1);

		//bullet
		glm::vec3 bullet_position = glm::vec3(m_npc2->position().x + 0.2f, m_npc2->position().y + 0.55f, m_npc2->position().z);
		glm::mat4 bullet_tranform(1.0f);
		bullet_tranform = glm::translate(bullet_tranform, bullet_position - glm::vec3(m_bullet->offset().x, 0.f, m_bullet->offset().z) * m_bullet->scale().x);
		bullet_tranform = glm::rotate(bullet_tranform, m_npc2->rotation_amount(), m_bullet->rotation_axis());
		bullet_tranform = glm::scale(bullet_tranform, m_bullet->scale());
		engine::renderer::submit(mesh_shader, bullet_tranform, m_bullet);

		//new machine gun model
		glm::mat4 auto_machinegun_transform(1.0f);
		auto_machinegun_transform = glm::translate(auto_machinegun_transform, glm::vec3(m_npc2->position().x, m_npc2->position().y + 0.5f, m_npc2->position().z) - glm::vec3(m_auto_machinegun->offset().x, -0.5f, m_auto_machinegun->offset().z) * m_auto_machinegun->scale().x);
		//auto_machinegun_transform = glm::rotate(auto_machinegun_transform, glm::pi<float>(), m_auto_machinegun->rotation_axis());
		auto_machinegun_transform = glm::rotate(auto_machinegun_transform, -m_npc2->rotation_amount(), m_auto_machinegun->rotation_axis());
		auto_machinegun_transform = glm::scale(auto_machinegun_transform, m_auto_machinegun->scale());
		engine::renderer::submit(mesh_shader, auto_machinegun_transform, m_auto_machinegun);


		// new roulete table
		glm::mat4 roulete_table_transform(1.0f);
		roulete_table_transform = glm::translate(roulete_table_transform, m_roulete_table->position() - glm::vec3(m_roulete_table->offset().x - 10.f, -1.5f, m_roulete_table->offset().z + 10.f) * m_roulete_table->scale().x);
		roulete_table_transform = glm::rotate(roulete_table_transform, m_roulete_table->rotation_amount(), m_roulete_table->rotation_axis());
		roulete_table_transform = glm::scale(roulete_table_transform, m_roulete_table->scale());
		engine::renderer::submit(mesh_shader, roulete_table_transform, m_roulete_table);


		// new blackjack table
		glm::mat4 blackjack_table_transform(1.0f);
		blackjack_table_transform = glm::translate(blackjack_table_transform, m_blackjack_table->position() - glm::vec3(m_blackjack_table->offset().x, 0.f, m_blackjack_table->offset().z) * m_blackjack_table->scale().x);
		blackjack_table_transform = glm::rotate(blackjack_table_transform, m_blackjack_table->rotation_amount(), m_blackjack_table->rotation_axis());
		//blackjack_table_transform = glm::rotate(blackjack_table_transform, -glm::pi<float>()/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		blackjack_table_transform = glm::scale(blackjack_table_transform, m_blackjack_table->scale());
		engine::renderer::submit(mesh_shader, blackjack_table_transform, m_blackjack_table);


		// red building
		glm::mat4 redbuilding_tranform(1.0f);
		redbuilding_tranform = glm::translate(redbuilding_tranform, m_redbuilding->position() - glm::vec3(m_redbuilding->offset().x + 14.f, -4.5f, m_redbuilding->offset().z - 1.5f) * m_redbuilding->scale().x);
		redbuilding_tranform = glm::rotate(redbuilding_tranform, glm::pi<float>() / 2.f, glm::vec3(0.f, 1.f, 0.f));
		redbuilding_tranform = glm::scale(redbuilding_tranform, m_redbuilding->scale());
		engine::renderer::submit(mesh_shader, redbuilding_tranform, m_redbuilding);

		

		/*
		//regular hexagon shape
		glm::mat4 hex_transform(1.0f);
		hex_transform = glm::translate(hex_transform, glm::vec3(0.f, 0.f, 5.f));
		engine::renderer::submit(textured_material_shader, m_hex_mesh, hex_transform);
		*/
		/*
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", ture);
		m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_pickup->position());
		pickup_transform = glm::rotate(pickup_tranform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		engine::renderer::submit(mesh_shader, m_pickup->meshes().at(0), pickup_transform);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
		*/





		m_material->submit(mesh_shader);
		engine::renderer::submit(mesh_shader, m_ball);
		//engine::renderer::submit(mesh_shader, m_bullet);

		m_mannequin_material->submit(mesh_shader);

		//new no mesh texture
		//m_tetrahedron_material->submit(mesh_shader);



		//player view change
		//engine::renderer::submit(mesh_shader, m_mannequin);
		engine::renderer::submit(mesh_shader, m_player.object());



		//rander four npc
		//engine::renderer::submit(mesh_shader, m_npc1);
		engine::renderer::submit(mesh_shader, m_npc2);
		engine::renderer::submit(mesh_shader, m_npc3);
		engine::renderer::submit(mesh_shader, m_npc4);

		engine::renderer::end_scene();

		// Render text
		const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
		std::string bankroll_text = "Bank Roll: " + std::to_string(static_cast<int>(m_bankroll)) + "$";
		m_text_manager->render_text(text_shader, bankroll_text, 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		// Render timer
		std::string timer_text = "Time left: " + std::to_string(static_cast<int>(m_timer));
		m_text_manager->render_text(text_shader, timer_text, 10.f, (float)engine::application::window().height() - 50.f, 0.5f, m_time_color);
		//m_text_manager->render_text(text_shader, "100$", 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	}
}

void example_layer::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);

		if (m_gamestate == GameState::Intro && e.key_code() == engine::key_codes::KEY_ENTER) {
			m_gamestate = GameState::Gameplay;
			//m_audio_manager->play("music");
		}
		else if (m_gamestate == GameState::Gameplay) {

			if (e.key_code() == engine::key_codes::KEY_TAB)
			{
				engine::render_command::toggle_wireframe();
			}
		}
	}
}

void example_layer::check_bounce()
{
	if (m_prev_sphere_y_vel < 0.1f && m_ball->velocity().y > 0.1f)
		//m_audio_manager->play("bounce");
		m_audio_manager->play_spatialised_sound("bounce", m_3d_camera.position(), glm::vec3(m_ball->position().x, 0.f, m_ball->position().z));
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}

/*
//regular hexagon shape
void example_layer::hexagon_shape() {
	std::vector<engine::mesh::vertex> hex_vertices;
	for (int i = 0; i < 6; i++) {
		float theta = (i / 6.0f) * 2 * engine::PI;
		engine::mesh::vertex vertex(
			glm::vec3(cos(theta), sin(theta), 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec2(cos(theta) / 2.0f + 0.25f, sin(theta) / 2.0f + 0.25f)
		);
		hex_vertices.push_back(vertex);
	}
	std::vector<uint32_t> hex_indices{
		0, 1, 2,
		2, 3, 0,
		3, 4, 5,
		5, 0, 3
	};
	m_hex_mesh = engine::mesh::create(hex_vertices, hex_indices);
}
*/
