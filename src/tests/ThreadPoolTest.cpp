#include "catch.hpp"
#include "../socknano.h"
#include <functional>
#include <atomic>

TEST_CASE("should run task", "[tp]")
{
    ThreadPool tp(1);

    std::atomic<bool> started(false);
    tp.SubmitTask([&started] { started = true; });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    tp.Shutdown();

    REQUIRE(started.load());
    REQUIRE(tp.isHalted());
}

TEST_CASE("should wait until task finish work", "[tp]")
{
    ThreadPool tp(1);

    std::atomic<bool> completed(false);
    tp.SubmitTask([&completed] {
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
        completed = true;
    });

    tp.Shutdown();

    REQUIRE(completed.load());
    REQUIRE(tp.isHalted());
}

TEST_CASE("should start eventually", "[tp]")
{
    int n = 5;

    ThreadPool tp(n);

    for (int i = 0; i < n; ++i)
    {
        tp.SubmitTask([] { std::this_thread::sleep_for(std::chrono::milliseconds(5000)); });
    }

    std::atomic<bool> started(false);

    std::function<void()> task = [&started] { started = true; };

    tp.SubmitTask(task);

    REQUIRE(!started.load());

    std::this_thread::sleep_for(std::chrono::milliseconds(6000));

    REQUIRE(started.load());

    tp.Shutdown();

    REQUIRE(tp.isHalted());
}