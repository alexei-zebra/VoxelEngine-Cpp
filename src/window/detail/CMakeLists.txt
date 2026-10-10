project(VoxelWindowBackends)

# Input state shared by the window backends.
add_library(VoxelWindowBase STATIC BaseInput.cpp)

target_include_directories(VoxelWindowBase PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_options(VoxelWindowBase PRIVATE ${VOXEL_COMPILE_OPTIONS})

add_library(VoxelGLFW STATIC GLFWWindow.cpp)

target_include_directories(VoxelGLFW PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_options(VoxelGLFW PRIVATE ${VOXEL_COMPILE_OPTIONS})

target_link_libraries(
    VoxelGLFW
    PRIVATE VoxelWindowBase
            glfw
            OpenGL::GL
            GLEW::GLEW
            glm::glm)
