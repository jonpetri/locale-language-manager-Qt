FIND_PACKAGE(Doxygen)


if (Doxygen_FOUND)

    option(Project_Build_Code_documentation "Create the HTML based code documentation (requires Doxygen)" ${DOXYGEN_FOUND})

    if(Project_Build_Code_documentation)

        MESSAGE(STATUS "* DOXYGEN:")

        #Check of sExeSourceDir
        if ("${sExeSourceDir}" STREQUAL "")
            MESSAGE(ERROR "The variable of project's source directory is empty.")
        endif()

        # Two docs are generated

        # 1- Doc of all the demo project
        add_subdirectory(codeDemo)

        # 2- Doc of the LlmQtLocaleLanguageManager class only, for github's wiki
        add_subdirectory(codeLocaleLanguageManager)

    endif()
else ()

       MESSAGE(STATUS "Doxygen not found, the code documentation can't be generated")

endif()


MESSAGE(STATUS " ")
