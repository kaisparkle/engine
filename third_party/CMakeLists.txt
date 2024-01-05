add_library(glad STATIC)
target_sources(glad PRIVATE glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

add_library(imgui STATIC)
target_sources(imgui PRIVATE
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_sdl2.cpp
)

target_include_directories(imgui PUBLIC imgui PUBLIC imgui/backends)
target_link_libraries(imgui PUBLIC ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})

add_library(implot STATIC)
target_sources(implot PRIVATE
        implot/implot.cpp
        implot/implot_demo.cpp
        implot/implot_items.cpp)

target_include_directories(implot PUBLIC implot)
target_link_libraries(implot PUBLIC imgui)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb)

add_subdirectory(assimp)

add_subdirectory(JoltPhysics/Build)