#include <string>        // 引入字符串头文件，用于处理字符串相关操作
#include <vector>        // 引入向量头文件，因为要接收字节数据的vector作为参数
#include <sstream>       // 引入字符串流头文件，方便将字节转换为十六进制字符串以及后续的数值转换等操作
#include <iomanip>       // 引入输入/输出流控制头文件，用于设置十六进制输出的格式，如宽度、填充字符等

// 函数用于将字节类型的vector转换为十六进制字符串，同时对偶数序号（第2、4、6……个字节对应的十六进制数）按规则进行计算
std::string calculateHexString(const std::vector<unsigned char>& bytes)
 {
    std::string result;
    // 预先为结果字符串分配大致空间，每个十六进制数占2位加一个空格（除最后一个外），这样可以减少字符串在添加元素过程中的内存重新分配次数，提高性能
    result.reserve(24 * 3 - 1);  

    // 循环遍历传入的字节vector，bytes.size()表示vector中元素的个数，也就是字节的个数（这里应为24个）
    for (size_t i = 0; i < bytes.size(); ++i) 
    {
        // 获取当前位置的字节数据
        unsigned char byte = bytes[i];
        // 创建一个字符串流对象ss，用于将字节数据转换为十六进制字符串
        std::stringstream ss;
        // 将字节数据转换为十六进制格式的字符串，std::hex表示以十六进制输出，std::setw(2)设置输出宽度为2位，
        // std::setfill('0')表示如果不足2位用'0'填充，static_cast<int>(byte)将无符号字符类型的字节转换为整数类型以便输出十六进制表示
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        // 从字符串流对象中获取转换后的十六进制字符串
        std::string hexByte = ss.str();

        // 判断字节序号（从0开始计数）是否为奇数，因为要对第2、4、6……个字节进行操作，这些字节序号在程序里是奇数（0表示第1个字节，1表示第2个字节，以此类推）
        if (i % 2 == 1) {  
            // 将十六进制字符串转换为整数（十六进制对应的十进制值），方便后续进行数值计算，nullptr表示使用默认的进制转换方式（这里是十六进制转十进制）
            int value = std::stoi(hexByte, nullptr, 16);
            // 判断转换后的十进制数值是偶数还是奇数，进行相应计算
            if (value % 2 == 0)
            {  // 如果是偶数
                value++;
            } else {  // 如果是奇数
                value--;
            }
            // 创建一个新的字符串流对象newSs，用于将计算后的数值重新转换为十六进制字符串
            std::stringstream newSs;
            // 将计算后的数值再次转换为十六进制格式的字符串，同样设置宽度、填充等格式要求
            newSs << std::hex << std::setw(2) << std::setfill('0') << value;
            // 获取重新转换后的十六进制字符串，用于更新当前字节对应的十六进制表示
            hexByte = newSs.str();
        }

        // 将当前字节对应的十六进制字符串添加到结果字符串中
        result += hexByte;
        // 如果不是最后一个字节，就在十六进制字符串后面添加一个空格，用于格式化输出，使结果更易读
        if (i < bytes.size() - 1) 
        {
            result += " ";
        }
    }

    // 返回处理后的十六进制字符串形式的结果
    return result;
}