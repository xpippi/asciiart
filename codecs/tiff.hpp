#ifndef TIFF_HPP
#define TIFF_HPP

#include "image.hpp"

inline bool is_tiff(const Image::Header & header)
{
    const std::array<unsigned char, 4> tiff_header1 = {0x49, 0x49, 0x2A, 0x00};
    const std::array<unsigned char, 4> tiff_header2 = {0x4D, 0x4D, 0x00, 0x2A};

    return std::equal(std::begin(tiff_header1), std::end(tiff_header1), std::begin(header), Image::header_cmp) ||
           std::equal(std::begin(tiff_header2), std::end(tiff_header2), std::begin(header), Image::header_cmp);
}

#ifdef TIFF_FOUND
class Tiff final: public Image
{
public:
    Tiff() = default;
    void open(std::istream & input, const Args & args) override;

    static void write(std::ostream & out, const Image & img, bool invert);
};
#endif
#endif // TIFF_HPP
