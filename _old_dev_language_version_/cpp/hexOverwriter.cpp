#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>  // 引入文件系统库，用于文件删除等文件系统相关操作（编译器需要C++17及以上支持）
#include "hexCalculator.cpp"  // 引入hexCalculator.cpp文件，以便调用其中的函数
#include "hexExtractor.cpp"  // 引入hexExtractor.cpp文件，以便获取原文件字节

namespace fs = std::filesystem;

// 函数用于将经过计算后的24个字节覆盖原文件的前24个字节，并保持文件其余内容不变，将新文件生成在同目录下，文件名使用原文件名+.gif
void overwriteFirst24Bytes(const std::string& fileName)
 {
    // 先提取原文件的前24个字节
    std::vector<unsigned char> originalBytes = extractFirst24Bytes(fileName);
    if (originalBytes.empty()) {
        std::cerr << "无法提取原文件 " << fileName << " 的字节数据，解密覆盖操作失败。" << std::endl;
        return;
    }

    // 对提取的字节进行计算，得到计算后的十六进制字符串（函数内会将其转换为字节形式进行处理）
    std::string calculatedHex = calculateHexString(originalBytes);
    std::vector<unsigned char> calculatedBytes;
    // 去除计算后的十六进制字符串中的空格，方便后续按每两个字符一组解析为字节
    std::string hexNoSpace = "";
    for (char c : calculatedHex)
     {
        if (c!= ' ') 
        {
            hexNoSpace += c;
        }
    }
    // 将十六进制字符串每两个字符一组转换为字节，并存入vector容器
    for (size_t i = 0; i < hexNoSpace.length(); i += 2) 
    {
        std::string hexByte = hexNoSpace.substr(i, 2);
        calculatedBytes.push_back(static_cast<unsigned char>(std::stoi(hexByte, nullptr, 16)));
    }

    // 生成新文件名，将原文件名加上".gif"后缀，这里假设原文件名不包含后缀或者你可以根据实际情况进一步处理文件名后缀相关逻辑
    std::string newFileName = fileName + ".gif";
    // 以二进制模式创建新文件，创建一个输出文件流对象outputFile，用于将覆盖后的内容写入新文件
    std::ofstream outputFile(newFileName, std::ios::binary);
    if (!outputFile) 
    {
        std::cerr << "无法创建新文件 " << newFileName << "，解密覆盖操作失败。" << std::endl;
        return;
    }

    // 将计算后并转换好的24个字节写入新文件
    outputFile.write(reinterpret_cast<const char*>(calculatedBytes.data()), calculatedBytes.size());

    // 跳过原文件已读取的前24个字节，准备读取剩余内容并写入新文件
    std::ifstream inputFile(fileName, std::ios::binary);
    inputFile.seekg(24, std::ios::beg);
    char buffer[4096];  // 定义一个合适大小的缓冲区用于批量读取文件剩余内容，这里以4096字节为例，可根据实际情况调整
    std::streamsize bytesRead;
    do {
        // 从原文件读取一块数据到缓冲区
        inputFile.read(buffer, sizeof(buffer));
        bytesRead = inputFile.gcount();
        // 将读取到的数据写入新文件
        outputFile.write(buffer, bytesRead);
    } while (bytesRead > 0);

    inputFile.close();
    outputFile.close();

    std::cout << "已成功生成新文件 " << newFileName << "，原文件的前24个字节已按照加密方法反向解密覆盖更新，按任意键继续...";

    // 以下是嵌入的删除旧文件的逻辑部分
    fs::path filePath = fs::current_path() / fileName;
    // 判断文件是否存在，如果存在则尝试删除
    if (fs::exists(filePath) && fs::is_regular_file(filePath))
     {
        try {
            fs::remove(filePath);
            std::cout << "已成功删除旧文件: " << fileName << std::endl;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "删除旧文件 " << fileName << " 时出现错误: " << e.what() << std::endl;
        }
    }
}