#include "proctor_engine.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {

    try {
        std::setvbuf(stdout, NULL, _IONBF, 0);
        // std::setvbuf(stdout, nullptr, _IONBF, 0); // Disable buffering

        std::cout << "✅ ProctorEngine CLI launched\n";

        // Print CLI arguments
        for (int i = 0; i < argc; ++i) {
            std::cout << "[ARG " << i << "] " << argv[i] << std::endl;
        }

        // Expecting: ./proctor_engine <userId> <examId> <eventId>
        if (argc < 4) {
            std::cerr << "❌ Missing required arguments.\n";
            std::cerr << "Usage: ./proctor_engine <userId> <examId> <eventId>\n";
            return 1;
        }

        std::string binaryPath = argv[0]; // 👈 path to the executable itself
        std::string userId     = argv[1];
        std::string examId     = argv[2];
        std::string eventId    = argv[3];

        // Endpoint where events will be emitted
        std::string socketUrl = "http://localhost:8383/api/v1i2/proctor/emit-event";

        ProctorEngine engine(userId, examId, socketUrl);

        // ✅ Initialize Engine with binaryPath
        if (!engine.initialize(binaryPath)) {
            std::cerr << "❌ Failed to initialize engine.\n";
            return 1;
        }

        // 🚀 Start Engine
        engine.start();

        // ⏳ Keep process alive while engine is running
        while (engine.isRunning()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "✅ Engine completed successfully.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "🚨 Unhandled exception in main: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "🚨 Unknown fatal error in main.\n";
    }

    return 0;
}
