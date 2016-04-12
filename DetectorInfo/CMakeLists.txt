
art_make( NO_PLUGINS
          LIBRARY_NAME  lardata_DetectorInfo
          LIB_LIBRARIES
			larcore_Geometry
                        ${MF_MESSAGELOGGER}
                        ${MF_UTILITIES}
                        ${CETLIB}
			${FHICLCPP}
                        ${ROOT_BASIC_LIB_LIST}
			${Boost_SYSTEM_LIBRARY}
        )



install_headers()
install_fhicl()
install_source()

#add_subdirectory(test)

