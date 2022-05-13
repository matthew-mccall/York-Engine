function(EmbedFile file target)
    file(READ ${file} "${target}_RAW" HEX)
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedTemplate.hpp.in" EmbedInclude)
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedTemplate.cpp.in" EmbedSource)

    string(REGEX MATCHALL "([A-Fa-f0-9][A-Fa-f0-9])" SEPARATED_HEX ${${target}_RAW})
    set(DATA_SIZE 0)
    # Iterate through each of the bytes from the source file
    foreach (hex IN LISTS SEPARATED_HEX)
        # Write the hex string to the line with an 0x prefix
        # and a , postfix to seperate the bytes of the file.
        string(APPEND DATA "0x${hex},")
        # Increment the element counter before the newline.
        math(EXPR iMod "${DATA_SIZE} % 32")
        if (iMod EQUAL 0)
            # Write a newline so that all of the array initializer
            # gets spread across multiple lines.
            string(APPEND DATA "\n    ")
        endif ()

        math(EXPR DATA_SIZE "${DATA_SIZE}+1")
    endforeach ()

    # configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedTemplate.hpp.in" "${PROJECT_BINARY_DIR}/Embed${target}/Embed${target}.hpp" NEWLINE_STYLE UNIX)
    # configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedTemplate.cpp.in" "${PROJECT_BINARY_DIR}/Embed${target}/Embed${target}.cpp" NEWLINE_STYLE UNIX)

    file(CONFIGURE OUTPUT "${PROJECT_BINARY_DIR}/Embed${target}/Embed${target}.hpp" CONTENT "${EmbedInclude}")
    file(CONFIGURE OUTPUT "${PROJECT_BINARY_DIR}/Embed${target}/Embed${target}.cpp" CONTENT "${EmbedSource}")
endfunction()