/**
 *Copyright 2022 CA.
 */

#ifndef CAPILOT_IMAGE_FRAME_H
#define CAPILOT_IMAGE_FRAME_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

namespace cav {

/**
 * 多路数据解码使用Demo:
 * 1、首先从Dat文件或者通过网络订阅的Topic中读取出Dat帧，如DatFrame;
 * 2、创建CapilotMultiImageFrame类型的对象image_frames;
 * 3、用image_frames对象中的方法解码DatFrame;
 * 4、根据图像通道索引，获取CapilotImageFrame对象
 *
 * Pseudocode:
 * CapilotMultiImageFrame image_frames;
 * bool ret = image_frames.decode_frame(DatFrame->data(), DatFrame->size());
 * if (ret) {
 *  // 获取图像总路数
 *  image_frames.frame_count();
 *  // 获取第0路图像
 *  cosnt CapilotImageFrame &frame0 = image_frames.get_frame(0);
 *  // 获取当前路图像帧ID
 *  frame0.frame_id();
 *  // 获取当前路图像时间戳
 *  frame0.timestamp();
 *  // 获取图像数据或图像大小
 *  frame0.image_data();
 *  frame0.image_size();
 * } else {
 *  // error 错误处理
 * }
 */

class CapilotImageFrame {
 public:
  CapilotImageFrame() {
    data_ = std::make_shared<std::vector<uint8_t> >();
    data_->resize(sizeof(int32_t) + sizeof(int64_t));
    set_frame_id(0);
    set_timestamp(0);
  }

  /**
   * @brief
   * @param[img] 图像起始地址
   * @param[len] 图像大小
   * @return
   */
  CapilotImageFrame(const void *img, size_t len) {
    data_ = std::make_shared<std::vector<uint8_t> >();
    data_->resize(sizeof(int32_t) + sizeof(int64_t) + len);
    set_frame_id(0);
    set_timestamp(0);
    ::memcpy(image_data(), img, len);
  }

  /**
   * @brief
   * @param[frame_id] 图像帧ID
   * @param[timestamp] 图像时间戳
   * @param[img] 图像起始地址
   * @param[len] 图像大小
   * @return
   */
  CapilotImageFrame(int32_t frame_id, int64_t timestamp, const void *img,
                    size_t len) {
    data_ = std::make_shared<std::vector<uint8_t> >();
    data_->resize(sizeof(int32_t) + sizeof(int64_t) + len);

    int32_t *id = frame_id_addr();
    *id = frame_id;
    int64_t *ts = timestamp_addr();
    *ts = timestamp;
    ::memcpy(image_data(), img, len);
  }

  /**
   * @brief 获取图像帧ID
   * @param
   * @return 图像帧ID
   */
  int32_t frame_id() { return *frame_id_addr(); }

  /**
   * @brief 设置图像帧ID
   * @param[frame_id] 图像帧ID
   * @return
   */
  void set_frame_id(int32_t frame_id) {
    int32_t *id = frame_id_addr();
    *id = frame_id;
  }

  /**
   * @brief 获取图像帧时间戳
   * @param
   * @return 图像帧时间戳
   */
  int64_t timestamp() { return *timestamp_addr(); }

  /**
   * @brief 设置图像帧时间戳
   * @param[timestamp] 图像帧时间戳
   * @return
   */
  void set_timestamp(int64_t timestamp) {
    int64_t *ts = timestamp_addr();
    *ts = timestamp;
  }

  /**
   * @brief 获取图像起始地址
   * @param
   * @return
   */
  void *image_data() {
    int64_t *ts_addr = timestamp_addr();
    return reinterpret_cast<void *>(ts_addr + 1);
  }

  /**
   * @brief 获取图像大小
   * @param
   * @return
   */
  size_t image_size() {
    return data_->size() - (sizeof(int32_t) + sizeof(int64_t));
  }

  /**
   * @brief 设置图像
   * @param[img] 图像起始地址
   * @param[len] 图像大小
   * @return
   */
  void set_image(const void *img, size_t len) {
    if (image_size() < len) {
      data_->resize(sizeof(int32_t) + sizeof(int64_t) + len);
    }
    ::memcpy(image_data(), img, len);
  }

