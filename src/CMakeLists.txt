add_library(cpp_lox)

target_include_directories(cpp_lox
    PUBLIC
        ${cpp_lox_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(cpp_lox
    PRIVATE
    Lox.cpp
    Token.cpp
    Scanner.cpp
)

add_executable(lox_repl)

target_include_directories(lox_repl
    PUBLIC
        ${cpp_lox_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(lox_repl
  PRIVATE
    main.cpp
)

target_link_libraries(lox_repl PRIVATE cpp_lox)