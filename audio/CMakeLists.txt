cmake_minimum_required(VERSION 3.12)
project(ExternalsAudio)

#cada uno tiene que cambiar aqui lo que haga falta en su computadora y sistema operativo/entorno
if(WIN32)
    set(DEFAULT_PDPATH "C:/Program Files/Pd/src")
    set(DEFAULT_PDLPATH "C:/Program Files/Pd/bin") # 
elseif(APPLE)
    set(DEFAULT_PDPATH "/Applications/Pd-0.55-0.app/Contents/Resources/src")
    set(DEFAULT_PDLPATH "")
else()
    set(DEFAULT_PDPATH "/usr/include/pd")
    set(DEFAULT_PDLPATH "")
endif()


get_filename_component(RAIZ_SUPERIOR "${CMAKE_SOURCE_DIR}" DIRECTORY)
set(DEST "${RAIZ_SUPERIOR}/externals" CACHE PATH "Destino de los externals")
set(PDPATH "${DEFAULT_PDPATH}" CACHE PATH "Ruta a m_pd.h")
set(PDLPATH "${DEFAULT_PDLPATH}" CACHE PATH "Ruta a las librerías (Solo Windows)")

#proyectos individuales
add_subdirectory(pan~)
add_subdirectory(my_blp~)
add_subdirectory(my_tabread4~)
add_subdirectory(reson~)