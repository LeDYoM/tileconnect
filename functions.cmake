function (prepareTestLibrary)
    include(FetchContent)
    message(STATUS "Fetching Catch2")
    #======================================

    # Catch the previous value of BUILD_SHARED_LIBS
    # looks like it is set somewhere. Set it to off
    set(PREVIOUS_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
    if(BUILD_SHARED_LIBS)
      set(BUILD_SHARED_LIBS OFF)
    endif()

    set(CATCH2_COMMIT 88abf9bf325c798c33f54f6b9220ef885b267f4f) # 3.12.0
    FetchContent_Declare(Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG ${CATCH2_COMMIT}
        CMAKE_CACHE_ARGS -DBUILD_SHARED_LIBS=OFF
    )

    FetchContent_MakeAvailable(Catch2)

    # Set the old value of BUILD_SHARED_LIBS
    set(BUILD_SHARED_LIBS ${PREVIOUS_BUILD_SHARED_LIBS})
    #======================================
    message(STATUS "Fetching Catch2 libraries done")
endfunction()

function(set_cxx_standard CURRENT_TARGET)
    set_target_properties(${CURRENT_TARGET} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON)
endfunction()

function(set_project_options CURRENT_TARGET)
set_cxx_standard(${CURRENT_TARGET})
    target_compile_options(${CURRENT_TARGET} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Wno-unknown-pragmas>
    )
    set_property(TARGET ${CURRENT_TARGET} PROPERTY COMPILE_WARNING_AS_ERROR ON)
endfunction()
