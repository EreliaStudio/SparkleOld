#pragma once

#include "spk_system_define.hpp"

#include "math/spk_vector2.hpp"
#include "math/spk_vector3.hpp"
#include "math/spk_random_generator.hpp"
#include "math/spk_pseudo_random_generator.hpp"
#include "math/spk_perlin.hpp"
#include "math/spk_matrix4x4.hpp"

#include "data_structure/spk_data_buffer.hpp"
#include "data_structure/spk_pool.hpp"

#include "design_pattern/spk_singleton.hpp"
#include "design_pattern/spk_inherence_object.hpp"
#include "design_pattern/spk_command_parser.hpp"
#include "design_pattern/spk_contract_provider.hpp"
#include "design_pattern/spk_observer.hpp"
#include "design_pattern/spk_stated_object.hpp"
#include "design_pattern/spk_activable_object.hpp"
#include "design_pattern/spk_activity.hpp"
#include "design_pattern/spk_activity_scheduler.hpp"
#include "design_pattern/spk_value.hpp"
#include "design_pattern/spk_context_manager.hpp"
#include "design_pattern/spk_memento.hpp"
#include "design_pattern/spk_state_machine.hpp"

#include "design_pattern/spk_context_manager.hpp"
#include "design_pattern/spk_memento.hpp"

#include "iostream/spk_iostream.hpp"

#include "threading/spk_promise.hpp"
#include "threading/spk_thread.hpp"
#include "threading/spk_thread_safe_queue.hpp"
#include "threading/spk_worker.hpp"
#include "threading/spk_persistent_worker.hpp"

#include "system/spk_chronometer.hpp"
#include "system/spk_timer.hpp"
#include "system/spk_time_metrics.hpp"

#include "graphics/spk_color.hpp"
#include "graphics/spk_texture.hpp"
#include "graphics/spk_image.hpp"
#include "graphics/spk_sprite_sheet.hpp"
#include "graphics/spk_font.hpp"
#include "graphics/spk_window.hpp"
#include "graphics/spk_pipeline.hpp"

#include "network/spk_network_message.hpp"
#include "network/spk_network_client.hpp"
#include "network/spk_network_server.hpp"
#include "network/spk_network_node.hpp"
#include "network/spk_network_local_node.hpp"
#include "network/spk_network_remote_node.hpp"
#include "network/spk_network_central_node.hpp"

#include "application/modules/spk_API_module.hpp"
#include "application/modules/spk_time_module.hpp"
#include "application/modules/spk_graphical_api_module.hpp"
#include "application/modules/spk_mouse_module.hpp"
#include "application/modules/spk_keyboard_module.hpp"

#include "application/modules/spk_widget_module.hpp"

#include "widget/spk_widget_interface.hpp"
#include "widget/spk_widget_components.hpp"
#include "widget/spk_widget_atlas.hpp"
#include "widget/spk_widget_canvas.hpp"

#include "widget/spk_widget_client_manager.hpp"
#include "widget/spk_widget_server_manager.hpp"
#include "widget/spk_widget_central_node_manager.hpp"
#include "widget/spk_widget_remote_node_manager.hpp"

#include "application/spk_application.hpp"

#include "spk_basic_functions.hpp"

#include "input/spk_input.hpp"
#include "input/spk_mouse_motion_input.hpp"
#include "input/spk_mouse_input.hpp"
#include "input/spk_key_input.hpp"

#include "miscellaneous/JSON/spk_JSON_object.hpp"
#include "miscellaneous/JSON/spk_JSON_file.hpp"
#include "miscellaneous/spk_translation_atlas.hpp"

#include "engine/spk_components.hpp"
#include "engine/spk_game_object.hpp"
#include "engine/spk_game_engine.hpp"
#include "engine/spk_mesh_primitive.hpp"

#include "widget/spk_widget_game_engine_manager.hpp"
