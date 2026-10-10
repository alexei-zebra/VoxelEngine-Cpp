project(VoxelWindowBackends)

# Input state shared by the window backends.
add_library(VoxelWindowBase STATIC BaseInput.cpp)

target_compile_options(VoxelWindowBase PRIVATE ${VOXEL_COMPILE_OPTIONS})

target_link_libraries(VoxelWindowBase PRIVATE VoxelCoreInterface)

add_library(VoxelGLFW STATIC GLFWWindow.cpp)

target_compile_options(VoxelGLFW PRIVATE ${VOXEL_COMPILE_OPTIONS})

target_link_libraries(VoxelGLFW PRIVATE VoxelWindowBase VoxelCoreInterface)
