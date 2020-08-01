#ifndef HEIF_HPP
#define HEIF_HPP

#include "image.hpp"

#include <algorithm>

inline bool is_heif(const Image::Header & header)
{
    const std::array<unsigned char, 4> ftyp_header = {'f', 't', 'y', 'p'};
    const std::array<std::array<unsigned char, 4>, 8> brand = {{
        {'h', 'e', 'i', 'c'},
        {'h', 'e', 'i', 'x'},
        {'h', 'e', 'v', 'c'},
        {'h', 'e', 'v', 'x'},
        {'h', 'e', 'i', 'm'},
        {'h', 'e', 'i', 's'},
        {'h', 'e', 'v', 'm'},
        {'h', 'e', 'v', 's'},
    }};

    if(!std::equal(std::begin(ftyp_header), std::end(ftyp_header), std::begin(header) + 4, Image::header_cmp))
        return false;

    return std::any_of(std::begin(brand), std::end(brand), [&header](const auto & b)
            {
                return std::equal(std::begin(b), std::end(b), std::begin(header) + 8, Image::header_cmp);
            });
}

#ifdef HEIF_FOUND
class Heif final: public Image
{
public:
    explicit Heif(std::istream & input);
};
#endif
#endif // HEIF_HPP
