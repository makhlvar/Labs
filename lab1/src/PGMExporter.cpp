#include "../include/PGMExporter.hpp"

void PGMExporter::writeWrapped(std::ostream& os, const std::string& token, int& lineLen) const {
    if (lineLen + static_cast<int>(token.length()) + 1 > 70) { 
        os << "\n"; 
        lineLen = 0; 
    }
    os << token << " ";
    lineLen += static_cast<int>(token.length()) + 1;
}

void PGMExporter::exportTo(std::ostream& os) const {
    os << "P2\n" << img.getWidth() << " " << img.getHeight() << "\n255\n";
    int lineLen = 0;
    for (int y = 0; y < img.getHeight(); ++y) {
        for (int x = 0; x < img.getWidth(); ++x) {
            writeWrapped(os, std::to_string(img.getPixel(x, y).to_grayscale()), lineLen);
        }
        os << "\n";
        lineLen = 0;
    }
}