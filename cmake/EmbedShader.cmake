function(EmbedShader input output target)

    add_custom_command(
            OUTPUT "${PROJECT_BINARY_DIR}/Embed${target}/Embed${target}.cpp" "${PROJECT_BINARY_DIR}/Embed${target}/Embed${target}.hpp"
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${input} -o ${output}
            COMMAND ${CMAKE_COMMAND} -Dfile=${output} -Dtarget=${target} -Droot=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateEmbedFile.cmake
            DEPENDS "${input}"
            VERBATIM
    )

    add_library(Embed${target} "${PROJECT_BINARY_DIR}/Embed${target}/Embed${target}.cpp" "${PROJECT_BINARY_DIR}/Embed${target}/Embed${target}.hpp")

endfunction()