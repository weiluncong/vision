#include "csda_rle_decode.h"

namespace cav
{

  CRLECompressedImage::~CRLECompressedImage()
  {
    if (data_)
    {
      delete[] data_;
      data_ = NULL;
    }
  }

  int CRLECompressedImage::Decompress(const uint8_t *input_data,
                                      int size, uint8_t *output_data,
                                      int &buf_len, uint16_t align)
  {
    if (!input_data || size == 0)
    {
      return Decompress_Failed;
    }
    const uint8_t *ptr = input_data;
    // Skip the magic number first
    ptr += 4;
    // Then the height and width
    uint16_t height =
        static_cast<uint16_t>(static_cast<uint16_t>(*ptr++ << 8) & 0xFF00);
    height |= static_cast<uint16_t>(*ptr++);
    uint16_t width =
        static_cast<uint16_t>(static_cast<uint16_t>(*ptr++ << 8) & 0xFF00);
    width |= static_cast<uint16_t>(*ptr++);

    if (height <= 0 || width <= 0)
    {
      return Decompress_Failed;
    }

    uint16_t step = width;
    uint16_t padding_value = 0;
    if (align > 0)
    {
      step = (width + align - 1) / align * align;
      padding_value = step - width;
    }

    if (buf_len < height * step)
    {
      return Decompress_BuffTooSmall;
    }

    buf_len = height * step;

    // Then the data
    int x = 0;
    int y = 0;
    int rle_cnt = (size - 8) / 3;
    uint8_t *p_im_data = output_data;
    int offset = 0;
    for (int i = 0; i < rle_cnt; ++i)
    {
      uint8_t label = *ptr++;
      uint16_t cnt = *reinterpret_cast<const uint16_t *>(ptr);
      ptr += 2;

      x += cnt;
      if (x > width)
      {
        return Decompress_Failed;
      }
      if (y > height)
      {
        return Decompress_Failed;
      }
      offset += static_cast<int>(cnt);
      if (offset > buf_len)
      {
        return Decompress_Failed;
      }

      for (int j = 0; j < cnt; ++j)
      {
        *p_im_data++ = label;
      }

      if (x == width)
      {
        y++;
        x = 0;
        p_im_data += padding_value;
        offset += padding_value;
        if (offset > buf_len)
        {
          return Decompress_Failed;
        }
      }
    }

    return Decompress_OK;
  }
}