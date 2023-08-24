set(TURBOPILOT_TARGET turbopilot)

find_package(Boost COMPONENTS thread system REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(${TURBOPILOT_TARGET} 
  main.cpp 
  gptj.cpp
  gptneox.cpp
  common.cpp
  server.cpp
  starcoder.cpp
  ../include/turbopilot/model.hpp
  ../include/turbopilot/gptj.hpp
  ../include/turbopilot/gptneox.hpp
  ../include/turbopilot/starcoder.hpp
  )

#set(THREADS_PREFER_PTHREAD_FLAG TRUE)
#find_package(Threads REQUIRED)


target_include_directories(${TURBOPILOT_TARGET} PRIVATE
  ../include
  ../extern/spdlog/include
  ../extern/crow/include
)

#target_compile_features(${TURBOPILOT_TARGET} PRIVATE cxx_std_11)

target_link_libraries(${TURBOPILOT_TARGET} PRIVATE ggml argparse)