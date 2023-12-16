#include <cassert>
#include <core/deltatime.h>

namespace Core {
    DeltaTime* DeltaTime::create_instance() {
        assert(instance == nullptr);
        instance = new DeltaTime;
        return instance;
    }

    void DeltaTime::destroy_instance() {
        assert(instance != nullptr);
        delete instance;
        instance = nullptr;
    }

    DeltaTime* DeltaTime::get_instance() {
        assert(instance != nullptr);
        return instance;
    }

    void DeltaTime::init() {
        deltatime = 0;
    }

    void DeltaTime::cleanup() {
        // do cleanup things
    }

    void DeltaTime::start_frame() {
        start = std::chrono::high_resolution_clock::now();
    }

    void DeltaTime::end_frame() {
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> frameDuration = end - start;
        deltatime = frameDuration.count();
    }

    double DeltaTime::get_delta_time() {
        return deltatime;
    }
}