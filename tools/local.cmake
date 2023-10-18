# ---------------------------------------------------------------------------
# QO Executables
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------

list(APPEND TOOLS_SRC
        tools/AdminTool.cpp
        )
list(APPEND TOOLS_SRC) #append your own targets here

# ---------------------------------------------------------------------------
# Executables
# ---------------------------------------------------------------------------

foreach (examplesourcefile ${TOOLS_SRC})
    get_filename_component(examplename_base ${examplesourcefile} NAME)
    string(REPLACE ".cpp" "" examplename ${examplename_base})
    add_executable(${examplename} ${examplesourcefile})
    target_link_libraries(${examplename} ${QOLIB_LINK_LIBRARIES})
    set_target_properties(${examplename} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endforeach (examplesourcefile ${TOOLS_SRC})
