#pragma once

#include <chrono>

namespace Core {
    class DeltaTime {
    public:
        // create the DeltaTime singleton
        static DeltaTime* create_instance();
        // destroy the DeltaTime singleton
        static void destroy_instance();
        // return the DeltaTime singleton
        static DeltaTime* get_instance();

        // initialize DeltaTime
        void init();
        // clean up DeltaTime
        void cleanup();

        // start timing a new frame
        void start_frame();
        // end timing the frame
        void end_frame();
        // return the current delta time
        double get_delta_time();
    private:
        inline static DeltaTime* instance;

        std::chrono::system_clock::time_point start;
        std::chrono::system_clock::time_point end;
        double deltatime;
    };
}