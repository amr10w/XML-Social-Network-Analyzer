add_library(core STATIC
    FileUtils.cpp
    FileUtils.h
    XMLValidator.cpp
    XMLValidator.h
)

target_link_libraries(core
    PRIVATE 
        
        Qt6::Core Qt6::Widgets
)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)