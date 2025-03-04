cmake_minimum_required(VERSION 3.14)
project(sysnotify)

set(CMAKE_CXX_STANDARD 17)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Define source files
set(SOURCES src/main.cpp src/version.cpp)

set(MY_BINARY_DIR bin)


# Generate Qt Resource File
qt_add_resources(RESOURCES resources.qrc)

# Create Executable
add_executable(sysnotify ${SOURCES} ${RESOURCES})
add_executable(sysnotify_debug ${SOURCES} ${RESOURCES})

# Link Qt6 Libraries
target_link_libraries(sysnotify Qt6::Widgets)
target_link_libraries(sysnotify_debug Qt6::Widgets)

# Set Output Directory
set_target_properties(sysnotify_debug PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    
set_target_properties(sysnotify PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Define Debug Build with Preprocessor Define
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_MODE -D_DEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE_MODE -D_NDEBUG")

# Define Debug & Release Targets
add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target sysnotify_debug
)

add_custom_target(release
    COMMAND ${CMAKE_COMMAND} -DRELEASE -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target sysnotify
)

# Install target - Copy binary to /usr/bin
install(TARGETS sysnotify DESTINATION /usr/bin)

# Define Clean Rule (Deletes temporary files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
    "qrc_resources.o;main.o.ocmake_install.cmake;CMakeCache.txt;qrc_resources.cpp;resources.qrc.depends"
)
