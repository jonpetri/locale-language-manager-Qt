##
# Files research
#
    #Research of Doxyfile.in
    file(GLOB sDoxygenSettingsFileIn  ${CMAKE_CURRENT_SOURCE_DIR}/*.in)
    #Research of Open_doc.bash
    file(GLOB sOpenDocLinuxBashFile  ${CMAKE_CURRENT_SOURCE_DIR}/*.bash)


##
# configure_file() of Doxyfile.in.
# The variables called sDoxygenTag_* modify the file sDoxygenSettingsFileIn (Doxyfile.in)
# through the CMAKE command configure_file().
#

    set(sDoxygenSettingsFileOut ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)


    # Compute the relative path from a <CMAKE_SOURCE_DIR> to a <CMAKE_CURRENT_SOURCE_DIR> and store it in the <variable>.
    file(RELATIVE_PATH sRelativeLocation "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

    set(sDoxygenTag_OutputDir       "${sOutputDir}/${sRelativeLocation}")
    set(sDoxygenTag_HtmlOutput      "html") # the name of the ouput folder
    set(sDoxygenTag_Input           "${sExeSourceDir} ${CMAKE_SOURCE_DIR}/README.md")
    set(sDoxygenTag_ImagePath       "${CMAKE_SOURCE_DIR}/data/images/")
    set(sDoxygenTag_ProjectName     "\"Locale language manager class for QT, in a demo project\"")
    set(sDoxygenTag_ProjectBrief    "\"Language management in a multilingual Qt application made easier.\"")

    # configure Doxyfile.in with the variables sDoxygenTag_*
    configure_file(${sDoxygenSettingsFileIn} ${sDoxygenSettingsFileOut} @ONLY)


##
# Targets / Add a macro in the build process to build a Doxygen doc
#
    MESSAGE(STATUS "Add a macro in the build process to build a Doxygen doc of all the demo project")
    add_custom_target( t_doc_doxygen_AllDemoProject ALL
                        COMMAND ${DOXYGEN_EXECUTABLE} ${sDoxygenSettingsFileOut}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        COMMENT "Doxygen : Generating All demo project code documentation."
                        VERBATIM
                        SOURCES ${sDoxygenSettingsFileIn} ${sOpenDocLinuxBashFile}
                     )


    configure_file(${sOpenDocLinuxBashFile} ${sDoxygenTag_OutputDir}/Open_doc.bash COPYONLY)

#   For personal record: I tried te create the output directory with a PRE_BUILD, but it works only for the "real" targets like library or executable
#  add_custom_command(TARGET doxygen
#          PRE_BUILD
#          COMMAND ${CMAKE_COMMAND} -E make_directory ${sDoxygenTag_OutputDir}
#          COMMENT "Doxygen : make_directory" )


#    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION     share/doc)


