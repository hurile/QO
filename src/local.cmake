# ---------------------------------------------------------------------------
# QO Src
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Files
# ---------------------------------------------------------------------------
                   
list(APPEND SRC_CC
        src/Admin.cpp
        src/Attribute.cpp
        src/Database.cpp
        src/Homework.cpp
        src/HomeworkLogic.cpp
        src/JoinQuery.cpp
        src/QueryGraph.cpp
        src/Register.cpp
        src/Table.cpp
        )
list(APPEND SRC_CC
        src/operator/Chi.cpp
        src/operator/CrossProduct.cpp
        src/operator/Groupby.cpp
        src/operator/HashJoin.cpp
        src/operator/Indexscan.cpp
        src/operator/Operator.cpp
        src/operator/Printer.cpp
        src/operator/Projection.cpp
        src/operator/Selection.cpp
        src/operator/Tablescan.cpp
        )
list(APPEND SRC_CC  ) #append your own sources to the list
                   
if(EXISTS "${PROJECT_SOURCE_DIR}/src/muster/local.cmake")
   include("${PROJECT_SOURCE_DIR}/src/muster/local.cmake")
endif()

# ---------------------------------------------------------------------------
# Library
# ---------------------------------------------------------------------------

add_library(qolib STATIC ${SRC_CC} ${INCLUDE_H})
set(QOLIB_LINK_LIBRARIES $<TARGET_OBJECTS:qolib> $<TARGET_PROPERTY:qolib,INTERFACE_LINK_LIBRARIES>)

# ---------------------------------------------------------------------------
# Linting
# ---------------------------------------------------------------------------

#add_tidy_target(lint_src "${SRC_CC}")
#list(APPEND lint_targets lint_src)
