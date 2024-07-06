//
// Created by zpx on 2024/07/06.
//

#ifndef SMALL_KV_OPTIONS_H
#define SMALL_KV_OPTIONS_H

#include <string>

namespace small_kv {
    struct Options {
        std::string STORAGE_DIR = "./output/db_storage";
        std::string WAL_DIR = "./output/wal_log.txt";
        size_t MEM_TABLE_MAX_SIZE = 4 * 1024 * 1024;
        uint32_t CACHE_SIZE = 4096;
        uint32_t LISST_NUM = 0;
    };

    inline Options MakeOptionsForDebugging() {
        return Options();
    }

    inline Options MakeOptionsForProduction() {
        return Options();
    }

    struct ReadOptions {
    };
    struct WriteOptions {
        bool Flush = false;
    };
}
#endif //SMALL_KV_OPTIONS_H
