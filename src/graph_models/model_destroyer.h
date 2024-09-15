#pragma once

#include <functional>

class ModelDestroyer {
public:
    ModelDestroyer(std::function<void()> destroyer) : destroyer(destroyer) { }

    ~ModelDestroyer() {
        destroyer();
    }

private:
    std::function<void()> destroyer;
};
