#include <iostream>
#include <exception>
#include <vector>
#include <visa.h>
#include <string>
#include <chrono>
#include <thread>

#include <zaber.hpp> 
#include <velmex.hpp>
#include <ldc.hpp>


int main() {
    // 1. Initialize VISA Resource Manager
    ViSession defaultRM = VI_NULL;
    ViStatus status = viOpenDefaultRM(&defaultRM);
    if (status < VI_SUCCESS) {
        std::cerr << "Failed to initialize VISA Resource Manager [Error Code: " << status << "]\n";
        return 1;
    }

    try {



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

            std::cout << "Velmex 1: " << vxm.query("X") << std::endl;

            std::cout << "Velmex 2: " << vxm.query("Y") << std::endl;

            std::cout << "Velmex 3: " << vxm.query("Z") << std::endl;

            std::cout << "Velmex 4" << vxm.query("T") << std::endl;


        }



       /* {

            Ldc502 laserController(defaultRM, "GPIB0::3::INSTR");

            std::cout << "Laser Controller Identity: " << laserController.getIdentity() << std::endl;


            laserController.setCurrentSetpoint(250.0);
            laserController.getCurrentSetpoint();

			if (laserController.isLaserOutputEnabled()) {
				std::cout << "Laser output is enabled.\n";
			}
			else {
				std::cout << "Laser output is disabled. Enabling now...\n";
				laserController.setLaserOutput(true);
			}

            std::cout << "The laser status:" << laserController.isLaserOutputEnabled() << std::endl;
            std::cout << "The live current reading:" << laserController.getActualCurrent()  << std::endl;




        }

        */


        // =================================================================
        // Initialize Instruments
        // Zaber: Serial connection (e.g. ASRL5::INSTR)
        // =================================================================
        
/*
        {
            std::cout << "--- Connecting to Zabers ---\n";
            Zaber motionStage(defaultRM, "ASRL5::INSTR");


            int numberOfDevices = 4; // Query first 2 daisy-chained axes 

            std::vector<ZaberAxisLimits> limits = motionStage.queryLimitsForAllDevices(numberOfDevices);



            for (const auto& axis : limits) {
                std::cout << "Axis " << axis.index << ":\n"
                    << "  Min Limit  : " << axis.minPosition << " microsteps\n"
                    << "  Max Limit  : " << axis.maxPosition << " microsteps\n"
                    << "  Resolution : " << axis.resolution << " microsteps/step\n";
            }
            std::cout << "\n";

           

			motionStage.getLimitMax(1);

            // Fetch and display positions for ALL devices 
            // This utilizes the std::cout loop built into Zaber::getPosition


            std::this_thread::sleep_for(std::chrono::milliseconds(2000));




			motionStage.getPosition(numberOfDevices);
           


            std::this_thread::sleep_for(std::chrono::seconds(2));
           

            
           // motionStage.moveAbsolute(1, motionStage.getLimitMax(1)); // Home the first axis


            std::this_thread::sleep_for(std::chrono::seconds(5));

          


           // motionStage.moveAbsolute(1, motionStage.getLimitMax(1));
            motionStage.moveAbsolute(2, 0); // Home the first axis
            motionStage.moveAbsolute(3, 0); // Home the first axis
            motionStage.moveAbsolute(4, 0); // Home the first axis
            


            std::this_thread::sleep_for(std::chrono::seconds(5));

            motionStage.getPosition(numberOfDevices);


            motionStage.home(0); // Home the first axis


        }
*/




    }
    catch (const std::exception& ex) {
        std::cerr << "\n[Runtime Exception]: " << ex.what() << "\n";
    }

    // Clean up VISA Resource Manager
    viClose(defaultRM);
    return 0;
}