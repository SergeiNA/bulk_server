cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(bulk_server VERSION 7.7.7)
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework system REQUIRED)

set (LIB_FILES 
                async_queue.h       async_queue.cpp 
                async_adaptor.h     async_adaptor.cpp 
                command_handler.h   command_handler.cpp
                observer.h          observer.cpp 
                Math.h              Math.cpp 
                metrics.h           metrics.cpp 
                threadManager.h     threadManager.cpp
                connectionManager.h connectionManager.cpp
                )


set(SERVER_FILES main.cpp 
                 server.h server.cpp
                ${LIB_FILES})

set(TEST_FILES 
                test_asyncAdaptor.cpp
                test_cmdhandler_nested.cpp
                test_cmdhandler_regular.cpp)

add_library(async SHARED ${LIB_FILES})

add_executable(bulk_server ${SERVER_FILES})

set(TESTS test_async_adaptor 
          test_async_cmdQ_nested
          test_async_cmdQ_regular)

list(LENGTH TESTS len_TESTS_r)
math(EXPR len_TESTS "${len_TESTS_r} - 1")

foreach(i RANGE ${len_TESTS})
    list(GET TESTS ${i} test_name)
    list(GET TEST_FILES ${i} test_file)

    add_executable(
        ${test_name}
        ${test_file}
    ) 

endforeach(i)

set(CMP_OPTIONS "-pthread;-Wpedantic;-Wall;-Wextra")
# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(async PUBLIC -DDEBUG)  # -D removed
    target_compile_definitions(bulk_server PUBLIC -DDEBUG)  # -D removed
    list(APPEND CMP_OPTIONS "-g")
    list(APPEND CMP_OPTIONS "-fsanitize=thread")
    # list(APPEND CMP_OPTIONS "-fsanitize=address")
    # list(APPEND CMP_OPTIONS "-fsanitize=leak")
    message("debug")
endif()

set_target_properties(async bulk_server  ${TESTS} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "${CMP_OPTIONS}"
    LINK_FLAGS "-fsanitize=thread"
    # LINK_FLAGS "-fsanitize=address"
    # LINK_FLAGS "-fsanitize=leak"
)

set_target_properties(${TESTS} PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)


target_link_libraries(bulk_server
    Threads::Threads
    ${Boost_LIBRARIES}
)

foreach(i RANGE ${len_TESTS})
    list(GET TESTS ${i} element)
    message(${element})

    target_link_libraries(
        ${element}
        async
        Threads::Threads
        ${Boost_LIBRARIES}
    )   

endforeach(i)

install(TARGETS bulk_server RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT nikseas@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()
add_test(${TESTS} ${TESTS})
