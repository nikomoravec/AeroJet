add_library(AeroJetClassTree ${AEROJET_LIB_TYPE}
    include/Node.hpp
    source/Node.cpp
    include/ClassInfoNode.hpp
    source/ClassInfoNode.cpp
)

target_include_directories(AeroJetClassTree PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(AeroJetClassTree PUBLIC
    AeroJet
)
