# ---------------------------------------------------------------------------
# QO Executables
# ---------------------------------------------------------------------------
find_package(PythonInterp REQUIRED)

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------

list(APPEND TEST_SRC
        tests/tester.cpp
        tests/tasks/task1.cpp
        tests/tasks/task2.cpp
        tests/tasks/task3.cpp
        tests/tasks/task4.cpp
        tests/tasks/task5.cpp
        tests/TestChi.cpp
        tests/TestJoin.cpp
        tests/TestScan.cpp
        tests/TestSelect.cpp
)
list(APPEND TEST_SRC) #append your own tests here

# ---------------------------------------------------------------------------
# Executables
# ---------------------------------------------------------------------------

add_executable(tester ${TEST_SRC})
target_link_libraries(tester ${QOLIB_LINK_LIBRARIES} Catch2::Catch2WithMain)
set_target_properties(tester PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

add_test(NAME Tests COMMAND tester WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

#add_executable(testgenerator "tests/testgenerator.cpp")
#target_link_libraries(testgenerator qolib catch)
#set_target_properties(testgenerator PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

add_custom_target(check
        COMMAND $<TARGET_FILE:tester>
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        )

add_dependencies(check tester)


# ---------------------------------------------------------------------------
# Linting
# ---------------------------------------------------------------------------

#add_tidy_target(lint_tools "${TESTS_SRC}")
#list(APPEND lint_targets lint_tools)

