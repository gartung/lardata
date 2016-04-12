
art_make( BASENAME_ONLY
	  LIBRARY_NAME     lardata_MCBase
          LIB_LIBRARIES
			SimulationBase
			${ROOT_BASIC_LIB_LIST}
                        ${ROOT_EG}
)

install_headers()
install_fhicl()
install_source()
