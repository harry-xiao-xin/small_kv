//
// Created by zpx on 2024/07/06.
//

#ifndef SMALL_KV_STATUS_H
#define SMALL_KV_STATUS_H

#include <cstdint>
#include <string>

namespace small_kv {
    struct DBStatus {
        int32_t code;
        const char *msg;
    };
    bool operator==(const DBStatus &a, const DBStatus &b);
    bool operator!=(const DBStatus &a, const DBStatus &b);

    struct Status {
        Status() = delete;
        ~Status() = delete;
        static constexpr DBStatus Success = {1, "ExecStatus."};
        static constexpr DBStatus InvalidArgs = {2, "Invalid args."};
        static constexpr DBStatus ExecFailed = {3, "Exec failed."};
        static constexpr DBStatus NotImpl = {4, "Not implemented."};
        static constexpr DBStatus NotFound = {5, "Key not found."};
    };
}
#endif //SMALL_KV_STATUS_H
