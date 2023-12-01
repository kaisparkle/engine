#include <core/engine.h>

int main() {
    Core::Engine* engine = Core::Engine::create_instance();
    engine->init();
    engine->run();
    engine->cleanup();
    Core::Engine::destroy_instance();
}
