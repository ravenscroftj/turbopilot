set(TURBOPILOT_TARGET turbopilot)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm" OR ${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
    message(STATUS "ARM detected")
    if (MSVC)
        # TODO: arm msvc?
    else()
        if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "armv6")
            # Raspberry Pi 1, Zero
            add_compile_options(-mfpu=neon-fp-armv8 -mfp16-format=ieee -mno-unaligned-access)
        endif()
        if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "armv7")
            # Raspberry Pi 2
            add_compile_options(-mfpu=neon-fp-armv8 -mfp16-format=ieee -mno-unaligned-access -funsafe-math-optimizations)
        endif()
        if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "armv8")
            # Raspberry Pi 3, 4, Zero 2 (32-bit)
            add_compile_options(-mfp16-format=ieee -mno-unaligned-access)
        endif()
    endif()
endif()


add_executable(${TURBOPILOT_TARGET} 
  main.cpp 
  gptj.cpp
  common.cpp
  server.cpp
  starcoder.cpp
  ../include/turbopilot/model.hpp
  ../include/turbopilot/gptj.hpp
  ../include/turbopilot/starcoder.hpp
  )


target_include_directories(${TURBOPILOT_TARGET} PRIVATE
  ../include
  ../extern/spdlog/include
  ../extern/crow/include
)


target_link_libraries(${TURBOPILOT_TARGET} PRIVATE ggml argparse)

target_link_libraries(${TURBOPILOT_TARGET} PUBLIC Crow::Crow)