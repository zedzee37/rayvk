project('rayvk', 'cpp')

src_files = [
  'src/main.cpp',
  'src/renderer.cpp',
  'src/swapchain.cpp',
  'src/device.cpp',
  'src/image.cpp',
]

include_dirs = include_directories('include')

# Find Vulkan
vulkan_dep = dependency('vulkan', required: true)

# Find GLFW
glfw_dep = dependency('glfw3', required: true)

# Create executable
executable(
  'rayvk',
  src_files,
  include_directories: include_dirs,
  dependencies: [vulkan_dep, glfw_dep],
)
