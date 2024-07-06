//
// Created by zpx on 2024/07/06.
//
#include "file/file_reader.h"

namespace small_kv {
    FileReader::FileReader(const std::string &file_path) {
        if (access(file_path.c_str(), F_OK) != F_OK) {
            LOG(ERROR) << "cannot access: " << file_path << ".\n";
        } else {
            fd = open(file_path.c_str(), O_RDONLY);
            if (fd < 0) {
                LOG(ERROR) << "cannot open: " << file_path << ".\n";
            }
        }
    }

    FileReader::~FileReader() {
        if (fd != -1) {
            close(fd);
            fd = -1;
        }
    }

    DBStatus FileReader::read(void *buf, int32_t count, int32_t offset) const {
        if (buf == nullptr) {
            return Status::InvalidArgs;
        }
        if (fd == -1) {
            LOG(ERROR) << "fd==-1.\n";
            return Status::ExecFailed;
        }
        auto cnt = pread(fd, buf, count, offset);
        if (cnt != count) {
            LOG(ERROR) << "pread exec failed.\n";
            return Status::ExecFailed;
        }
        return Status::Success;
    }
}