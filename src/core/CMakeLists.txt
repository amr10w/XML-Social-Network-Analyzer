add_library(core STATIC
    FileUtils.cpp
    FileUtils.h
    XMLValidator.cpp
    XMLValidator.h
    XMLMinifier.h
    XMLMinifier.cpp
    Tree.h
    SimpleMap.h
    BPE.h
    BPE.cpp
    XML_Compress.h
    XML_Compress.cpp
    XML_Decompress.h
    XML_Decompress.cpp
)

target_link_libraries(core
    PRIVATE 
        
        Qt6::Core Qt6::Widgets
)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)