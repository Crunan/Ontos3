#include "mainloop.h"
#include <mutex>

MainLoop::MainLoop()
{

}


void MainLoop::Tick() {
    static std::mutex mutex;
    static bool mainLoopRunning = false;

    std::lock_guard<std::mutex> lock(mutex);

    if (mainLoopRunning)
        return;

    mainLoopRunning = true;

    runstateMachine();

    mainLoopRunning = false;
}
