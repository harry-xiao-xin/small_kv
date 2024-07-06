//
// Created by zpx on 2024/07/04.
//
#include <gtest/gtest.h>
#include <glog/logging.h>

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    google::InitGoogleLogging("small_kv");
    FLAGS_log_dir = "./log";
    FLAGS_alsologtostderr = true;
    LOG(INFO) << "Start test...\n";
    return RUN_ALL_TESTS();
}