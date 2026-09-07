cmake_minimum_required(VERSION 4.3.3 FATAL_ERROR)

set(PROJECT_NAME MyFirstGame)
project(${PROJECT_NAME})


add_executable(${PROJECT_NAME} 
    src/main.cpp

    src/Resources/ResourceManager.cpp
    src/Resources/ResourceManager.h
    src/Resources/stb_image.h

    src/Renderer/Texture2D.h
    src/Renderer/Texture2D.cpp
    src/Renderer/Sprite.h
    src/Renderer/Sprite.cpp
    src/Renderer/getTileUV.h
    src/Renderer/ShaderProgram.cpp
    src/Renderer/ShaderProgram.h

    src/GamePlay/Bullet.h
    src/GamePlay/Bullet.cpp
    src/GamePlay/Tank.h
    src/GamePlay/Tank.cpp
    src/GamePlay/Tile.h
    src/GamePlay/Water.h
    src/GamePlay/Water.cpp
    src/GamePlay/Bush.h
    src/GamePlay/Bush.cpp
    src/GamePlay/Wall.h
    src/GamePlay/Wall.cpp
    src/GamePlay/GameMap.h
    src/GamePlay/GameMap.cpp
    src/GamePlay/TankController.h
    src/GamePlay/TankController.cpp
    src/GamePlay/InputData.h
    src/GamePlay/Explosion.h
    src/GamePlay/Explosion.cpp

    src/Utils/WorldPosition.h
    src/Utils/Collision.h
    src/Utils/InputManager.h
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_include_directories(${PROJECT_NAME} PRIVATE external/glfw/include)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

target_include_directories(${PROJECT_NAME} PRIVATE external/glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_target(CopyRes ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)
add_dependencies(${PROJECT_NAME} CopyRes)
