#pragma once
#include "IPNMExporter.hpp"
#include "Image.hpp"
#include <string>

class PPMExporter : public IPNMExporter {
private:
    const Image& img;
    void writeWrapped(std::ostream& os, const std::string& token, int& lineLen) const;
public:
    PPMExporter(const Image& image) : img(image) {}
    void exportTo(std::ostream& os) const override;
};