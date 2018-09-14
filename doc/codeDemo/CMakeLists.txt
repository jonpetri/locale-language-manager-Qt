##
# In that file, the variables called sDoxygenTag_* modify the file sDoxygenSettingsFileIn (Doxyfile.in)
# through the CMAKE command configure_file().
#

set(sDoxygenSettingsFileIn ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(sDoxygenSettingsFileOut ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Definition of output directory, in sDoxygenSettingsFileOut, thanks to the command configure_file()
#   Compute the relative path from a <directory> to a <file> and store it in the <variable>.
file(RELATIVE_PATH sRelativeLocation "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(sDoxygenTag_OutputDir ${sOutputDir}/${sRelativeLocation} )
set(sDoxygenTag_Input "${sExeSourceDir} ${CMAKE_SOURCE_DIR}/README.md")
set(sDoxygenTag_ImagePath "${CMAKE_SOURCE_DIR}/data/images/")

# configure Doxyfile.in with the variables sDoxygenTag_*
configure_file(${sDoxygenSettingsFileIn} ${sDoxygenSettingsFileOut} @ONLY)


add_custom_target ( t_doc_AllDemoProject_makeDirectory ALL
                    COMMAND ${CMAKE_COMMAND} -E make_directory ${sDoxygenTag_OutputDir}
                )

MESSAGE(STATUS "Add a macro in the build process to build a Doxygen doc of all the demo project")
add_custom_target( t_doc_doxygen_AllDemoProject ALL
                    COMMAND ${DOXYGEN_EXECUTABLE} ${sDoxygenSettingsFileOut}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    COMMENT "Doxygen : Generating All demo project code documentation."
                    VERBATIM
                     )

add_dependencies(t_doc_doxygen_AllDemoProject t_doc_AllDemoProject_makeDirectory) # add_dependencies(2nd done, 1st done)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Open_doc.bash ${sDoxygenTag_OutputDir}/Open_doc.bash COPYONLY)

#   For personal record: I tried te create the output directory with a PRE_BUILD, but it works only for the "real" targets like library or executable
#  add_custom_command(TARGET doxygen
#          PRE_BUILD
#          COMMAND ${CMAKE_COMMAND} -E make_directory ${sDoxygenTag_OutputDir}
#          COMMENT "Doxygen : make_directory" )


#    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION     share/doc)


