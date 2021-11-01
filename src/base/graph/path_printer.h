#ifndef BASE__PATH_PRINTER_H_
#define BASE__PATH_PRINTER_H_

#include <ostream>


class PathPrinter {
public:
    virtual ~PathPrinter() = default;

    virtual void print(std::ostream& os, uint64_t path_id) const = 0;
};

#endif // BASE__PATH_PRINTER_H_
