//
// Created by zpx on 2024/07/06.
//
#include <memory>
#include <gtest/gtest.h>
#include "file/file_writer.h"
#include "file/file_reader.h"

using namespace small_kv;
TEST(file_reader, basic) {
    // 新建文件
    std::string path = "./build/tests.sst";
    auto fwriter = std::make_unique<FileWriter>(path, true);
    std::string data1 = "123gfds4h6.1 s0 3ds 4g00 x_";
    std::string data2 = "  7112gdf455 4 56 u 455u 123fgh12fgn  ";
    std::string data3 = ".*-123 34fe r65 4r1 0m1 j0.10,/,.";
    fwriter->append(data1.data(), data1.size());
    fwriter->append(data2.data(), data2.size());
    fwriter->append(data3.data(), data3.size());
    fwriter->close();

    // 读取文件
    auto freader = std::make_unique<FileReader>(path);
    int cnt = data1.size() + data2.size() + data3.size();
    char buf[cnt];
    EXPECT_EQ(freader->read(buf, cnt, 0).code, Status::Success.code);
    EXPECT_EQ(std::string(buf, cnt), data1 + data2 + data3);
    auto _ = std::system(("rm -rf " + path).c_str());
}
