//
// Created by zpx on 2024/07/06.
//

#ifndef SMALL_KV_FILE_READER_H
#define SMALL_KV_FILE_READER_H

#include <fcntl.h>
#include <glog/logging.h>
#include "db/status.h"

namespace small_kv {
    class FileReader final {
    public:
        explicit FileReader(const std::string &file_path);
        ~FileReader();
        DBStatus read(void *buf, int32_t count, int32_t offset = 0) const;
    private:
        int fd{0};
    };
}
#endif //SMALL_KV_FILE_READER_H
