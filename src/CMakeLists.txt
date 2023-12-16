add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        core/engine.cpp
        core/engine.h
        core/window.cpp
        core/window.h
        core/vertex.h
        core/deltatime.cpp
        core/deltatime.h
        ui/editor.cpp
        ui/editor.h
        render/renderer.h
        render/gl.cpp
        render/gl.h
        entities/entity.cpp
        entities/entity.h
        entities/entitymanager.cpp
        entities/entitymanager.h
        components/component.h
        components/transform.cpp
        components/transform.h
        components/mesh.h
        components/camera.cpp
        components/camera.h
        player/playermanager.cpp
        player/playermanager.h
        tools/modelimport.cpp
        tools/modelimport.h
        tools/scrollingbuffer.h)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>")

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${CMAKE_PROJECT_NAME} glad glm imgui implot SDL2 assimp Optick::OptickCore ${CMAKE_DL_LIBS})