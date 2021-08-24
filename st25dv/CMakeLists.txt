if (TARGET tinyusb_device)
    add_executable(st25dv
            st25dv_main.c
            st25dv.c
            )

    # Pull in our pico_stdlib which aggregates commonly used features
    target_link_libraries(st25dv pico_stdlib hardware_i2c)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(st25dv 1)
    pico_enable_stdio_uart(st25dv 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(st25dv)

    # add url via pico_set_program_url
    example_auto_set_url(st25dv)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building st25dv because TinyUSB submodule is not initialized in the SDK")
endif()
