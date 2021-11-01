#ifndef BASE__BINDING_ITER_H_
#define BASE__BINDING_ITER_H_

#include <ostream>

#include "base/binding/binding.h"

// Abstract class
class BindingIter {
public:
    virtual ~BindingIter() = default;

    // returns the position where all the results will be written
    virtual Binding& get_binding() noexcept = 0;

    // begin has to be called before calling next()
    virtual void begin() = 0;

    // Returns true if there are more bindings or false otherwise.
    // If true is returned the reference to the Binding returned in `get_binding()` is updated.
    virtual bool next() = 0;

    // prints execution statistics into an ostream
    virtual void analyze(std::ostream&, int indent = 0) const = 0;
};

#endif // BASE__BINDING_ITER_H_
