set(CMAKE_AUTORCC ON)

add_library(UI_Pages STATIC
    WelcomePage.cpp
    WelcomePage.h
    WelcomePage.ui
    TeamPage.cpp      
    TeamPage.h       
    TeamPage.ui       
    resources.qrc
    ProcessingPage.cpp
    ProcessingPage.h
    ProcessingPage.ui
    Aboutpage.cpp
    Aboutpage.h
    Aboutpage.ui
    VerificationPage.h
    VerificationPage.cpp
    VerificationPage.ui
    formattingPage.cpp
    formattingPage.h
    formattingPage.ui
    minifyingPage.cpp
    minifyingPage.h
    minifyingPage.ui
    json_converter_page.ui
    json_converter_page.h
    json_converter_page.cpp
    CompressPage.cpp
    CompressPage.h
    CompressPage.ui
    postsearch.cpp
    postsearch.h
    social_relations_page.cpp
    social_relations_page.h
    social_relations_page.ui
    graphpage.cpp
    graphpage.h
    graphpage.ui
    influence_activity.h
    influence_activity.cpp
    influence_activity_page.ui

)


set_property(TARGET UI_Pages PROPERTY AUTORCC ON)

target_link_libraries(UI_Pages
    PRIVATE
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(UI_Pages
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)