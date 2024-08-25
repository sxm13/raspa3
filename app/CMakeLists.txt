project(raspa3)

add_executable(${PROJECT_NAME}
        main.cpp
)

if (LINUX)
  target_link_libraries(${PROJECT_NAME}
        PUBLIC
        libs::raspakit
        -static-libgcc
        ${BLAS_LIBRARIES}
        ${LAPACK_LIBRARIES}
        ${HDF5_CXX_LIBRARIES}
        ${HDF5_LIBRARIES}
        OpenMP::OpenMP_CXX
        -nostdlib++
        -l:libc++.a
        -l:libc++abi.a
        -lm
        -ldl
        -lz
        -lpthread
)
elseif (WIN32 AND ${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64")
  target_link_libraries(${PROJECT_NAME}
        PUBLIC
        libs::raspakit
        ${BLAS_LIBRARIES}
        ${LAPACK_LIBRARIES}
        ${HDF5_CXX_LIBRARIES}
	${llvm_libs}
        -nostdlib++
)
elseif (WIN32 AND ${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM64")
  target_link_libraries(${PROJECT_NAME}
        PUBLIC
        libs::raspakit
        ${BLAS_LIBRARIES}
        ${LAPACK_LIBRARIES}
        C:/vcpkg/installed/arm64-windows/lib/libf2c.lib
        ${HDF5_CXX_LIBRARIES}
	${llvm_libs}
        -nostdlib++
)
elseif (APPLE)
  target_link_libraries(${PROJECT_NAME}
        PUBLIC
        libs::raspakit
        ${BLAS_LIBRARIES}
        ${LAPACK_LIBRARIES}
        /usr/local/lib/libhdf5_cpp.a
      	/usr/local/lib/libhdf5.a
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

install(TARGETS ${PROJECT_NAME} DESTINATION bin COMPONENT app)
