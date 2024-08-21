configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day1/exercise_1_methane_in_a_box/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day1/exercise_3_co2_in_itq-29/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day1/exercise_4_co2_energy_histogram_zeolites/300K/itq-29/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day1/exercise_4_co2_energy_histogram_zeolites/300K/mfi/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day1/exercise_4_co2_energy_histogram_zeolites/600K/itq-29/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day1/exercise_4_co2_energy_histogram_zeolites/600K/mfi/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day1/exercise_5_co2_in_itq-29_energy_histogram/300K/itq-29/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day1/exercise_5_co2_in_itq-29_energy_histogram/300K/mfi/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day1/exercise_5_co2_in_itq-29_energy_histogram/600K/itq-29/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day1/exercise_5_co2_in_itq-29_energy_histogram/600K/mfi/run)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day2/exercise_1_rdf/100mol_uc/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day2/exercise_1_rdf/200mol_uc/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day2/exercise_1_rdf/400mol_uc/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day2/exercise_3_md_diffusion_methane_in_mfi/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day2/exercise_4_md_diffusion_methane_in_itq-29/run)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day3/exercise_1_gibbs_co2/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day3/exercise_2_helium_void_fraction_itq-29/run)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/day3/exercise_3_adsorption_co2_in_itq-29/run)


install(DIRECTORY "day1" DESTINATION share/raspa3/workshop 
        COMPONENT workshop
        PATTERN "run" EXCLUDE
        PERMISSIONS OWNER_READ GROUP_READ)

install(DIRECTORY "day1" DESTINATION share/raspa3/workshop 
        COMPONENT workshop
        PATTERN "run" 
        PERMISSIONS  OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)

install(DIRECTORY "day2" DESTINATION share/raspa3/workshop 
        COMPONENT workshop
        PATTERN "run" EXCLUDE
        PERMISSIONS OWNER_READ GROUP_READ)

install(DIRECTORY "day2" DESTINATION share/raspa3/workshop 
        COMPONENT workshop
        PATTERN "run" 
        PERMISSIONS  OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)

install(DIRECTORY "day3" DESTINATION share/raspa3/workshop 
        COMPONENT workshop
        PATTERN "run" EXCLUDE
        PERMISSIONS OWNER_READ GROUP_READ)

install(DIRECTORY "day3" DESTINATION share/raspa3/workshop 
        COMPONENT workshop
        PATTERN "run" 
        PERMISSIONS  OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)
