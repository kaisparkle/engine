add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        asset/asset.h
        asset/assetmanager.cpp
        asset/assetmanager.h
        asset/model.h
        core/engine.cpp
        core/engine.h
        core/window.cpp
        core/window.h
        core/vertex.h
        core/mesh.h
        core/texture.h
        core/deltatime.cpp
        core/deltatime.h
        ui/editor.cpp
        ui/editor.h
        render/renderer.h
        render/framebuffer.h
        render/gl/apiobjects.h
        render/gl/renderer.cpp
        render/gl/renderer.h
        render/gl/framebuffer.cpp
        render/gl/framebuffer.h
        entity/entity.cpp
        entity/entity.h
        entity/entitymanager.cpp
        entity/entitymanager.h
        component/component.h
        component/transform.cpp
        component/transform.h
        component/model.cpp
        component/model.h
        component/camera.cpp
        component/camera.h
        component/flycontroller.cpp
        component/flycontroller.h
        player/playermanager.cpp
        player/playermanager.h
        tools/modelimport.cpp
        tools/modelimport.h
        tools/scrollingbuffer.h)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>")

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${CMAKE_PROJECT_NAME} glad glm imgui implot SDL2 assimp Optick::OptickCore stb ${CMAKE_DL_LIBS})