  /**
   * @brief 序列化帧
   * @param
   * @return
   */
  bool encode_frame() {
    // data_中数据已经序列化
    /**
     * |frameId|timestamp|image_payload|
     * | 4byte |  8byte  |   var len   |
     */
    return true;
  }

  /**
   * @brief 序列化帧
   * @param [addr] 序列化数据起始地址
   * @param [len] 序列化数据大小
   * @return
   */
  bool encode_frame(void *&addr, size_t &len) {
    addr = data_->data();
    len = data_->size();
    return true;
  }

  /**
   * @brief 反序列化帧
   * @param [addr] 待反序列化数据起始地址
   * @param [len] 待反序列化数据大小
   * @return 反序列化失败false， 成功true
   */
  bool decode_frame(void *addr, size_t len) {
    if (len < sizeof(int32_t) + sizeof(int64_t)) {
      return false;
    }

    len -= sizeof(int32_t);
    len -= sizeof(int64_t);
    if (image_size() < len) {
      data_->resize(sizeof(int32_t) + sizeof(int64_t) + len);
    }

    uint8_t *cur = (uint8_t *)addr;
    int32_t *id = frame_id_addr();
    *id = *((int32_t *)cur);
    cur += sizeof(int32_t);

    int64_t *ts = timestamp_addr();
    *ts = *((int64_t *)cur);
    cur += sizeof(int64_t);

    if (len > 0) {
      ::memcpy(image_data(), cur, len);
    }
    return true;
  }

  /**
   * @brief 获取序列化后的数据起始地址
   * @param
   * @return
   */
  void *frame_data() { return reinterpret_cast<void *>(data_->data()); }

  /**
   * @brief 获取序列化后的数据大小
   * @param
   * @return
   */
  size_t frame_size() { return data_->size(); }

 private:
  int32_t *frame_id_addr() {
    return reinterpret_cast<int32_t *>(data_->data());
  }

  int64_t *timestamp_addr() {
    return reinterpret_cast<int64_t *>(data_->data() + sizeof(int32_t));
  }

  std::shared_ptr<std::vector<uint8_t> > data_;
};

class CapilotParsingFrame {
 public:
  CapilotParsingFrame() {
    data_ = std::make_shared<std::vector<uint8_t> >();
    data_->resize(sizeof(int32_t) + sizeof(int64_t));
    set_frame_id(0);
    set_timestamp(0);
  }

  /**
   * @brief 获取图像帧ID
   * @param
   * @return 图像帧ID
   */
  int32_t frame_id() { return *frame_id_addr(); }

  /**
   * @brief 设置图像帧ID
   * @param[frame_id] 图像帧ID
   * @return
   */
  void set_frame_id(int32_t frame_id) {
    int32_t *id = frame_id_addr();
    *id = frame_id;
  }

  /**
   * @brief 获取图像帧时间戳
   * @param
   * @return 图像帧时间戳
   */
  int64_t timestamp() { return *timestamp_addr(); }

  /**
   * @brief 设置图像帧时间戳
   * @param[timestamp] 图像帧时间戳
   * @return
   */
  void set_timestamp(int64_t timestamp) {
    int64_t *ts = timestamp_addr();
    *ts = timestamp;
  }

  /**
   * @brief 获取全景分割图像起始地址
   * @param
   * @return
   */
  void *parsing_data() {
    if (!parsing_) {
      return nullptr;
    }
    return reinterpret_cast<void *>(parsing_->data());
  }

  /**
   * @brief 获取全景分割图像大小
   * @param
   * @return
   */
  size_t parsing_size() {
    if (!parsing_) {
      return 0;
    }

    return parsing_->size();
  }

  /**
   * @brief 获取车道线分割图像起始地址
   * @param
   * @return
   */
  void *lane_parsing_data() {
    if (!lane_parsing_) {
      return nullptr;
    }
    return reinterpret_cast<void *>(lane_parsing_->data());
  }

  /**
   * @brief 获取车道线分割图像大小
   * @param
   * @return
   */
  size_t lane_parsing_size() {
    if (!lane_parsing_) {
      return 0;
    }

    return lane_parsing_->size();
  }

  /**
   * @brief 设置全景分割图像
   * @param[img] 图像起始地址
   * @param[len] 图像大小
   * @return
   */
  void set_parsing(const void *img, size_t len) {
    if (!parsing_) {
      parsing_ = std::make_shared<std::vector<uint8_t> >();
    }

    if (parsing_->size() != len) {
      parsing_->resize(len);
    }
    ::memcpy(parsing_->data(), img, len);
  }

