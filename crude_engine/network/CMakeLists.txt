target_sources(crude_engine
    PRIVATE
    socket_address_factory.cpp
    socket_address.cpp
    socket_util.cpp
    tcp_socket.cpp
    udp_socket.cpp
    network_system.cpp)

target_sources(crude_engine
    PUBLIC
    FILE_SET cxx_modules_network TYPE CXX_MODULES FILES
    socket_address_factory.ixx
    socket_address.ixx
    socket_util.ixx
    tcp_socket.ixx
    udp_socket.ixx
    network_system.ixx)
