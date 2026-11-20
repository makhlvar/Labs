#include "../include/PPMExporter.hpp"

void PPMExporter::writeWrapped(std::ostream& os, const std::string& token, int& lineLen) const {
    if (lineLen + static_cast<int>(token.length()) + 1 > 70) { 
        os << "\n"; 
        lineLen = 0; 
    }
    os << token << " ";
    lineLen += static_cast<int>(token.length()) + 1;
}

void PPMExporter::exportTo(std::ostream& os) const {
    os << "P3\n" << img.getWidth() << " " << img.getHeight() << "\n255\n";
    int lineLen = 0;
    for (int y = 0; y < img.getHeight(); ++y) {
        for (int x = 0; x < img.getWidth(); ++x) {
            const Color& c = img.getPixel(x, y);
            writeWrapped(os, std::to_string(static_cast<int>(c.r * 255)), lineLen);
            writeWrapped(os, std::to_string(static_cast<int>(c.g * 255)), lineLen);
            writeWrapped(os, std::to_string(static_cast<int>(c.b * 255)), lineLen);
        }
        os << "\n";
        lineLen = 0;
    }
}