  /**
   * @brief 设置车道线分割图像
   * @param[img] 图像起始地址
   * @param[len] 图像大小
   * @return
   */
  void set_lane_parsing(const void *img, size_t len) {
    if (!lane_parsing_) {
      lane_parsing_ = std::make_shared<std::vector<uint8_t> >();
    }

    if (lane_parsing_->size() != len) {
      lane_parsing_->resize(len);
    }
    ::memcpy(lane_parsing_->data(), img, len);
  }

  /**
   * @brief 序列化帧
   * @param
   * @return
   */
  bool encode_frame() {
    /**
     * |frameId|timestamp| len |parsing_payload| len |lane_parsing_payload|
     * | 4byte |  8byte  |4byte|     len       |4byte|     len            |
     */
    data_->resize(sizeof(int32_t) + sizeof(int64_t) + sizeof(int32_t) +
                  parsing_size() + sizeof(int32_t) + lane_parsing_size());
    uint8_t *cur = data_->data() + sizeof(int32_t) + sizeof(int64_t);
    int32_t parsing_len = parsing_size();
    ::memcpy(cur, &parsing_len, sizeof(int32_t));
    cur += sizeof(int32_t);
    if (parsing_len > 0) {
      ::memcpy(cur, parsing_data(), parsing_len);
      cur += parsing_len;
    }

    int32_t lane_parsing_len = lane_parsing_size();
    ::memcpy(cur, &lane_parsing_len, sizeof(int32_t));
    cur += sizeof(int32_t);
    if (lane_parsing_len > 0) {
      ::memcpy(cur, lane_parsing_data(), lane_parsing_len);
      cur += lane_parsing_len;
    }

    return true;
  }

  /**
   * @brief 序列化帧
   * @param [addr] 序列化数据起始地址
   * @param [len] 序列化数据大小
   * @return
   */
  bool encode_frame(void *&addr, size_t &len) {
    encode_frame();
    addr = data_->data();
    len = data_->size();
    return true;
  }

  /**
   * @brief 反序列化帧
   * @param [addr] 待反序列化数据起始地址
   * @param [len] 待反序列化数据大小
   * @return 反序列化失败false， 成功true
   */
  bool decode_frame(void *addr, size_t len) {
    if (len < sizeof(int32_t) + sizeof(int64_t)) {
      return false;
    }

    len -= sizeof(int32_t);
    len -= sizeof(int64_t);
    uint8_t *cur = (uint8_t *)addr;
    int32_t *id = frame_id_addr();
    *id = *((int32_t *)cur);
    cur += sizeof(int32_t);

    int64_t *ts = timestamp_addr();
    *ts = *((int64_t *)cur);
    cur += sizeof(int64_t);

    if (len < sizeof(int32_t)) {
      return false;
    }
    int32_t parsing_len = *((int32_t *)cur);
    len -= sizeof(int32_t);
    cur += sizeof(int32_t);
    if (len < (size_t)parsing_len) {
      return false;
    }

    if (parsing_len > 0) {
      set_parsing(cur, parsing_len);
      len -= parsing_len;
      cur += parsing_len;
    }

    if (len < sizeof(int32_t)) {
      return false;
    }
    int32_t lane_parsing_len = *((int32_t *)cur);
    len -= sizeof(int32_t);
    cur += sizeof(int32_t);
    if (len < (size_t)lane_parsing_len) {
      return false;
    }

    if (lane_parsing_len > 0) {
      set_lane_parsing(cur, lane_parsing_len);
      len -= lane_parsing_len;
      cur += lane_parsing_len;
    }

    return true;
  }

  /**
   * @brief 获取序列化后的数据起始地址
   * @param
   * @return
   */
  void *frame_data() { return reinterpret_cast<void *>(data_->data()); }

  /**
   * @brief 获取序列化后的数据大小
   * @param
   * @return
   */
  size_t frame_size() { return data_->size(); }

 private:
  int32_t *frame_id_addr() {
    return reinterpret_cast<int32_t *>(data_->data());
  }

  int64_t *timestamp_addr() {
    return reinterpret_cast<int64_t *>(data_->data() + sizeof(int32_t));
  }

