#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <array>
#include <istream>
#include <memory>
#include <string>

unsigned char rgb_to_gray(unsigned char r, unsigned char g, unsigned char b);
float rgb_to_gray(float r, float g, float b);

// set to the size of the longest magic number
constexpr std::size_t max_header_len = 12; // 12 bytes needed to identify JPEGs

class Image
{
public:
    virtual ~Image() = default;
    virtual unsigned char get_pix(std::size_t row, std::size_t col) const = 0;
    virtual size_t get_width() const = 0;
    virtual size_t get_height() const = 0;
    using Header = std::array<char, max_header_len>;
};

class Header_buf: public std::streambuf
{
public:
    Header_buf(const Image::Header & header, std::istream & input);

protected:
    int underflow() override;

private:
    static const std::size_t buffer_size {2048};
    std::array<char, buffer_size> buffer_;
    std::istream & input_;
};

[[nodiscard]] std::unique_ptr<Image> get_image_data(std::string & input_filename, int bg);

#endif // IMAGE_HPP
