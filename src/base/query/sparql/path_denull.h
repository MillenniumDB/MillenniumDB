#pragma once

#include <memory>

namespace SPARQL {
class IPath;
class PathAlternatives;
class PathSequence;
class PathAtom;
class PathKleeneStar;
class PathOptional;

class PathDenull {
public:
    static std::unique_ptr<IPath> denull(std::unique_ptr<IPath> path);

private:
    static std::unique_ptr<IPath> accept_denull(std::unique_ptr<IPath>);

    static std::unique_ptr<IPath> denull(std::unique_ptr<PathAlternatives>);
    static std::unique_ptr<IPath> denull(std::unique_ptr<PathSequence>);
    static std::unique_ptr<IPath> denull(std::unique_ptr<PathAtom>);
    static std::unique_ptr<IPath> denull(std::unique_ptr<PathKleeneStar>);
    static std::unique_ptr<IPath> denull(std::unique_ptr<PathOptional>);
};
} // namespace SPARQL