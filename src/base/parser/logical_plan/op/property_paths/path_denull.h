#ifndef BASE__PATH_DENULL_H_
#define BASE__PATH_DENULL_H_

#include <memory>

/*
PathDenull is used to delete redundant operators, in particular
removes extra KleeneStar, Optional and ? operators.

PathDenull only will be called when  OpPathKleeneStar or OpPathOptional
classes are constructed. Recursively look by a redundant operator
*/

class OpPath;
class OpPathAlternatives;
class OpPathSequence;
class OpPathAtom;
class OpPathKleeneStar;
class OpPathOptional;

class PathDenull {
public:
    static std::unique_ptr<OpPath> denull(std::unique_ptr<OpPath> path);

private:
    static std::unique_ptr<OpPath> accept_denull(std::unique_ptr<OpPath>);

    static std::unique_ptr<OpPath> denull(std::unique_ptr<OpPathAlternatives>);
    static std::unique_ptr<OpPath> denull(std::unique_ptr<OpPathSequence>);
    static std::unique_ptr<OpPath> denull(std::unique_ptr<OpPathAtom>);
    static std::unique_ptr<OpPath> denull(std::unique_ptr<OpPathKleeneStar>);
    static std::unique_ptr<OpPath> denull(std::unique_ptr<OpPathOptional>);
};

#endif // BASE__PATH_DENULL_H_
