MESSAGE(STATUS " ")
MESSAGE(STATUS "* EXECUTABLE")

##
# EXECUTABLE'S NAME
#

    SET( sExeName "${PROJECT_NAME}Exe")
    MESSAGE(STATUS "Name: ${sExeName}")
	

##
# INCLUDES:
#
        set(sExeSourceDir "${CMAKE_CURRENT_SOURCE_DIR}")
        SET(sExeSourceDir ${sExeSourceDir} PARENT_SCOPE)  # For locale translations and unit test
        MESSAGE(STATUS "SOURCE_DIR: ${sExeSourceDir}")
        include_directories (${sExeSourceDir})
	


##
# SOURCE FILES RESEARCH
#
    #Research of .h/.cpp files
    file(GLOB_RECURSE sExeSourceFiles  ${sExeSourceDir}/*.h ${sExeSourceDir}/*.cpp)
    #group_files(sExeSourceFiles)
     SET(sExeSourceFiles ${sExeSourceFiles} PARENT_SCOPE)  # For unit test

    # Research of .UI files:
    file(GLOB_RECURSE sExeUiFiles  ${sExeSourceDir}/*.ui)
    #group_files(sExeUiFiles)


    #qt5_wrap_ui(UI_HEADERS ${sExeUiFiles})  # -> not needed because CMAKE_AUTOUIC is activated

##
# Executable
#	
        add_executable(${sExeName}  WIN32 ${sExeSourceFiles} ${sExeUiFiles})
        set_target_properties(${sExeName} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY ${sOutputDir}/bin
            LIBRARY_OUTPUT_DIRECTORY ${sOutputDir}/bin
            RUNTIME_OUTPUT_DIRECTORY ${sOutputDir}/bin
            )

        # if  Project_Use_build_time_share_files_and_settings = ON  ->  the aplication will read the data files  in AppDir/../data
        if ( Project_Use_build_time_share_files_and_settings )
            target_compile_definitions(${sExeName} PRIVATE USE_BUILD_TIME_SHARE_FILES_AND_SETTINGS=1)
        else()
            target_compile_definitions(${sExeName} PRIVATE USE_BUILD_TIME_SHARE_FILES_AND_SETTINGS=0)
        endif()

##
# link to Qt
#
    MESSAGE(STATUS "Link to Qt5 lib")
    target_link_libraries( ${sExeName} ${Qt5Core_QTMAIN_LIBRARIES} )
    qt5_use_modules(${sExeName} Widgets)
	

    MESSAGE(STATUS " ")
