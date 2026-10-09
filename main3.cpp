#include <vrm.hpp>
#include <ipg.hpp>
#include <remoteControl.hpp>
#include <zaber.hpp>
#include <ldc.hpp>
#include <ic4_camera.hpp>
#include <velmex.hpp>

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    try {

// IPG YLR Laser

       /* {
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
        } // End of Laser scope (resources clean up here if applicable)

        */


// Velmex
      {

                VisaResourceManager rm;


                ChannelSettings vxmSettings;
                vxmSettings.timeoutMs = 1500;
                vxmSettings.terminationCharacter = '\r'; // Enforces \r termination for viRead()
                vxmSettings.baudRate = 9600;             // Velmex VXM default
                vxmSettings.dataBits = 8;
                vxmSettings.stopBits = VI_ASRL_STOP_ONE;
                vxmSettings.parity = VI_ASRL_PAR_NONE;
                vxmSettings.flowControl = VI_ASRL_FLOW_NONE;

                // Open Velmex session with explicit ChannelSettings
                VelmexVXM vxm(rm.handle(), "ASRL10::INSTR", vxmSettings);

                std::cout << "Velmex Status:" << vxm.isOpen() << std::endl;


                std::cout<< "Velmex Identity: " << vxm.query("X") << std::endl;

                std::cout << "Velmex Identity: " << vxm.query("Y") << std::endl;

                std::cout << "Velmex Identity: " << vxm.query("Z") << std::endl;







        }





// SRS LDC 502
        {
            VisaResourceManager rm;

            std::string ldcResource = "GPIB0::2::INSTR";
            Ldc502 ldc(rm.handle(), ldcResource);

            if (!ldc.isOpen()) {
                std::cerr << "Failed to connect to LDC laser.\n";
                return 1;
            }

            std::cout << "Connected! Identity: " << ldc.getIdentity() << "\n";
            std::cout << "Laser Status: " << ldc.getStatus() << "\n";
        } // End of Laser scope (resources clean up here if applicable)
     




// Zaber
       

        

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    std::cout << "Application exiting." << std::endl;
    return 0;
}