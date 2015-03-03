add_subdirectory( RawData )
add_subdirectory( RecoObjects )
add_subdirectory( Utilities )

file(GLOB FHICL_FILES 
     [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

