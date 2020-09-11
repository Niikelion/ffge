cmake_minimum_required(VERSION 3.9.2)

project(ffge VERSION 2.0.0 DESCRIPTION "Graphics engine")

find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(freeimage REQUIRED)

set(SOURCES
	src/basics.cpp
	src/buffer.cpp
	src/camera.cpp
	src/font.cpp
	src/frame.cpp
	src/material.cpp
	src/mesh.cpp
	src/obj.cpp
	src/object.cpp
	src/renderer.cpp
	src/scene.cpp
	src/shader.cpp
	src/sprite.cpp
	src/text.cpp
	src/texture.cpp
	src/uniform.cpp
)

set(HEADERS
	include/ffge/basics.hpp
	include/ffge/buffer.hpp
	include/ffge/camera.hpp
	include/ffge/ff_glew.hpp
	include/ffge/ffge.hpp
	include/ffge/font.hpp
	include/ffge/frame.hpp
	include/ffge/ftguard.hpp
	include/ffge/material.hpp
	include/ffge/mesh.hpp
	include/ffge/obj.hpp
	include/ffge/object.hpp
	include/ffge/renderer.hpp
	include/ffge/scene.hpp
	include/ffge/shader.hpp
	include/ffge/sprite.hpp
	include/ffge/text.hpp
	include/ffge/texture.hpp
	include/ffge/uniform.hpp
)

add_library(ffge ${SOURCES} ${HEADERS})
target_include_directories(ffge PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    	$<INSTALL_INTERFACE:./include>)
target_include_directories(ffge PRIVATE include/ffge)
target_link_libraries(ffge PUBLIC freeimage::FreeImage glm GLEW::GLEW freetype)

add_library(ffge::ffge ALIAS ffge)

install(TARGETS ffge
        EXPORT ffgeTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        )


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ffgeConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

install(EXPORT ffgeTargets
        FILE ffgeTargets.cmake
        NAMESPACE ffge::
        DESTINATION lib/cmake/ffge
         )
		 
configure_file(ffgeConfig.cmake.in ffgeConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ffgeConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/ffgeConfigVersion.cmake"
        DESTINATION lib/cmake/ffge
        )
		
export(TARGETS ffge NAMESPACE ffge:: FILE ffgeTargets.cmake)