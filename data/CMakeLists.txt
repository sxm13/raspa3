install(DIRECTORY forcefields DESTINATION share/raspa3 COMPONENT app)
install(DIRECTORY molecules DESTINATION share/raspa3 COMPONENT app)
install(DIRECTORY manual DESTINATION share/raspa3 COMPONENT manual)

if(WIN32)
  install(DIRECTORY dlls/ DESTINATION bin COMPONENT app)
endif()
