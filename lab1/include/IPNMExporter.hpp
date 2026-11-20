#pragma once
#include <ostream>

class IPNMExporter {
public:
    virtual ~IPNMExporter() = default;
    virtual void exportTo(std::ostream& os) const = 0;
};