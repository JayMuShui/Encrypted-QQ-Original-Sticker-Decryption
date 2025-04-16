//需要C++17及以上支持
#include <iostream>
#include <string>
#include <filesystem>  // 引入文件系统库，用于处理文件路径及文件夹遍历等操作
// 引入包含覆盖函数的头文件（假设在同一个项目目录下，实际可能需要根据情况调整包含路径），以便调用overwriteFirst24Bytes函数
#include "hexOverwriter.cpp"
#include <conio.h>    // 引入控制台输入头文件，用于实现等待用户按键的功能

namespace fs = std::filesystem;

int main() 
{
    // 获取当前可执行文件所在的目录路径
    fs::path currentPath = fs::current_path();

    // 检查当前目录是否存在，如果不存在则输出错误信息并返回，结束程序（正常情况下当前目录必然存在，但为了严谨性添加此检查）
    if (!fs::exists(currentPath)) 
    {
        std::cerr << "当前可执行文件所在目录不存在，出现异常情况，请检查。" << std::endl;
        return 1;
    }

    // 遍历当前可执行文件所在目录下的所有文件和目录项
    for (const auto& entry : fs::directory_iterator(currentPath)) 
    {
        // 如果当前项是一个常规文件（不是目录等其他类型），则进行后续操作
        if (entry.is_regular_file()) 
        {
            // 获取当前文件的文件名（不含路径部分）
            std::string fileName = entry.path().filename().string();
            // 过滤掉可执行文件decrypt.exe，可根据实际修改
            if (fileName!= "[decrypt].exe") 
            {
                // 调用overwriteFirst24Bytes函数对该文件进行提取字节、运算、覆盖等操作
                overwriteFirst24Bytes(fileName);
            }
        }
    }

    std::cout << "所有文件处理完成，按任意键退出程序...";
    // 等待用户按下任意键，使命令行窗口保持显示，方便查看输出结果，之后结束程序
    _getch();

    return 0;
}