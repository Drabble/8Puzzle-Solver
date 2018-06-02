project(PCM C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm -fopenmp -Wall")


add_executable(PCM_seq main.c)
