find_package(MPI REQUIRED)

if(MPI_CXX_FOUND)
    add_executable(solver main.cpp)
    target_link_libraries(solver PRIVATE MPI::MPI_CXX)
    
    target_compile_definitions(solver PRIVATE _USE_MATH_DEFINES)
else()
    message(WARNING "MPI не найден! Лаба 3 (p1_first_test) не будет собрана.")
endif()