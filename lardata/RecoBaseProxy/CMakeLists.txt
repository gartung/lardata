add_subdirectory(ProxyBase)

art_make(LIB_LIBRARIES lardataobj_RecoBase ${ROOT_Core_LIBRARY} )

install_headers()
install_source()
