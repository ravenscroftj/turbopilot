set(TURBOPILOT_TARGET turbopilot)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


add_executable(${TURBOPILOT_TARGET} 
  main.cpp 
  gptj.cpp
  common.cpp
  server.cpp
  ../include/turbopilot/model.hpp
  ../include/turbopilot/gptj.hpp
  )


target_include_directories(${TURBOPILOT_TARGET} PRIVATE
  ../include
  ../extern/spdlog/include
  ../extern/crow/include
)


target_link_libraries(${TURBOPILOT_TARGET} PRIVATE ggml argparse)

target_link_libraries(${TURBOPILOT_TARGET} PUBLIC Crow::Crow)