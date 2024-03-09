#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class Valve {
private:
    bool state; // Valve state: open or closed

public:
    Valve() : state(false) {}

    void open() {
        state = true;
    }

    void close() {
        state = false;
    }

    bool isOpen() const {
        return state;
    }

    // Added function for stop simulation
    void stopSimulation() const {
        std::cout << "Valve stopped simulation." << std::endl;
    }
};

class FuelTank {
private:
    static int tankCount; // Static variable to count the number of tanks
    int tankID;
    double capacity;
    double fuel_quantity;
    bool broken;
    Valve valve;

public:
    FuelTank(double cap) : tankID(++tankCount), capacity(cap), fuel_quantity(0), broken(false) {}

    void addFuel(double quantity) {
        fuel_quantity = std::min(fuel_quantity + quantity, capacity);
    }

    void removeFuel(double quantity) {
        fuel_quantity = std::max(fuel_quantity - quantity, 0.0);
    }

    void breakTank() {
        broken = true;
    }

    void repairTank() {
        broken = false;
    }

    void openValve() {
        valve.open();
    }

    void closeValve() {
        valve.close();
    }

    double getFuelQuantity() const {
        return fuel_quantity;
    }

    bool isBroken() const {
        return broken;
    }

    bool isValveOpen() const {
        return valve.isOpen();
    }

    int getTankID() const {
        return tankID;
    }

    // Eklediðimiz fonksiyon
    double getCapacity() const {
        return capacity;
    }

    // Eklediðimiz statik fonksiyon
    static int getTankCount() {
        return tankCount;
    }

    // Added function for stop simulation
    void stopSimulation() const {
        std::cout << "Tank " << tankID << ": Simulation stopped." << std::endl;
    }
};

// Ýlk deðer atamasý
int FuelTank::tankCount = 0; // Initialize static variable

class Engine {
private:
    double fuel_per_second;
    bool status;
    FuelTank internalTank;
    FuelTank* connectedTank;

public:
    Engine(double fuelPerSecond) : fuel_per_second(fuelPerSecond), status(false), internalTank(55.0), connectedTank(nullptr) {}

    bool isConnected() const {
        return connectedTank != nullptr;
    }

    FuelTank* getConnectedTank() const {
        return connectedTank;
    }

    void startEngine() {
        if (connectedTank != nullptr) {
            status = true;
            std::cout << "Engine started." << std::endl;
        }
        else {
            std::cout << "Engine cannot start without a connected tank!" << std::endl;
        }
    }

    void stopEngine() {
        if (status) {
            status = false;
            std::cout << "Engine stopped." << std::endl;
            // Return remaining fuel to the connected tank with the minimum fuel
            if (internalTank.getFuelQuantity() > 0) {
                connectedTank->addFuel(internalTank.getFuelQuantity());
                std::cout << "Remaining fuel given back to Tank " << connectedTank->getTankID() << "." << std::endl;
                internalTank.removeFuel(internalTank.getFuelQuantity());
            }
        }
        else {
            std::cout << "Engine is not running." << std::endl;
        }
    }

    void connectTank(FuelTank* tank) {
        connectedTank = tank;
        std::cout << "Tank " << tank->getTankID() << " connected to the engine." << std::endl;
    }

    void disconnectTank() {
        if (connectedTank != nullptr) {
            std::cout << "Tank " << connectedTank->getTankID() << " disconnected from the engine." << std::endl;
            connectedTank = nullptr;
        }
        else {
            std::cout << "No tank connected to the engine." << std::endl;
        }
    }

    void absorbFuel() {
        if (internalTank.getFuelQuantity() > 20.0) {
            // Absorb fuel from a connected tank with minimum fuel
            double absorbedFuel = std::min(internalTank.getFuelQuantity() - 20.0, connectedTank->getFuelQuantity());
            connectedTank->removeFuel(absorbedFuel);
            internalTank.addFuel(absorbedFuel);
            std::cout << "Fuel absorbed from Tank " << connectedTank->getTankID() << "." << std::endl;
        }
    }

    void wait(int seconds) {
        if (status) {
            for (int i = 0; i < seconds; ++i) {
                internalTank.removeFuel(fuel_per_second);
            }
            std::cout << "Waited for " << seconds << " seconds." << std::endl;
        }
        else {
            std::cout << "Engine is not running. Cannot wait." << std::endl;
        }
    }

    void printTotalFuelQuantity() const {
        double totalFuel = internalTank.getFuelQuantity();
        if (connectedTank != nullptr) {
            totalFuel += connectedTank->getFuelQuantity();
        }
        std::cout << "Total fuel quantity: " << totalFuel << " units." << std::endl;
    }

    void printTotalConsumedFuelQuantity() const {
        double consumedFuel = 0.0;
        if (status) {
            consumedFuel = fuel_per_second;
        }
        std::cout << "Total consumed fuel quantity: " << consumedFuel << " units." << std::endl;
    }

    // Added function for stop simulation
    void stopSimulation() const {
        std::cout << "Engine: Simulation stopped." << std::endl;
        if (connectedTank != nullptr) {
            connectedTank->stopSimulation();
        }
    }
};

class TankManager {
private:
    std::vector<FuelTank> tankList;

public:
    void giveBackFuel(int tankID) {
        auto it = std::find_if(tankList.begin(), tankList.end(),
            [tankID](const FuelTank& tank) { return tank.getTankID() == tankID; });

        if (it != tankList.end()) {
            it->addFuel(it->getFuelQuantity());
            std::cout << "Remaining fuel given back to Tank " << tankID << "." << std::endl;
        }
        else {
            std::cout << "Tank " << tankID << ": Invalid tank ID." << std::endl;
        }
    }

