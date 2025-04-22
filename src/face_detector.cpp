#include "face_detector.h"
#include "utils.h"
#include <opencv2/dnn.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>

FaceDetector::FaceDetector() : isRunning_(false) {}

FaceDetector::~FaceDetector() {
    stopCapture();
    utils::log("🧹 FaceDetector destructor complete.");
}

#include <fstream>


bool FaceDetector::initialize(const std::string& binaryPath) {
    utils::log("🔥 [DEBUG] Entered FaceDetector::initialize()");

    try {
        // Start from where the binary is
        std::filesystem::path execDir = std::filesystem::path(binaryPath).parent_path();
        utils::log("🧭 Binary located at: " + execDir.string());

        // Climb up from electron/bin/mac → electron/bin → electron → gyapak-test-series → ai-proctor-engine/models
        std::filesystem::path modelDir = execDir.parent_path().parent_path() / "ai-proctor-engine/models";
        modelDir = std::filesystem::weakly_canonical(modelDir);
        utils::log("📂 Resolved model directory: " + modelDir.string());

        std::string protoPath = (modelDir / "deploy.prototxt").string();
        std::string modelPath = (modelDir / "res10_300x300_ssd_iter_140000.caffemodel").string();

        utils::log("📂 Checking model files:");
        utils::log("   Prototxt: " + protoPath);
        utils::log("   CaffeModel: " + modelPath);

        if (!std::filesystem::exists(protoPath)) {
            utils::log("❌ Prototxt file NOT FOUND: " + protoPath);
            return false;
        }

        if (!std::filesystem::exists(modelPath)) {
            utils::log("❌ CaffeModel file NOT FOUND: " + modelPath);
            return false;
        }

        utils::log("✅ Model files exist. Loading...");

        faceNet_ = cv::dnn::readNetFromCaffe(protoPath, modelPath);

        if (faceNet_.empty()) {
            utils::log("❌ OpenCV failed to load the model (network is empty)");
            return false;
        }

        utils::log("✅ Face detection model loaded successfully");
        return true;

    } catch (const std::exception& e) {
        utils::log("❌ Exception in FaceDetector initialization: " + std::string(e.what()));
        return false;
    }
}

bool FaceDetector::startCapture() {
    if (isRunning_.exchange(true)) return true;

    try {
        capture_.open(0);
        if (!capture_.isOpened()) {
            utils::log("❌ Could not open camera");
            isRunning_ = false;
            return false;
        }

        CaptureThread_ = std::thread(&FaceDetector::captureLoop, this);
        utils::log("📸 Capture thread started.");
        return true;
    } catch (const std::exception& e) {
        utils::log("Exception in starting faceDetector: " + std::string(e.what()));
        isRunning_ = false;
        return false;
    }
}


void FaceDetector::stopCapture() {
    if (!isRunning_.exchange(false)) return;

    if (CaptureThread_.joinable()) {
        CaptureThread_.join();
        utils::log("📥 Capture thread joined.");
    }

    if (capture_.isOpened()) {
        capture_.release();
    }

    utils::log("🛑 Face detector stopped and cleaned up.");
}

void FaceDetector::captureLoop() {
    while (isRunning_) {
        try {
            cv::Mat frame;
            if (!capture_.read(frame) || frame.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            {
                std::lock_guard<std::mutex> lock(frameMutex_);
                currentFrame_ = frame.clone();
            }

            frameReady_.notify_one();  // Notify consumer
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        } catch (const std::exception& e) {
            utils::log("Exception in capture loop: " + std::string(e.what()));
        }
    }
}

int FaceDetector::detectFaces() {
    try {
        cv::Mat frame;

        {
            std::unique_lock<std::mutex> lock(frameMutex_);
            frameReady_.wait(lock, [this]() { return !currentFrame_.empty(); });  // Wait for fresh frame
            frame = currentFrame_.clone();
        }

        // Prepare input blob for DNN
        cv::Mat blob = cv::dnn::blobFromImage(
            frame, 1.0, cv::Size(300, 300),
            cv::Scalar(104.0, 177.0, 123.0), false, false
        );

        faceNet_.setInput(blob);
        cv::Mat output = faceNet_.forward();

        cv::Mat detections(output.size[2], output.size[3], CV_32F, output.ptr<float>());

        int faceCount = 0;
        for (int i = 0; i < detections.rows; ++i) {
            float confidence = detections.at<float>(i, 2);
            if (confidence > confidenceThreshold_) {
                faceCount++;
            }
        }

        return faceCount;
    } catch (const std::exception& e) {
        utils::log("Exception in detectFaces: " + std::string(e.what()));
        return 0;
    }
}

cv::Mat FaceDetector::getCurrentFrame() {
    std::lock_guard<std::mutex> lock(frameMutex_);
    return currentFrame_.clone();
}
