add_executable(unit_tests_raspakit
               cbmc_interactions.cpp
               energy_decomposition.cpp 
               gradients.cpp
               pbc.cpp
               spacegroup.cpp
               dudlambda.cpp
               ewald.cpp
               insertion_deletion.cpp
               mc.cpp
               pressure.cpp
               static_energy.cpp
               rigid_gradient.cpp
               electrostatic_potential.cpp
               electrostatic_field.cpp
               electrostatic_polarization.cpp
               nist_water_1.cpp
               nist_water_2.cpp
               nist_water_3.cpp
               nist_water_4.cpp
               main.cpp)


if (LINUX)
  target_link_libraries(unit_tests_raspakit
                        PUBLIC
                        libs::raspakit
                        ${BLAS_LIBRARIES}
                        ${LAPACK_LIBRARIES}
                        ${HDF5_CXX_LIBRARIES}
                        ${HDF5_LIBRARIES}
                        OpenMP::OpenMP_CXX
                        GTest::gtest
                        GTest::gtest_main
                        GTest::gmock
                        GTest::gmock_main
                        -nostdlib++
                        -l:libc++.a
                        -l:libc++abi.a
                        -lm
                        -ldl
                        -lz
                        -lpthread
)
elseif (WIN32)
  target_link_libraries(unit_tests_raspakit
                        PUBLIC
                        libs::raspakit
                        ${BLAS_LIBRARIES}
                        ${LAPACK_LIBRARIES}
                        ${HDF5_CXX_LIBRARIES}
                        GTest::gtest
                        GTest::gtest_main
                        ${llvm_libs}
                        -nostdlib++
)
elseif (APPLE)
target_link_libraries(unit_tests_raspakit PRIVATE
                      libs::raspakit
                      ${BLAS_LIBRARIES}
                      ${LAPACK_LIBRARIES}
                      /usr/local/lib/libhdf5_cpp.a
                      /usr/local/lib/libhdf5.a
                      GTest::gtest
                      GTest::gtest_main
                      GTest::gmock
                      GTest::gmock_main
                      -nostdlib++
                      ${LLVM_LIBS}/c++/libc++experimental.a
                      ${LLVM_LIBS}/c++/libc++.a
                      ${LLVM_LIBS}/c++/libc++abi.a
                      ${LLVM_LIBS}/libunwind.a
                      ${LLVM_LIBS}/../../../libomp/${LLVM_VERSION}/lib/libomp.a
                      -lz
                      -lpthread
)
endif()

gtest_discover_tests(unit_tests_raspakit)

install(TARGETS unit_tests_raspakit DESTINATION share/raspa3/tests COMPONENT unit_tests_raspakit)
