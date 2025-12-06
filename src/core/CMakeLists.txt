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
    Graph.h
    Graph.cpp
    Mutual.cpp
    Mutual.h
    GraphvizWrapper.h
    GraphVisualizer.h
    GraphVisualizer.cpp
    JsonConverter.cpp
    JsonConverter.h
)

target_link_libraries(core
    PRIVATE 
        
        Qt6::Core Qt6::Widgets
)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# --- CONDITIONAL LINKING ---
# We use the variable set in the Root CMake
if(GRAPHVIZ_FOUND)
    # 1. Find the libs
    file(GLOB GVC_LIBS "${GRAPHVIZ_DIR}/lib/*.lib")
    
    # 2. Add Include path
    target_include_directories(core PRIVATE "${GRAPHVIZ_DIR}/include")
    
    # 3. Link the libs
    target_link_libraries(core PRIVATE ${GVC_LIBS})
    
    # 4. CRITICAL: Tell C++ code that Graphviz is here
    target_compile_definitions(core PUBLIC ENABLE_GRAPHVIZ)
endif()