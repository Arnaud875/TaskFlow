function(configure_msvc_compile_flags target)
	target_compile_options(${target} PRIVATE
        $<$<CONFIG:Debug>:
            /Zi     # Debug info
            /Od     # Disable optimization
            /RTC1   # Runtime checks
            /sdl    # Security checks
            /WX     # Treat warnings as errors
            /MP     # Multi-processor compilation
            /DEBUG  # Generate debug info
            /analyze# Code analysis
        >
        $<$<CONFIG:Release>:
            /O2     # Maximum optimization
            /W4     # Warning level 4
            /WX     # Treat warnings as errors
            /MP     # Multi-processor compilation
            /GL     # Whole program optimization
            /Gy     # Function-level linking
            /Oi     # Intrinsic functions
            /DNDEBUG# Define NDEBUG
            /guard:cf# Control Flow Guard
        >
    )

    target_link_options(${target} PRIVATE
        $<$<CONFIG:Release>:
            /LTCG   # Link-time code generation
            /OPT:REF# Remove unused functions
            /OPT:ICF# Identical COMDAT folding
        >
    )
endfunction()

function(configure_gcc_compile_flags target)
    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Debug>:
            -O0     # No optimization
            -g      # Debug info
            -Wall   # Enable all warnings
            -Wextra # Enable extra warnings
            -Werror # Treat warnings as errors
            -pedantic # Pedantic warnings
        >
        $<$<CONFIG:Release>:
            -O3     # Maximum optimization
            -Wall   # Enable all warnings
            -Wextra # Enable extra warnings
            -Werror # Treat warnings as errors
            -pedantic # Pedantic warnings
        >
    )
endfunction()

function(compile_flags target)
	if (MSVC)
		configure_msvc_compile_flags(${target})
    elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        configure_gcc_compile_flags(${target})
	endif()
endfunction()