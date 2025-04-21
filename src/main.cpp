#include "proctor_engine.h"
#include <iostream>
#include <thread>
#include <chrono>

// int main() {
//     try {
//         ProctorEngine engine("67f6a65f334e40ba01ab2924", "exam123", "http://localhost:8383/api/v1i2/proctor/emit-event");

//         // Initialize Proctor Engine
//         if (!engine.initialize()) {
//             std::cerr << "❌ Failed to initialize engine.\n";
//             return 1;
//         }

//         // Start and run engine for 10 seconds
//         engine.start();
//         // std::this_thread::sleep_for(std::chrono::seconds(10));
//         engine.stop();

//         std::cout << "✅ Engine completed successfully.\n";
//     }
//     catch (const std::exception& e) {
//         std::cerr << "🚨 Unhandled exception in main: " << e.what() << std::endl;
//     }
//     catch (...) {
//         std::cerr << "🚨 Unknown fatal error in main." << std::endl;
//     }

//     return 0;
// }


int main() {
    try {
        ProctorEngine engine("67f6a65f334e40ba01ab2924", "exam123", "http://localhost:8383/api/v1i2/proctor/emit-event");

        // Initialize Proctor Engine
        if (!engine.initialize()) {
            std::cerr << "❌ Failed to initialize engine.\n";
            return 1;
        }

        // Start the engine and wait for it to finish (auto timeout in monitorLoop)
        engine.start();

        // ⏳ Wait until engine naturally ends (5-min timeout or signal)
        while (engine.isRunning()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "✅ Engine completed successfully.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "🚨 Unhandled exception in main: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "🚨 Unknown fatal error in main." << std::endl;
    }

    return 0;
}
