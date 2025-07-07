#pragma once

#ifndef NO_MDB_CLI

#include <chrono>

enum class Model { Quad, RDF, GQL };

int RunCLI(Model model, std::chrono::seconds timeout);
#endif
