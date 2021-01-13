project(resgen)

add_executable(resgen main.cpp)

target_compile_features(resgen PRIVATE cxx_std_17)

function(add_resource generated_source)
    set(resource_file "${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1}")
    set(outfile "${PROJECT_BINARY_DIR}/${ARGV1}")
    get_filename_component(filedir ${ARGV1} DIRECTORY)
    file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${filedir}")
    add_custom_command(
        OUTPUT "${outfile}.cpp" "${outfile}.hpp"
        COMMAND resgen ${resource_file}
        DEPENDS resgen ${resource_file}
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/${filedir}"
        COMMENT "Generating code for resource ${resource_file}"
        VERBATIM
        )
    set(${generated_source} "${outfile}.cpp" PARENT_SCOPE)
endfunction()
