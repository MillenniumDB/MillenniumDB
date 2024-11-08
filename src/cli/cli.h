#pragma once

#include <chrono>

enum class Model { Quad, RDF };

int RunCLI(Model model, std::chrono::seconds timeout);
