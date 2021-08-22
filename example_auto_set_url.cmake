set(PICO_EXAMPLE_URL_BASE "https://github.com/jqt3of5/st25dv_pico.git/tree/HEAD")
macro(example_auto_set_url TARGET)
    file(RELATIVE_PATH URL_REL_PATH "${PICO_EXAMPLES_PATH}" "${CMAKE_CURRENT_LIST_DIR}")
    pico_set_program_url(${TARGET} "${PICO_EXAMPLE_URL_BASE}/${URL_REL_PATH}")
endmacro()