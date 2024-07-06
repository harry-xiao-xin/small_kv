//
// Created by zpx on 2024/07/06.
//
#include "db/status.h"

namespace small_kv {
    bool operator==(const DBStatus &a, const DBStatus &b) {
        return a.code == b.code;
    }

    bool operator!=(const DBStatus &a, const DBStatus &b) {
        return a.code != b.code;
    }
}