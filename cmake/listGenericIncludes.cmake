function(listGenericIncludes INCLUDES_TABLE_VAR)
	set(GENERIC_INCLUDES_FILES
			"${CMAKE_SOURCE_DIR}/includes/sparkle.hpp - ."
			"${CMAKE_SOURCE_DIR}/includes/spk_basic_functions.hpp - ."
			"${CMAKE_SOURCE_DIR}/includes/spk_system_define.hpp - ."

			"${CMAKE_SOURCE_DIR}/includes/application/modules/spk_API_module.hpp - NOT_COPIED"
			"${CMAKE_SOURCE_DIR}/includes/application/modules/spk_keyboard_module.hpp - application/modules"
			"${CMAKE_SOURCE_DIR}/includes/application/modules/spk_message_consumer_module.hpp - application/modules"
			"${CMAKE_SOURCE_DIR}/includes/application/modules/spk_mouse_module.hpp - application/modules"
			"${CMAKE_SOURCE_DIR}/includes/application/modules/spk_profiler_module.hpp - application/modules"
			"${CMAKE_SOURCE_DIR}/includes/application/modules/spk_system_module.hpp - application/modules"
			"${CMAKE_SOURCE_DIR}/includes/application/modules/spk_time_module.hpp - application/modules"
			"${CMAKE_SOURCE_DIR}/includes/application/modules/spk_widget_module.hpp - application/modules"

			"${CMAKE_SOURCE_DIR}/includes/application/spk_abstract_application.hpp - application"
			"${CMAKE_SOURCE_DIR}/includes/application/spk_application.hpp - application"

			"${CMAKE_SOURCE_DIR}/includes/data_structure/spk_data_buffer.hpp - data_structure"
			"${CMAKE_SOURCE_DIR}/includes/data_structure/spk_pool.hpp - data_structure"

			"${CMAKE_SOURCE_DIR}/includes/debug/spk_profiler.hpp - debug"

			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_activable_object.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_activity.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_activity_scheduler.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_command_parser.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_context_manager.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_contract_provider.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_inherence_object.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_memento.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_observer.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_singleton.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_stated_object.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_state_machine.hpp - design_pattern"
			"${CMAKE_SOURCE_DIR}/includes/design_pattern/spk_value.hpp - design_pattern"

			"${CMAKE_SOURCE_DIR}/includes/graphics/spk_pipeline.hpp - NOT_COPIED"
			"${CMAKE_SOURCE_DIR}/includes/graphics/pipeline/spk_abstract_pipeline.hpp - graphics/pipeline"
			"${CMAKE_SOURCE_DIR}/includes/graphics/pipeline/spk_default_shader.hpp - graphics/pipeline"

			"${CMAKE_SOURCE_DIR}/includes/graphics/spk_frame.hpp - NOT_COPIED"
			"${CMAKE_SOURCE_DIR}/includes/graphics/frame/spk_abstract_frame.hpp - graphics/frame"
			"${CMAKE_SOURCE_DIR}/includes/graphics/frame/spk_frame.hpp - NOT_COPIED"

			"${CMAKE_SOURCE_DIR}/includes/graphics/spk_image.hpp - NOT_COPIED"

			"${CMAKE_SOURCE_DIR}/includes/graphics/spk_surface.hpp - NOT_COPIED"
			"${CMAKE_SOURCE_DIR}/includes/graphics/surface/spk_abstract_surface.hpp - graphics/surface/"
			"${CMAKE_SOURCE_DIR}/includes/graphics/surface/spk_surface.hpp - NOT_COPIED"

			"${CMAKE_SOURCE_DIR}/includes/graphics/spk_area.hpp - graphics"
			"${CMAKE_SOURCE_DIR}/includes/graphics/spk_color.hpp - graphics"
			"${CMAKE_SOURCE_DIR}/includes/graphics/spk_mesh.hpp - graphics"
			"${CMAKE_SOURCE_DIR}/includes/graphics/spk_window.hpp - graphics"

			"${CMAKE_SOURCE_DIR}/includes/input/spk_input.hpp - input"
			"${CMAKE_SOURCE_DIR}/includes/input/spk_input_group.hpp - input"
			"${CMAKE_SOURCE_DIR}/includes/input/spk_input_status.hpp - input"
			"${CMAKE_SOURCE_DIR}/includes/input/spk_keyboard.hpp - input"
			"${CMAKE_SOURCE_DIR}/includes/input/spk_key_input.hpp - input"
			"${CMAKE_SOURCE_DIR}/includes/input/spk_mouse.hpp - input"
			"${CMAKE_SOURCE_DIR}/includes/input/spk_mouse_input.hpp - input"
			"${CMAKE_SOURCE_DIR}/includes/input/spk_mouse_motion_input.hpp - input"

			"${CMAKE_SOURCE_DIR}/includes/iostream/spk_iostream.hpp - iostream"

			"${CMAKE_SOURCE_DIR}/includes/math/perlin/spk_iperlin.hpp - math/perlin"
			"${CMAKE_SOURCE_DIR}/includes/math/perlin/spk_perlin_1D.hpp - math/perlin"
			"${CMAKE_SOURCE_DIR}/includes/math/perlin/spk_perlin_2D.hpp - math/perlin"
			"${CMAKE_SOURCE_DIR}/includes/math/perlin/spk_perlin_3D.hpp - math/perlin"
			"${CMAKE_SOURCE_DIR}/includes/math/spk_matrix4x4.hpp - math"
			"${CMAKE_SOURCE_DIR}/includes/math/spk_perlin.hpp - math"
			"${CMAKE_SOURCE_DIR}/includes/math/spk_pseudo_random_generator.hpp - math"
			"${CMAKE_SOURCE_DIR}/includes/math/spk_random_generator.hpp - math"
			"${CMAKE_SOURCE_DIR}/includes/math/spk_vector2.hpp - math"
			"${CMAKE_SOURCE_DIR}/includes/math/spk_vector3.hpp - math"

			"${CMAKE_SOURCE_DIR}/includes/miscellaneous/JSON/spk_JSON_file.hpp - miscellaneous/JSON"
			"${CMAKE_SOURCE_DIR}/includes/miscellaneous/JSON/spk_JSON_object.hpp - miscellaneous/JSON"
			"${CMAKE_SOURCE_DIR}/includes/miscellaneous/spk_is_container.hpp - miscellaneous"
			"${CMAKE_SOURCE_DIR}/includes/miscellaneous/spk_translation_atlas.hpp - miscellaneous"

			"${CMAKE_SOURCE_DIR}/includes/network/spk_network_acceptor.hpp - NOT_COPIED"
			"${CMAKE_SOURCE_DIR}/includes/network/spk_network_central_node.hpp - network"
			"${CMAKE_SOURCE_DIR}/includes/network/spk_network_client.hpp - network"
			"${CMAKE_SOURCE_DIR}/includes/network/spk_network_local_node.hpp - network"
			"${CMAKE_SOURCE_DIR}/includes/network/spk_network_message.hpp - network"
			"${CMAKE_SOURCE_DIR}/includes/network/spk_network_node.hpp - network"
			"${CMAKE_SOURCE_DIR}/includes/network/spk_network_remote_node.hpp - network"
			"${CMAKE_SOURCE_DIR}/includes/network/spk_network_server.hpp - network"
			"${CMAKE_SOURCE_DIR}/includes/network/spk_network_socket.hpp - NOT_COPIED"

			"${CMAKE_SOURCE_DIR}/includes/system/spk_chronometer.hpp - system"
			"${CMAKE_SOURCE_DIR}/includes/system/spk_timer.hpp - system"
			"${CMAKE_SOURCE_DIR}/includes/system/spk_time_metrics.hpp - system"

			"${CMAKE_SOURCE_DIR}/includes/threading/spk_persistent_worker.hpp - threading"
			"${CMAKE_SOURCE_DIR}/includes/threading/spk_promise.hpp - threading"
			"${CMAKE_SOURCE_DIR}/includes/threading/spk_thread.hpp - threading"
			"${CMAKE_SOURCE_DIR}/includes/threading/spk_thread_safe_queue.hpp - threading"
			"${CMAKE_SOURCE_DIR}/includes/threading/spk_worker_pool.hpp - threading"

			"${CMAKE_SOURCE_DIR}/includes/widget/spk_widget_activity_scheduler_manager.hpp - widget"
			"${CMAKE_SOURCE_DIR}/includes/widget/spk_widget_atlas.hpp - widget"
			"${CMAKE_SOURCE_DIR}/includes/widget/spk_widget_canvas.hpp - widget"
			"${CMAKE_SOURCE_DIR}/includes/widget/spk_widget_central_node_manager.hpp - widget"
			"${CMAKE_SOURCE_DIR}/includes/widget/spk_widget_client_manager.hpp - widget"
			"${CMAKE_SOURCE_DIR}/includes/widget/spk_widget_input_group_manager.hpp - widget"
			"${CMAKE_SOURCE_DIR}/includes/widget/spk_widget_interface.hpp - widget"
			"${CMAKE_SOURCE_DIR}/includes/widget/spk_widget_remote_node_manager.hpp - widget"
			"${CMAKE_SOURCE_DIR}/includes/widget/spk_widget_server_manager.hpp - widget"
			
			"${CMAKE_SOURCE_DIR}/includes/widget/widget_component/spk_widget_component_box.hpp - widget/widget_component"
		)

	# --- Specific Windows
	if (WIN32)
		set(GENERIC_SYSTEM_DEPENDENT_INCLUDES_FILES
				"${CMAKE_SOURCE_DIR}/includes/application/modules/windows/spk_API_module.hpp - application/modules"
				"${CMAKE_SOURCE_DIR}/includes/graphics/frame/windows/spk_frame.hpp - graphics"
				"${CMAKE_SOURCE_DIR}/includes/graphics/surface/windows/spk_surface.hpp - NOT_COPIED"
				"${CMAKE_SOURCE_DIR}/includes/network/windows/spk_network_acceptor.hpp - network"
				"${CMAKE_SOURCE_DIR}/includes/network/windows/spk_network_socket.hpp - network"
			)

	# --- Specific Linux
	elseif(UNIX)
		set(GENERIC_SYSTEM_DEPENDENT_INCLUDES_FILES
				"${CMAKE_SOURCE_DIR}/includes/application/modules/linux/spk_API_module.hpp - application/modules"
				"${CMAKE_SOURCE_DIR}/includes/graphics/frame/linux/spk_frame.hpp - graphics"
				"${CMAKE_SOURCE_DIR}/includes/graphics/surface/linux/spk_surface.hpp - NOT_COPIED"
				"${CMAKE_SOURCE_DIR}/includes/network/linux/spk_network_acceptor.hpp - network"
				"${CMAKE_SOURCE_DIR}/includes/network/linux/spk_network_socket.hpp - network"
			)

	endif()

	set(${INCLUDES_TABLE_VAR} ${${INCLUDES_TABLE_VAR}} ${GENERIC_INCLUDES_FILES} ${GENERIC_SYSTEM_DEPENDENT_INCLUDES_FILES} PARENT_SCOPE)

endfunction()