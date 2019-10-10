#include "gif.hpp"

#include <iostream>

Gif::Gif(const Header & header, std::istream & input, unsigned char bg):
    header_{header},
    input_{input}
{
    int error_code = GIF_OK;
    GifFileType * gif = DGifOpen(this, read_fn, &error_code);
    if(!gif)
        throw std::runtime_error{"Error setting up GIF: " + std::string{GifErrorString(error_code)}};

    if(DGifSlurp(gif) != GIF_OK)
    {
        DGifCloseFile(gif, NULL);
        throw std::runtime_error{"Error reading GIF: " + std::string{GifErrorString(gif->Error)}};
    }

    auto pal = gif->SavedImages[0].ImageDesc.ColorMap;
    if(!pal)
    {
        pal = gif->SColorMap;
        if(!pal)
        {
            DGifCloseFile(gif, NULL);
            throw std::runtime_error{"Could not find color map"};
        }
    }

    std::vector<unsigned char> gray_pal(pal->ColorCount);
    for(std::size_t i = 0; i < std::size(gray_pal); ++i)
    {
        gray_pal[i] = rgb_to_gray(
            pal->Colors[i].Red,
            pal->Colors[i].Green,
            pal->Colors[i].Blue
            );
    }

    int transparency_ind = -1;
    GraphicsControlBlock gcb;
    if(DGifSavedExtensionToGCB(gif, 0, &gcb) == GIF_OK)
        transparency_ind = gcb.TransparentColor;

    width_ = gif->SWidth;
    height_ = gif->SHeight;

    if(gif->SavedImages[0].ImageDesc.Left != 0 || gif->SavedImages[0].ImageDesc.Top != 0
            || static_cast<std::size_t>(gif->SavedImages[0].ImageDesc.Width) != width_
            || static_cast<std::size_t>(gif->SavedImages[0].ImageDesc.Height) != height_)
    {
        throw std::runtime_error{"GIF has wrong size or offset"};
    }

    auto & im = gif->SavedImages[0].RasterBits;

    image_data_.resize(height_);
    for(auto && row: image_data_)
    {
        row.resize(width_);
    }

    for(std::size_t row = 0; row < height_; ++row)
    {
        for(std::size_t col = 0; col < width_; ++col)
        {
            auto index = im[row * width_ + col];

            auto val = gray_pal[index];

            if(index == transparency_ind)
                val = bg;

            image_data_[row][col] = val;
        }
    }

    DGifCloseFile(gif, NULL);
}

int Gif::read_fn(GifByteType * data, int length) noexcept
{
    std::size_t gif_ind = 0;
    while(header_bytes_read_ < std::size(header_) && gif_ind < static_cast<std::size_t>(length))
        data[gif_ind++] = header_[header_bytes_read_++];

    input_.read(reinterpret_cast<char *>(data) + gif_ind, length - gif_ind);
    if(input_.bad())
    {
        std::cerr<<"FATAL ERROR: Could not read GIF image\n";
        return GIF_ERROR;
    }

    return input_.gcount() + gif_ind;
}

int Gif::read_fn(GifFileType* gif_file, GifByteType * data, int length) noexcept
{
    auto gif = static_cast<Gif*>(gif_file->UserData);
    if(!gif)
    {
        std::cerr<<"FATAL ERROR: Could not get GIF struct pointer\n";
        return GIF_ERROR;
    }

    return gif->read_fn(data, length);
}
