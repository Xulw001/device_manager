#include "pch.h"

namespace clog {

Logger g_logger_;

std::string convert(const std::wstring &src) {
  std::string dst;
  if (src.empty()) {
    return dst;
  }

  int size = -1;
  size = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
  if (size <= 0) {
    return false;
  }

  dst.resize(size);
  size = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, (char *)dst.c_str(),
                             size, NULL, NULL);
  if (size <= 0) {
    return false;
  }

  dst.resize(size - 1);
  return dst;
}

Logger &Logger::operator<<(const std::string &data) {
  if (flag) {
    ofstream_ << data;
  }
  return *this;
}

Logger &Logger::operator<<(const std::wstring &data) {
  if (flag) {
    ofstream_ << convert(data);
  }
  return *this;
}

Logger &Logger::operator<<(const int data) {
  if (flag) {
    ofstream_ << data;
  }
  return *this;
}

Logger &Logger::operator()(LEVLE level) {
  if (level >= level_) {
    auto t =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    if (!ofstream_.is_open()) {
      ofstream_.open(k_log_file_name, std::ios_base::out | std::ios_base::app |
                                          std::ios_base::binary);
    }

    if (ofstream_.is_open()) {
      tm tm;
      localtime_s(&tm, &t);
      if (ofstream_.tellp() != 0) ofstream_ << std::endl;
      ofstream_ << std::put_time(&tm, "%Y:%m:%d %H:%M:%S ");
      switch (level) {
        case Debug:
          ofstream_ << "[Debug]";
          break;
        case Warning:
          ofstream_ << "[Warning]";
          break;
        case Error:
          ofstream_ << "[Error]";
          break;
        case Info:
        default:
          ofstream_ << "[Info]";
          break;
      }
    }
    flag = true;
  } else {
    flag = false;
  }

  return *this;
}

};  // namespace clog