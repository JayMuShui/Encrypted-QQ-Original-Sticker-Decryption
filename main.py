import os
import shutil

def extract_first_24_bytes(file_path):
    """提取文件前24字节，若不足则返回None"""
    try:
        with open(file_path, 'rb') as f:
            header = f.read(24)
            if len(header) < 24:
                print(f"文件 {os.path.basename(file_path)} 不足24字节，已跳过")
                return None
            return bytearray(header)
    except IOError as e:
        print(f"无法打开文件 {file_path}: {e}")
        return None

def process_bytes(header):
    """处理字节：偶数索引（第2、4、6...个字节）奇偶翻转"""
    for i in range(1, 24, 2):  # 索引1,3,5...对应第2、4、6...字节
        value = header[i]
        if value % 2 == 0:
            header[i] = (value + 1) % 256  # 防止溢出
        else:
            header[i] = (value - 1) % 256
    return header

def decrypt_file(input_path, output_dir):
    """解密单个文件并保存到输出目录（追加.gif后缀）"""
    # 提取并处理头24字节
    header = extract_first_24_bytes(input_path)
    if header is None:
        return
    
    processed_header = process_bytes(header)
    
    # 创建输出目录
    os.makedirs(output_dir, exist_ok=True)
    
    # 构建输出路径（原文件名+.gif）
    original_filename = os.path.basename(input_path)
    new_filename = f"{original_filename}.gif"  # 关键修改点
    output_path = os.path.join(output_dir, new_filename)
    
    try:
        # 写入处理后的头24字节
        with open(output_path, 'wb') as out_f:
            out_f.write(processed_header)
            
            # 追加剩余内容
            with open(input_path, 'rb') as in_f:
                in_f.seek(24)  # 跳过已处理的头
                shutil.copyfileobj(in_f, out_f)
                
        print(f"解密成功: {original_filename} -> {new_filename}")
        
    except IOError as e:
        print(f"文件写入失败 {new_filename}: {e}")

def main():
    # 定义路径
    input_dir = "Input"  # 加密文件夹
    output_dir = "Output"  # 解密文件夹
    
    # 检查加密文件夹是否存在
    if not os.path.exists(input_dir):
        print(f"错误：加密文件夹 '{input_dir}' 不存在")
        return
    
    # 遍历加密文件夹中的文件
    for filename in os.listdir(input_dir):
        input_path = os.path.join(input_dir, filename)
        
        # 跳过目录和隐藏文件
        if os.path.isfile(input_path) and not filename.startswith('.'):
            decrypt_file(input_path, output_dir)
    
    input("处理完成，按回车键退出...")

if __name__ == "__main__":
    main()
