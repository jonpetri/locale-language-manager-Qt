set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)

MESSAGE(STATUS "* LOCALE TRANSLATIONS:")

FIND_PACKAGE(Qt5LinguistTools)

if (Qt5LinguistTools_FOUND)

    MESSAGE(STATUS "Qt5LinguistTools found, verion: ${Qt5LinguistTools_VERSION}") 
    MESSAGE(STATUS "Macro to process translation files")

    #Check of sExeSourceDir
    if ("${sExeSourceDir}" STREQUAL "")
        MESSAGE(ERROR "The variable of the source directory of translations is empty.")
    endif()

    # research  of existing .TS files:
    # The "create translation" macro will be done over that files
    file(GLOB_RECURSE sTranslationTsFiles  ${CMAKE_CURRENT_SOURCE_DIR}/*.ts)


    # manual add of new ts file, if wanted
    set( Project_Add_new_translation_ts_file "" CACHE STRING
        "The name of a new ts file, if wanted. The name has to be: [..]_<language code>[_<region code>].ts  Ex: _zh_CH.ts or _eu.ts ")

    if ( NOT("${Project_Add_new_translation_ts_file}" STREQUAL "") )
        set (sTranslationTsFiles "${sTranslationTsFiles};${CMAKE_CURRENT_SOURCE_DIR}/${Project_Add_new_translation_ts_file}")
    endif()

    # If there is no ts file yet, we create a 1st one
    if ("${sTranslationTsFiles}" STREQUAL "")
        #That means there is no ts file created yet
        MESSAGE(WARNING "There is no ts file created yet ??  -> Creation of new ts file: _eu.ts")
                set(sTranslationTsFiles "${CMAKE_CURRENT_SOURCE_DIR}/_eu.ts")
    endif()

    # Definition of QM files output directory
        #   Compute the relative path from a directory to a file or directory and store it in the variable.
        file(RELATIVE_PATH sRelativeLocation "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
        set_source_files_properties(${sTranslationTsFiles} PROPERTIES OUTPUT_LOCATION ${sOutputDir}/${sRelativeLocation})

    # macro creation :
    QT5_CREATE_TRANSLATION(sTranslationQmFiles ${sExeSourceDir} ${sTranslationTsFiles} OPTIONS -source-language en)
    # In case of issues, the next command can help:
    #QT5_CREATE_TRANSLATION(sTranslationQmFiles ${sExeSourceDir} ${sTranslationTsFiles} OPTIONS -I ${sExeSourceDir})

    ADD_CUSTOM_TARGET ( t_translations_update ALL
                        DEPENDS ${sTranslationQmFiles}
                        COMMENT "Qt locale translation: ts files update and qm files build"
                        VERBATIM
                      )


     if ("${sTranslationQmFiles}" STREQUAL "")  # If  sTranslationQmFiles is empty, there is an issue somewhere
        MESSAGE(WARNING "Therer is no qm file to generate")
    endif()

else ()

       MESSAGE(WARNING "Qt5LinguistTools not found, the translation files will not be processed")

endif()


#	install(FILES ${sTranslationQmFiles} DESTINATION translations)

MESSAGE(STATUS " ")
