/*此自定义函数可根据需要修改用于提取指定文件的指定字节并储存在vector容器当中*/
/*This custom function can be modified as needed to extract the specified bytes of the specified file and store it in the vector container*/

#include <iostream>    // 引入输入/输出流头文件，用于输出错误信息等操作
#include <fstream>     // 引入文件流头文件，用于操作文件的读取
#include <vector>      // 引入向量头文件，用于创建存储字节数据的vector容器

// 函数用于从指定文件中提取前24个字节，以字节类型的vector返回
std::vector<unsigned char> extractFirst24Bytes(const std::string& fileName) 
{
    // 以二进制模式打开指定文件，创建一个输入文件流对象inputFile
    std::ifstream inputFile(fileName, std::ios::binary);
    // 判断文件是否成功打开，如果没有成功打开（即inputFile为假），则输出错误信息，并返回一个空的vector
    if (!inputFile) 
    {
        std::cerr << "无法打开文件 " << fileName << std::endl;
        return std::vector<unsigned char>();
    }

    // 创建一个大小为24的vector容器，用于存储从文件中读取的字节数据，元素类型为unsigned char（无符号字符，可表示字节）
    std::vector<unsigned char> buffer(24);
    // 从打开的文件中读取24个字节的数据到buffer向量中，注意需要将buffer的数据指针转换为char*类型，因为read函数的参数要求
    inputFile.read(reinterpret_cast<char*>(buffer.data()), 24);
    // 检查实际读取到的字节数是否小于24，如果小于24，表示文件内容不足24个字节，无法完成预期的提取操作
    if (inputFile.gcount() < 24) 
    {
        std::cerr << "文件 " << fileName << " 小于24个字节，这不是一个符合解密条件的文件，无法提取完整的24个字节。" << std::endl;
        // 关闭已打开的文件，释放相关资源
        inputFile.close();
        return std::vector<unsigned char>();
    }

    // 关闭已经读取完数据的文件，关闭后就不能再通过这个文件流对象进行读写操作了
    inputFile.close();
    // 返回包含读取到的24个字节的vector容器
    return buffer;
}