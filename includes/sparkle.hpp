#pragma once

#include "spk_system_define.hpp"

#include "math/spk_vector2.hpp"
#include "math/spk_vector3.hpp"
#include "math/spk_random_generator.hpp"
#include "math/spk_perlin.hpp"

#include "data_structure/spk_data_buffer.hpp"
#include "data_structure/spk_pool.hpp"

#include "debug/spk_profiler.hpp"

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
#include "threading/spk_worker_pool.hpp"
#include "threading/spk_persistent_worker.hpp"

#include "system/spk_chronometer.hpp"
#include "system/spk_timer.hpp"
#include "system/spk_time_metrics.hpp"

#include "application/spk_abstract_application.hpp"

#include "graphics/spk_window.hpp"

#include "network/spk_message.hpp"
#include "network/spk_client.hpp"
#include "network/spk_server.hpp"

#include "application/modules/spk_API_module.hpp"
#include "application/modules/spk_time_module.hpp"
#include "application/modules/spk_window_module.hpp"
#include "application/modules/spk_mouse_module.hpp"
#include "application/modules/spk_keyboard_module.hpp"

#include "application/modules/spk_widget_module.hpp"

#include "widget/spk_abstract_widget.hpp"
#include "widget/spk_widget_atlas.hpp"

#include "widget/spk_client_manager.hpp"
#include "widget/spk_server_manager.hpp"

#include "application/spk_application.hpp"

#include "spk_basic_functions.hpp"

#include "miscellaneous/JSON/spk_JSON_object.hpp"
#include "miscellaneous/JSON/spk_JSON_file.hpp"
#include "miscellaneous/spk_translation_atlas.hpp"
