#include "bmp.hpp"

#include <stdexcept>

#include <cstdint>

#include "bmp_common.hpp"

Bmp::Bmp(std::istream & input)
{
    input.exceptions(std::ios_base::badbit | std::ios_base::failbit);
    try
    {
        std::size_t file_pos {0}; // need to keep track of where we are in the file because in.tellg() fails on at least some pipe inputs

        bmp_data bmp;
        read_bmp_file_header(input, bmp, file_pos);
        read_bmp_info_header(input, bmp, file_pos);

        set_size(bmp.width, bmp.height);

        read_bmp_data(input, bmp, file_pos, image_data_);
    }
    catch(std::ios_base::failure&)
    {
        if(input.bad())
            throw std::runtime_error{"Error reading BMP: could not read file"};
        else
            throw std::runtime_error{"Error reading BMP: unexpected end of file"};
    }
}
