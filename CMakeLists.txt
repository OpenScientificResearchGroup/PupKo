cmake_minimum_required(VERSION 3.15)
project(PupKo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PupKoBuildTests "Build PupKo tests" ON)
option(PupKoBuildExamples "Build PupKo examples" ON)

include(GNUInstallDirs)

add_library(PupKoCore
    src/Engine.cpp
    src/Session.cpp
    src/PupKoCApi.cpp
    src/Tokenizer/CommandLineTokenizer.cpp
    src/Router/RouterTypes.cpp
    src/Router/SemanticRouter.cpp
    src/Provider/SchemaProvider.cpp
    src/Provider/HistoryProvider.cpp
    src/Provider/AgentToolProvider.cpp
    src/Provider/CompositeProvider.cpp
    src/Ranker/CandidateRanker.cpp)

target_include_directories(PupKoCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(PupKoCore PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(PupKoCore PRIVATE /W4)
else()
    target_compile_options(PupKoCore PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_library(PupKo::Core ALIAS PupKoCore)

if(PupKoBuildTests)
    enable_testing()
    add_subdirectory(tests)
endif()

if(PupKoBuildExamples)
    add_executable(CommandLineDemo examples/CommandLineDemo.cpp)
    target_include_directories(CommandLineDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(CommandLineDemo PRIVATE PupKoCore)
endif()
