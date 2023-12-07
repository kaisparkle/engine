add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        core/engine.cpp
        core/engine.h
        core/window.cpp
        core/window.h
        ui/imgui.cpp
        ui/imgui.h
        render/renderer.h
        render/gl.cpp
        render/gl.h
        entities/entity.cpp
        entities/entity.h
        entities/entitymanager.cpp
        entities/entitymanager.h
        components/component.h
        components/transform.cpp
        components/transform.h)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>")

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${CMAKE_PROJECT_NAME} glad glm imgui SDL2 ${CMAKE_DL_LIBS})