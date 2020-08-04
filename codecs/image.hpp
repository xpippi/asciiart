#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <array>
#include <istream>
#include <memory>
#include <vector>

#include "../args.hpp"
#include "../color.hpp"
#include "exif.hpp"

// set to the size of the longest magic number
constexpr std::size_t max_header_len = 12; // 12 bytes needed to identify JPEGs

class Image
{
public:
    virtual ~Image() = default;

    void swap(Image & other)
    {
        std::swap(width_, other.width_);
        std::swap(height_, other.height_);
        std::swap(image_data_, other.image_data_);
    }

    const std::vector<Color> & operator[](std::size_t i) const
    {
        return image_data_[i];
    }
    std::size_t get_width() const { return width_; }
    std::size_t get_height() const { return height_; }

    using Header = std::array<char, max_header_len>;
    static bool header_cmp(unsigned char a, char b);
    static std::vector<unsigned char> read_input_to_memory(std::istream & input);

    Image scale(std::size_t new_width, std::size_t new_height) const;

    void convert(const Args & args) const;

protected:
    void set_size(std::size_t w, std::size_t h);
    void transpose_image(exif::Orientation orientation);

    std::size_t width_{0};
    std::size_t height_{0};
    std::vector<std::vector<Color>> image_data_;
};

[[nodiscard]] std::unique_ptr<Image> get_image_data(const Args & args);

#endif // IMAGE_HPP
