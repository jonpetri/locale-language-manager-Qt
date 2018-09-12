cmake_minimum_required(VERSION 3.4.3)

# Project's name
project (localeLanguageManagerQtDemo)
MESSAGE(STATUS " ")#Display an empty line

##
# Which compiler do we use?
#
    MESSAGE(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
    MESSAGE(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

##
# Build  type set up: debug or release
#
    set_property(
        CACHE CMAKE_BUILD_TYPE
        PROPERTY STRINGS
        Debug Release )

    if( "${CMAKE_BUILD_TYPE}" STREQUAL "" )
        set( CMAKE_BUILD_TYPE "Debug" )
    endif()


    MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

##
# if  Project_Use_build_time_share_files_and_settings = ON  ->  the aplication will read the data files  in AppDir/../data
#
    option(Project_Use_build_time_share_files_and_settings "The application use shared files (translations...) and settings located in AppDir/../data" ON)
    if ( CMAKE_BUILD_TYPE MATCHES Release AND Project_Use_build_time_share_files_and_settings)

         MESSAGE(WARNING "Realease build with use of build time share files")
    ELSEIF ( CMAKE_BUILD_TYPE MATCHES Debug AND NOT(Project_Use_build_time_share_files_and_settings))
         MESSAGE(WARNING "Debug build without use of build time share files")
    endif()

##
# Compiler configuration according build type
#
    set(CMAKE_CXX_STANDARD 11)      # setting the language standard, same as add_definitions(-std=c++14)

    if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
      # using GCC
        #add_definitions(-std=c++14)

        if( CMAKE_BUILD_TYPE MATCHES Release )
            add_definitions(-O2 -s)
        else ()
            add_definitions(-pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal
            -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion
            -Wshadow -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default
            -Wswitch-enum -Wundef -Wlogical-op -Winline)
            #add_definitions(-Werror)
        endif()

    #elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      # using Clang
    #elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
      # using Intel C++
    #elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
      # using Visual Studio C++
      #add_definitions(/EHc-)
       ##add_definitions(-DUNICODE -D_UNICODE)
    endif()


##
# Output directories
#

    # The directories build_products/sOutputDir contains only the output files: exe, dll, so, doc, translations... without any cache file, makefile...
    # Directory crated below:
    #       build_tagets/debug
    #       build_tagets/release
    # build_tagets location (Project_Targets_dir) is in the cache, so can be modified by the user
    # sOutputDir = build_products/debug  or build_products/release, according build type

    set (Project_Targets_dir ${CMAKE_SOURCE_DIR}/build_targets CACHE STRING "Directory of the output files: exe, dll, so, doc, translations...")
    set(sDebugBuildProductsDir ${Project_Targets_dir}/debug)
    set(sReleaseBuildProductsDir ${Project_Targets_dir}/release)

    if( CMAKE_BUILD_TYPE MATCHES Release )
            set (sOutputDir ${sReleaseBuildProductsDir})
    else ()
            set (sOutputDir ${sDebugBuildProductsDir})
    endif()

##
# FUNCTIONS
#
        # groups source files according to the directory structure, usefull for visual studio only
	function(group_files SOURCES)
	  foreach(SOURCE_FILE ${${SOURCES}})
		get_filename_component(GROUP "${SOURCE_FILE}" PATH)
		string(REPLACE "${CMAKE_SOURCE_DIR}" "" GROUP "${GROUP}")
		string(REPLACE "/" "\\" GROUP "${GROUP}")

		set(GROUP "${GROUP}\\")
		foreach(REPL ${ARGN})
		  string(REPLACE "\\${REPL}\\" "\\" GROUP "${GROUP}")
		endforeach()

		source_group("${GROUP}" FILES "${SOURCE_FILE}")
	  endforeach()
	endfunction()
 

 
## 
#  Qt
#
    MESSAGE(STATUS " ")
    MESSAGE(STATUS "* QT:")

    # Instruct CMake to run moc automatically when needed
    set(CMAKE_AUTOMOC ON)
    # Create code from a list of Qt designer ui files
    set(CMAKE_AUTOUIC ON)

    if(WIN32)
        #Can be used in case the lib is not found:
        #set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.5.1/5.5/mingw492_32/lib/cmake" CACHE PATH "QT installation DIR.")
    endif(WIN32)
    if(LINUX)
        #set(CMAKE_PREFIX_PATH "/home/jon/Qt/5.11.1/gcc_64/lib/cmake" CACHE PATH "QT installation DIR.")
    endif()
    
    # Find the QtWidgets library
    #find_package(Qt5 REQUIRED)
    find_package(Qt5Core REQUIRED)
    MESSAGE(STATUS "Qt5Core found, verion: ${Qt5Core_VERSION}")
    find_package(Qt5Widgets REQUIRED)
    MESSAGE(STATUS "Qt5Widgets found, verion: ${Qt5Widgets_VERSION}")

##
# Sub-directories include
#
    add_subdirectory(src)
    add_subdirectory(data)
    add_subdirectory(doc)
    add_subdirectory(test)
