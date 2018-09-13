
FIND_PACKAGE(Doxygen)


if (Doxygen_FOUND)

    option(Project_Build_Code_documentation "Create and install the HTML based code documentation (requires Doxygen)" ${DOXYGEN_FOUND})

    if(Project_Build_Code_documentation)

        MESSAGE(STATUS "* DOXYGEN:")

        set(sDoxygenSettingsFileIn ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
        set(sDoxygenSettingsFileOut ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # Definition of output directory, in sDoxygenSettingsFileOut, thanks to the command configure_file()
        #   Compute the relative path from a <directory> to a <file> and store it in the <variable>.
        file(RELATIVE_PATH sRelativeLocation "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

        set( sDoxygenOutputDir ${sOutputDir}/${sRelativeLocation} )

        configure_file(${sDoxygenSettingsFileIn} ${sDoxygenSettingsFileOut} @ONLY)


        add_custom_target ( t_doxygen_make_directory ALL
                            COMMAND ${CMAKE_COMMAND} -E make_directory ${sDoxygenOutputDir}
                        )

        MESSAGE(STATUS "Macro to run Doxygen")
        add_custom_target( t_doxygen_doc_generation ALL
                            COMMAND ${DOXYGEN_EXECUTABLE} ${sDoxygenSettingsFileOut}
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            COMMENT "Doxygen : Generating code documentation."
                            VERBATIM
                             )

        add_dependencies(t_doxygen_doc_generation t_doxygen_make_directory) # add_dependencies(2nd done, 1st done)

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Open_doc.bash ${sDoxygenOutputDir}/Open_doc.bash COPYONLY)

        #   For personal record: I tried te create the output directory with a PRE_BUILD, but it works only for the "real" targets like library or executable
        #  add_custom_command(TARGET doxygen
        #          PRE_BUILD
        #          COMMAND ${CMAKE_COMMAND} -E make_directory ${sDoxygenOutputDir}
        #          COMMENT "Doxygen : make_directory" )


        #    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION     share/doc)

    endif()
else ()

       MESSAGE(STATUS "Doxygen not found, the code documentation can't be generated")

endif()


MESSAGE(STATUS " ")
