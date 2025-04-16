#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

void decryptFile(const char *inputFile, const char *outputFile) {
    FILE *input, *output;
    unsigned char buffer[BUFFER_SIZE];
    int i, j, k;

    input = fopen(inputFile, "rb");
    if (input == NULL) {
        perror("Error opening input file");
        exit(1);
    }

    output = fopen(outputFile, "wb");
    if (output == NULL) {
        fclose(input);
        perror("Error opening output file");
        exit(1);
    }

    // 处理前三行
    for (i = 0; i < 3; i++) {
        fread(buffer, 1, BUFFER_SIZE, input);

        for (j = 0, k = 0; j < BUFFER_SIZE && k < 16; j += 2, k++) {
            // 对每组末尾一个数进行解密操作
            if (buffer[j + 1] % 2 == 1) {
                buffer[j + 1]--;
            } else {
                buffer[j + 1]++;
            }
        }

        fwrite(buffer, 1, BUFFER_SIZE, output);
    }

    // 处理剩余文件内容（直接复制）
    while (!feof(input)) {
        size_t bytesRead = fread(buffer, 1, BUFFER_SIZE, input);
        fwrite(buffer, 1, bytesRead, output);
    }

    fclose(input);
    fclose(output);
}

int main() {
    decryptFile("encrypted.gif", "decrypted.gif");

    return 0;
}

