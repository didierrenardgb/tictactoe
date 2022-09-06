#include <chrono>

#include "App.h"

int main(int, char*[])
{
    ttt::App app(std::chrono::milliseconds(500), { 0.45f, 0.55f, 0.60f, 1.00f });
    return app.run();
}
