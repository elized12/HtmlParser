cmake_minimum_required(VERSION 3.10)
project(HtmlParser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB PARSER_SRC src/parser/*.cpp)
file(GLOB TOKENIZATOR_SRC src/tokenizator/*.cpp)

set(HTMLPARSER_SRC
    ${PARSER_SRC}
    ${TOKENIZATOR_SRC}
)

add_library(htmlparser STATIC ${HTMLPARSER_SRC})

target_include_directories(htmlparser PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/parser>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/tokenizator>
)