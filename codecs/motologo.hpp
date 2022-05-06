#ifndef MOTOLOGO_HPP
#define MOTOLOGO_HPP

#include "image.hpp"

inline bool is_motologo(const Image::Header & header)
{
    const std::array<unsigned char, 9> srf_header = {'M', 'o', 't', 'o', 'L', 'o', 'g', 'o', '\0'};

    return std::equal(std::begin(srf_header), std::end(srf_header), std::begin(header), Image::header_cmp);
}

class MotoLogo final: public Image
{
public:
    MotoLogo(std::istream & input, const Args & args);
    void handle_extra_args(const Args & args) override;
private:
    bool list_ {false};
    std::string image_name_ {"logo_boot"};
};
#endif // MOTOLOGO_HPP