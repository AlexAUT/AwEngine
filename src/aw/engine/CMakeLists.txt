target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/engine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/performance.cpp
    ${CMAKE_CURRENT_LIST_DIR}/state.cpp
    ${CMAKE_CURRENT_LIST_DIR}/stateMachine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/window.cpp

    ${CMAKE_CURRENT_LIST_DIR}/particleSystem/system.cpp
    ${CMAKE_CURRENT_LIST_DIR}/particleSystem/renderer.cpp
    )
