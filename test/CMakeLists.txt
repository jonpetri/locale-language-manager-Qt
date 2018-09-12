## 
#  Boost unit_test_framework  find_package
#

    MESSAGE(STATUS "* Boost Unit Test :")

    if (WIN32)
        #set(BOOST_ROOT "C:/DEV/TOOLS/Boost/boost")
        #set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
    endif (WIN32)

    #If you run into issues with CMake looking for incorrect library names, this may help:
    #add_definitions(-DBOOST_ALL_NO_LIB)  # tell the compiler to undefine this boost macro
    find_package(Boost COMPONENTS unit_test_framework )

if (Boost_UNIT_TEST_FRAMEWORK_FOUND)

    option(Project_Build_Unit_Test_executable "Build the Boost Unit Test executable" ${Boost_UNIT_TEST_FRAMEWORK_FOUND})

    if(Project_Build_Unit_Test_executable)

        message(STATUS "Boost Include: ${Boost_INCLUDE_DIR}")
        message(STATUS "Boost Library dirs: ${Boost_LIBRARY_DIRS}")
        message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")
        # These commands will ensure that Boost libraries (all) are linked dynamically:
        set(Boost_USE_STATIC_LIBS        OFF)
        set(Boost_USE_MULTITHREADED      ON)
        set(Boost_USE_STATIC_RUNTIME     OFF)
        set(BOOST_ALL_DYN_LINK           ON)   # force dynamic linking for all libraries

        enable_testing()

    ##
    # TEST PROJECT:
    #

	    SET(sUnitTestProjectName "${PROJECT_NAME}_UnitTest")
	    #SET(sUnitTestProjectName ${sUnitTestProjectName} PARENT_SCOPE)
	
            MESSAGE(STATUS " ")
	    MESSAGE("Project: ${sUnitTestProjectName}")
	    project(${sUnitTestProjectName})



    ##
    # INCLUDES:
    #
        set( sUnitTestIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" )
        #set( sUnitTestIncludeDir "${sUnitTestIncludeDir}" PARENT_SCOPE)

        MESSAGE(STATUS "INCLUDE_DIR: ${sUnitTestIncludeDir}")
        include_directories (${sUnitTestIncludeDir})
        include_directories (${sExeSourceDir})

    ##
    # SOURCE FILES RESEARCH
    #

        file(GLOB_RECURSE sUnitTestSourceFiles  ${sUnitTestIncludeDir}/*.h ${sUnitTestIncludeDir}/*.cpp)
        group_files(sUnitTestSourceFiles)

    ##
    # COVERAGE
    #
        SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
        SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

	
    ##
    # Executable
    #	


        list(REMOVE_ITEM sExeSourceFiles ${sExeSourceDir}/main_Demo.cpp)
        add_executable(${sUnitTestProjectName}  ${sUnitTestSourceFiles} ${sExeSourceFiles} )

        # if  Project_Use_build_time_share_files_and_settings = ON  ->  the aplication will read the data files  in AppDir/../data
        if ( Project_Use_build_time_share_files_and_settings )
            target_compile_definitions(${sUnitTestProjectName} PRIVATE USE_BUILD_TIME_SHARE_FILES_AND_SETTINGS=1)
        else()
            target_compile_definitions(${sUnitTestProjectName} PRIVATE USE_BUILD_TIME_SHARE_FILES_AND_SETTINGS=0)
        endif()


        INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
        LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
        target_link_libraries(${sUnitTestProjectName} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    # Definition of exe files output directory
        #   Compute the relative path from a <directory> to a <file> and store it in the <variable>.
        file(RELATIVE_PATH sRelativeLocation "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
        file(MAKE_DIRECTORY ${sOutputDir}/${sRelativeLocation})
        set_target_properties(${sUnitTestProjectName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${sOutputDir}/${sRelativeLocation})


    ADD_TEST(boostTest ${sUnitTestProjectName})


    # Copy of test data folder
    add_custom_target(copy-test-folder ALL
        COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${sOutputDir}/${sRelativeLocation}/data)



    ##
    # link to Qt
    #
        MESSAGE(STATUS "Link to Qt5 lib")

        target_link_libraries( ${sUnitTestProjectName} ${Qt5Core_QTMAIN_LIBRARIES} )
        qt5_use_modules(${sUnitTestProjectName} Core)
        qt5_use_modules(${sUnitTestProjectName} Widgets)


    ##
    # COVERAGE
    #

        SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
        SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov --coverage")


        set_target_properties(${sUnitTestProjectName} PROPERTIES COMPILE_FLAGS ${GCC_COVERAGE_COMPILE_FLAGS})
        #target_compile_options(${sUnitTestProjectName} PUBLIC ${GCC_COVERAGE_COMPILE_FLAGS})
        set_target_properties(${sUnitTestProjectName} PROPERTIES LINK_FLAGS ${GCC_COVERAGE_LINK_FLAGS})
    endif()
else ()

       MESSAGE(STATUS "Boost unit_test_framework not found, the unit test exe can't be build")
endif()


    MESSAGE(STATUS " ")

