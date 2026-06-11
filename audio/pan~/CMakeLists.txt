cmake_minimum_required(VERSION 3.12)

set(PD_NAME "pan~") 
set(SRC_FILES "pan~.c") # Nombre real de tu archivo .c

#Reemplazo de tilde NADA MAS QUE EN EL CASO DE WINDOWS uffff
string(REPLACE "~" "_tilde" TARGET_NAME "${PD_NAME}")

add_library(${TARGET_NAME} MODULE ${SRC_FILES})
target_include_directories(${TARGET_NAME} PRIVATE . "${PDPATH}")

# aqui cada uno tiene que adaptar de acuerdo con su SO y herramientas
if(MSVC)
    # --- ENTORNO WINDOWS (MSVC) ---
    target_link_libraries(${TARGET_NAME} PRIVATE "${PDLPATH}/pd.lib")
    target_compile_options(${TARGET_NAME} PRIVATE /O2 /Oi /Ot /fp:fast)
    target_compile_definitions(${TARGET_NAME} PRIVATE MSW NT PD PD_LONGINTTYPE=__int64 _CRT_SECURE_NO_WARNINGS)
    
    set_target_properties(${TARGET_NAME} PROPERTIES 
        OUTPUT_NAME "${PD_NAME}"
        PREFIX "" 
        SUFFIX ".dll"
    )
    target_link_options(${TARGET_NAME} PRIVATE "/EXPORT:${TARGET_NAME}_setup")
else()
    # --- ENTORNO LINUX / MAC (GCC o Clang) ---
    target_compile_options(${TARGET_NAME} PRIVATE -O3 -ffast-math -funroll-loops -fomit-frame-pointer -Wall -Wextra -Wshadow -Wstrict-aliasing)
    if(APPLE)
        set_target_properties(${TARGET_NAME} PROPERTIES 
            OUTPUT_NAME "${PD_NAME}"
            PREFIX "" 
            SUFFIX ".pd_darwin"
        )
        target_link_options(${TARGET_NAME} PRIVATE "-undefined" "dynamic_lookup")
    else()
        set_target_properties(${TARGET_NAME} PROPERTIES 
            OUTPUT_NAME "${PD_NAME}"
            PREFIX "" 
            SUFFIX ".pd_linux"
        )
    endif()
endif()

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${TARGET_NAME}>" "${DEST}/"
    COMMENT "Copiando ${PD_NAME} a la carpeta central de externals..."
)