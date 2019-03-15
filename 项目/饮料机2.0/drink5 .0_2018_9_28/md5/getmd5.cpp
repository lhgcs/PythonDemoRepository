#include "getmd5.h"
#include "md5/md5.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

GetMd5::GetMd5(){
}
/**
  字符串MD5
 * @brief GetMd5::get_str_md5
 * @param data
 * @return
 */
char* GetMd5::get_str_md5(const md5_byte_t *data){
    // md5_byte_t *data = "C-Language";
    int len=0;
    const md5_byte_t *temp=data;
    while (*temp != '\0') {
        len++;
        temp++;
    }
    temp=NULL;

    md5_byte_t digest[16];
    md5_state_t md5StateT;
    md5_init(&md5StateT);
    md5_append(&md5StateT, data, len);
    md5_finish(&md5StateT, digest);
    //char md5String[33] = { '\0' };
    char *md5String=NULL;
    char hexBuffer[3];
    for (size_t i = 0; i != 16; ++i){
        if (digest[i] < 16){
            sprintf(hexBuffer, "0%x", digest[i]);
        }
        else{
            sprintf(hexBuffer, "%x", digest[i]);
        }
        strcat(md5String, hexBuffer);
    }
    printf("%s\n", md5String);
    return md5String;
}

#include <time.h>
#define INPUT_BUFFER_SIZE (1024*1024)
/**
  文件MD5
 * @brief GetMd5::get_file_md5
 * @param filename
 * @return
 */
char *GetMd5::get_file_md5(char *filename){
    char *md5String=NULL;
    printf("文件名：%s",filename);
    FILE *file = fopen(filename, "rb");
    if (file){
        printf("正在计算MD5值...\n");
        time_t startTime = time(NULL);
        md5_state_t md5StateT;
        md5_init(&md5StateT);
        unsigned char buffer[INPUT_BUFFER_SIZE];
        while (!feof(file)){
            size_t numberOfObjects = fread(buffer, sizeof(char), INPUT_BUFFER_SIZE, file);
            md5_append(&md5StateT, buffer, numberOfObjects);
        }
        md5_byte_t digest[16];
        md5_finish(&md5StateT, digest);
        //char md5String[33] = { '\0' }, hexBuffer[3];
        char hexBuffer[3];
        for (size_t i = 0; i != 16; ++i){
            if (digest[i] < 16)
                sprintf(hexBuffer, "0%x", digest[i]);
            else
                sprintf(hexBuffer, "%x", digest[i]);
            strcat(md5String, hexBuffer);
        }
        time_t endTime = time(NULL);
        fclose(file);
        printf("计算完毕：%s，耗时%u秒\n", md5String, endTime - startTime);
    }
    else
        printf("无法打开文件：%s\n", filename);
    return md5String;
}

