#include <vrm.hpp>
#include <ipg.hpp>
#include <iostream>

int main() {
    try {
        VisaResourceManager rm;

        // Example TCPIP socket resource string for the YLR laser
        std::string ylrResource = "TCPIP0::192.168.3.232::10001::SOCKET";
        ipgYLR laser(rm.handle(), ylrResource);

        if (!laser.isOpen()) {
            std::cerr << "Failed to connect to YLR laser.\n";
            return 1;
        }

        std::cout << "Connected! Identity: " << laser.getIdentity() << "\n";
        std::cout << "Laser Status: " << laser.getStatus() << "\n";

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}