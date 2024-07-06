//
// Created by zpx on 2024/07/06.
//

#ifndef SMALL_KV_FILE_WRITE_H
#define SMALL_KV_FILE_WRITE_H

#include <cstring>
#include <cassert>
#include <cstdint>
#include "db/status.h"
#include <string>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>

namespace small_kv {
    class FileWriter final {
    public:
        FileWriter(const std::string &file_path, bool append = true);
        ~FileWriter();
        DBStatus append(const char *data, int32_t len, bool flush = false);
        DBStatus flush();
        void sync();
        void close();
    private:
        DBStatus buf_persist(const char *data, int32_t len);
        bool is_buffer_full();
    private:
        static constexpr int32_t BUFFER_SIZE = 65535;
        int32_t buffer_offset = 0;
        char buffer[BUFFER_SIZE];
        int32_t fd;
    };
}
#endif //SMALL_KV_FILE_WRITE_H
