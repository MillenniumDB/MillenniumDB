#pragma once

#include <memory>

// DenullPath is used to simplify some regular expressions
// PathDenull::denull() is called when a PathKleeneStar is constructed, and it
// recursively looks for nullable expressions to simplify (nullable means
// epsilon is accepted by the expression) applying these rules:

// DENULL(EXPR*) => EXPR
// DENULL(EXPR?) => EXPR
// DENULL(E1 / E2) => DENULL(E1) | DENULL(E2) if E1 and E2 are nullables
//                 => E1 / E2                 if E1 and E2 are not nullables
// DENULL(E1 | E2) => DENULL(E1) | DENULL(E2) if E1 or E2 is nullable
//                 => E1 | E2                 if E1 or E2 are not nullables
// E.g:
// (A*)* => A*
// (A?)* => A*
// (A* / B*)* => (A | B)*
// (A* | B*)* => (A | B)*

// There is another simplification when constructing a PathOptional
// but it's not the responsibility of PathDenull to do this.
// When we have EXPR? and EXPR is nullable, we remove the optional
// E.g:
// (A*)? => A*
// (A* | B?)? => A* | B?

class IPath;
class PathAlternatives;
class PathAtom;
class PathCheck;
class PathKleeneStar;
class PathOptional;
class PathSequence;

class PathDenull {
public:
    static std::unique_ptr<IPath> denull(std::unique_ptr<IPath> path);

private:
    static std::unique_ptr<IPath> accept_denull(std::unique_ptr<IPath>);

    static std::unique_ptr<IPath> denull(std::unique_ptr<PathAlternatives>);
    static std::unique_ptr<IPath> denull(std::unique_ptr<PathAtom>);
    static std::unique_ptr<IPath> denull(std::unique_ptr<PathCheck>);
    static std::unique_ptr<IPath> denull(std::unique_ptr<PathKleeneStar>);
    static std::unique_ptr<IPath> denull(std::unique_ptr<PathOptional>);
    static std::unique_ptr<IPath> denull(std::unique_ptr<PathSequence>);
};
