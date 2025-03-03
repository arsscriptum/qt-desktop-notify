cmake_minimum_required(VERSION 3.14)
project(qt-desktop-notify)

set(CMAKE_CXX_STANDARD 17)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Define source files
set(SOURCES src/main.cpp)
#set(SOURCES src/main.cpp qrc_resources.cpp)
# Generate Qt Resource File
qt_add_resources(RESOURCES resources.qrc)

# Create Executable
add_executable(qt-desktop-notify ${SOURCES} ${RESOURCES})

# Link Qt6 Libraries
target_link_libraries(qt-desktop-notify Qt6::Widgets)

# Set Output Directory
set_target_properties(qt-desktop-notify PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Define Clean Rule (Deletes temporary files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
    "qrc_resources.cpp;resources.qrc.depends"
)
