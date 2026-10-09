#include <vrm.hpp>
#include <ipg.hpp>
#include <arroyo.hpp>
#include <iostream>
#include <stdexcept>

int main() {
    try {
        VisaResourceManager rm;


		const std::string ipgResource = "TCPIP0::192.168.3.232:INSTR";
        ipgYLR ipg(rm.handle(), ipgResource);


        if (!ipg.isOpen()) {
            throw std::runtime_error("Could not open the IPG VISA session.");
        }


		ipg.getIdentity();





        const std::string arroyoResource = "ASRL4::INSTR";
        Arroyo5240 arroyo(rm.handle(), arroyoResource);

        if (!arroyo.isOpen()) {
            throw std::runtime_error("Could not open the Arroyo VISA session.");
        }

        std::cout << "Instrument: " << arroyo.getIdentity();
        std::cout << "Beep response: " << arroyo.beep();

        arroyo.setTemperatureSetpoint(30.0);
        std::cout << "Temperature setpoint: " << arroyo.getTemperatureSetpoint() << " C\n";
        std::cout << "Current Temperature: " << arroyo.getCurrentTemperature() << " C\n";

    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    std::cout << "VISA program ended" << std::endl;
    return 0;
}