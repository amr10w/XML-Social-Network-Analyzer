#ifndef GRAPHVIZWRAPPER_H
#define GRAPHVIZWRAPPER_H

// This Macro is defined in src/core/CMakeLists.txt ONLY if library is found
#ifdef ENABLE_GRAPHVIZ
    #include <graphviz/gvc.h>
    #include <graphviz/cgraph.h>
    #include <graphviz/cdt.h>
#endif

#endif // GRAPHVIZWRAPPER_H