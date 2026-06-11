cmake_minimum_required(VERSION 3.12)

set(NAME "argsi") # Cambiar solo el nombre del external en cada carpeta
set(SRC_FILES "${NAME}.c")

add_library(${NAME} MODULE ${SRC_FILES})
target_include_directories(${NAME} PRIVATE . "${PDPATH}")

if(MSVC)
    target_link_libraries(${NAME} PRIVATE "${PDLPATH}/pd.lib")
    target_compile_options(${NAME} PRIVATE /O2 /Oi /Ot /fp:fast)
    set_target_properties(${NAME} PROPERTIES PREFIX "" SUFFIX ".dll")
    target_link_options(${NAME} PRIVATE "/EXPORT:${NAME}_setup")
else()
    target_compile_options(${NAME} PRIVATE -O3 -ffast-math -funroll-loops -fomit-frame-pointer -Wall -Wextra -Wshadow -Wstrict-aliasing)
    if(APPLE)
        set_target_properties(${NAME} PROPERTIES PREFIX "" SUFFIX ".pd_darwin")
        target_link_options(${NAME} PRIVATE "-undefined" "dynamic_lookup")
    else()
        set_target_properties(${NAME} PROPERTIES PREFIX "" SUFFIX ".pd_linux")
    endif()
endif()

add_custom_command(TARGET ${NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${NAME}>" "${DEST}/"
    COMMENT "Copiando ${NAME} a la carpeta central de externals..."
)