  std::shared_ptr<std::vector<uint8_t> > data_;
  std::shared_ptr<std::vector<uint8_t> > parsing_;
  std::shared_ptr<std::vector<uint8_t> > lane_parsing_;
};

template <class FrameType>
class CapilotFrameWrapper {
 public:
  /**
   * 早期版本单路图像类型CapilotImageFrame，前4字节为FrameId，且FrameId >= 0,
   * 为了兼容已有Dat数据中的单图, 增加小于0的Magic Number以区分是否为多图
   * 序列化格式：
   * |4byte MagicNumber|4byte CapilotImageFrameSize|CapilotImageFramePayload|
   */
  CapilotFrameWrapper() : magic_num_(-1) {}

  /**
   * @brief 获取图像帧总数量
   * @param
   * @return
   */
  size_t frame_count() { return frames_.size(); }

  /**
   * @brief 根据多路图像的顺序获取某一帧
   * @param index 图像帧的顺序编号
   * @return
   */
  const FrameType &get_frame(int index) {
    if (index >= frames_.size()) {
      FrameType empty;
      return empty;
    }

    return frames_[index];
  }

  /**
   * @brief 添加一帧图像
   * @param
   * @return
   */
  void add_frame(FrameType frame) { frames_.push_back(frame); }

  /**
   * @brief 删除所有图像帧
   * @param
   * @return
   */
  void clear_frames() {
    frames_.clear();
    if (data_) {
      data_.reset();
    }
  }

  /**
   * @brief 序列化帧
   * @param
   * @return
   */
  bool encode_frame() {
    size_t total = sizeof(int32_t);
    for (auto frame : frames_) {
      if (!frame.encode_frame()) {
        return false;
      }
      total += sizeof(int32_t);
      total += frame.frame_size();
    }

    data_ = std::make_shared<std::vector<uint8_t> >(total);
    uint8_t *addr = data_->data();
    ::memcpy(addr, &magic_num_, sizeof(int32_t));
    addr += sizeof(int32_t);

    for (auto frame : frames_) {
      int32_t len = frame.frame_size();
      ::memcpy(addr, &len, sizeof(int32_t));
      addr += sizeof(int32_t);
      ::memcpy(addr, frame.frame_data(), len);
      addr += len;
    }
    return true;
  }

  /**
   * @brief 反序列化帧
   * @param [addr] 待反序列化数据起始地址
   * @param [len] 待反序列化数据大小
   * @return 反序列化失败false， 成功true
   */
  bool decode_frame(void *addr, size_t len) {
    clear_frames();
    if (len < sizeof(int32_t)) {
      return false;
    }

    int32_t magic = *reinterpret_cast<int32_t *>(addr);
    if (magic > 0) {
      FrameType frame;
      if (frame.decode_frame(addr, len)) {
        add_frame(frame);
        return true;
      } else {
        return false;
      }
    }

    // skip magic number
    uint8_t *cur_addr = (uint8_t *)addr;
    cur_addr += sizeof(int32_t);
    int32_t residue = len - sizeof(int32_t);
    while (residue > 0) {
      if (residue < sizeof(int32_t)) {
        return false;
      }

      int32_t cur_len = *reinterpret_cast<int32_t *>(cur_addr);
      residue -= sizeof(int32_t);
      cur_addr += sizeof(int32_t);
      if (residue < cur_len) {
        return false;
      }

      FrameType frame;
      if (frame.decode_frame(cur_addr, cur_len)) {
        residue -= cur_len;
        cur_addr += cur_len;
        add_frame(frame);
      } else {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief 获取序列化后的数据起始地址
   * @param
   * @return
   */
  void *frame_data() {
    if (data_) {
      return reinterpret_cast<void *>(data_->data());
    } else {
      return nullptr;
    }
  }

  /**
   * @brief 获取序列化后的数据大小
   * @param
   * @return
   */
  size_t frame_size() {
    if (data_) {
      return data_->size();
    } else {
      return 0;
    }
  }

 private:
  int32_t magic_num_;
  std::vector<FrameType> frames_;
  std::shared_ptr<std::vector<uint8_t> > data_;
};

using CapilotMultiImageFrame = CapilotFrameWrapper<CapilotImageFrame>;
using CapilotMultiParsingFrame = CapilotFrameWrapper<CapilotParsingFrame>;



};  // namespace cav
#endif