    void addFuelTank(double capacity) {
        FuelTank newTank(capacity);
        tankList.push_back(newTank);
        std::cout << "Tank added with capacity " << capacity << "." << std::endl;
    }

    void listFuelTanks() const {
        for (const auto& tank : tankList) {
            std::cout << "Tank " << tank.getTankID() << ": Fuel Quantity - " << tank.getFuelQuantity()
                << ", Capacity - " << tank.getCapacity()
                << ", Broken - " << (tank.isBroken() ? "Yes" : "No")
                << ", Valve State - " << (tank.isValveOpen() ? "Open" : "Closed") << std::endl;
        }
    }

    void printFuelTankCount() const {
        std::cout << "Fuel tank count: " << FuelTank::getTankCount() << "." << std::endl;
    }

    void printFuelTank(int tankID) const {
        auto it = std::find_if(tankList.begin(), tankList.end(),
            [tankID](const FuelTank& tank) { return tank.getTankID() == tankID; });

        if (it != tankList.end()) {
            const auto& tank = *it;
            std::cout << "Tank " << tank.getTankID() << " Info:" << std::endl
                << "Tank ID: " << tank.getTankID() << std::endl
                << "Capacity: " << tank.getCapacity() << std::endl
                << "Fuel Quantity: " << tank.getFuelQuantity() << std::endl
                << "Broken: " << (tank.isBroken() ? "Yes" : "No") << std::endl
                << "Valve State: " << (tank.isValveOpen() ? "Open" : "Closed") << std::endl;
        }
        else {
            std::cout << "Tank " << tankID << ": Invalid tank ID." << std::endl;
        }
    }

    void removeFuelTank(int tankID) {
        auto it = std::find_if(tankList.begin(), tankList.end(),
            [tankID](const FuelTank& tank) { return tank.getTankID() == tankID; });

        if (it != tankList.end()) {
            tankList.erase(it);
            std::cout << "Tank " << tankID << " removed." << std::endl;
        }
        else {
            std::cout << "Tank " << tankID << ": Invalid tank ID." << std::endl;
        }
    }
    void connectFuelTankToEngine(Engine* engine, int tankID) {
        auto it = std::find_if(tankList.begin(), tankList.end(),
            [tankID](const FuelTank& tank) { return tank.getTankID() == tankID; });

        if (it != tankList.end()) {
            FuelTank* tank = &(*it);
            if (!engine->isConnected()) {
                engine->connectTank(tank);
            }
            else {
                std::cout << "Another tank is already connected to the engine." << std::endl;
            }
        }
        else {
            std::cout << "Tank " << tankID << ": Invalid tank ID." << std::endl;
        }
    }
};

class CommandReader {
private:
    TankManager tankManager;

public:
    void readCommands(const std::string& inputFilename, const std::string& outputFilename) {
        std::ifstream inputFile(inputFilename);
        std::ofstream outputFile(outputFilename);

        if (inputFile.is_open() && outputFile.is_open()) {
            Engine engine(5.5); // Create the engine with a fixed fuel_per_second value

            std::string command;
            while (inputFile >> command) {
                if (command == "start_engine;") {
                    engine.startEngine();
                }
                else if (command == "stop_engine;") {
                    engine.stopEngine();
                }
                else if (command.find("absorb_fuel") != std::string::npos) {
                    engine.absorbFuel();
                }
                else if (command.find("wait") != std::string::npos) {
                    int seconds;
                    inputFile >> seconds;
                    engine.wait(seconds);
                }
                else if (command.find("add_fuel_tank") != std::string::npos) {
                    double capacity;
                    inputFile >> capacity;
                    tankManager.addFuelTank(capacity);
                }
                else if (command.find("fill_tank") != std::string::npos) {
                    int tankID;
                    double quantity;
                    inputFile >> tankID >> quantity;
                    tankManager.giveBackFuel(tankID);
                }
                else if (command.find("connect_fuel_tank_to_engine") != std::string::npos) {
                    int tankID;
                    inputFile >> tankID;
                    tankManager.connectFuelTankToEngine(&engine, tankID);
                }
                else if (command.find("disconnect_fuel_tank_from_engine") != std::string::npos) {
                    engine.disconnectTank();
                }
                else if (command == "list_fuel_tanks;") {
                    tankManager.listFuelTanks();
                }
                else if (command == "print_fuel_tank_count;") {
                    tankManager.printFuelTankCount();
                }
                else if (command.find("list_connected_tanks;") != std::string::npos) {
                    std::cout << "Connected tanks: ";
                    if (engine.isConnected()) {
                        std::cout << engine.getConnectedTank()->getTankID();
                    }
                    else {
                        std::cout << "None";
                    }
                    std::cout << std::endl;
                }
                else if (command == "print_total_fuel_quantity;") {
                    engine.printTotalFuelQuantity();
                }
                else if (command == "print_total_consumed_fuel_quantity;") {
                    engine.printTotalConsumedFuelQuantity();
                }
                else if (command.find("print_tank_info") != std::string::npos) {
                    int tankID;
                    inputFile >> tankID;
                    tankManager.printFuelTank(tankID);
                }
                else if (command == "stop_simulation;") {
                    engine.stopSimulation();
                }
            }
        }
        else {
            std::cout << "Unable to open input or output file." << std::endl;
        }
    }
};

int main() {
    CommandReader reader;
    reader.readCommands("input.txt", "output.txt");
    return 0;
}
