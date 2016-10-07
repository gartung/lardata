cet_test( SurfYZTest USE_BOOST_UNIT LIBRARIES lardata_RecoObjects larreco_Deprecated )
cet_test( SurfXYZTest USE_BOOST_UNIT LIBRARIES lardata_RecoObjects larreco_Deprecated )
cet_test( SurfYZLineTest USE_BOOST_UNIT LIBRARIES lardata_RecoObjects larreco_Deprecated )
cet_test( TrackTest LIBRARIES lardata_RecoObjects larreco_Deprecated )
cet_test( LATest LIBRARIES lardata_RecoObjects larreco_Deprecated )

install_headers()
install_fhicl()
install_source()
