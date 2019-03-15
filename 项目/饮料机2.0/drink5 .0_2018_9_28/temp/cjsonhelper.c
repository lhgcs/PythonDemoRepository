#include "cjsonhelper.h"

#include "stdlib.h"
#include "stdio.h"

#define LOGD printf
#define LOGI printf
#define LOGE printf




////解析JSON包
//char *cjson_parse_string(char *str,char* key){
//    /*
//     * 解决c/c++函数返回局部变量char*的错误
//    char arr[] = "Hello,World\n";           // arr[] 所有元素保存在栈内存上，arr 是个符号地址，没有存储空间
//        return arr;                         // warning C4172: 返回局部变量或临时变量的地址, 即警告返回了指向栈内存的指针，返回后栈内存都会被自动回收
//    char *arr = "Hello,World\n";            // "Hello,World\n" 保存在只读常量区，非栈内存不受函数返回影响
//        return arr;                         // 其实返回的是 arr 的副本，返回后 arr 变量也销毁，但是其指向常量区不受影响
//    static char arr[] = "Hello,World\n";    // "Hello,World\n" 保存在静态存储区，非栈内存不受函数返回影响。
//    return arr;                             // 同方法1 arr 是个符号地址，没有存储空间
//    */
//    char *value=NULL;                               //定义一个字符串指针
//    cJSON *root= cJSON_Parse(str);                  //字符串解析成json结构体
//    if(NULL==root){
//        LOGD("json error\n");
//    }else{
//        LOGD("json: %s\n",cJSON_Print(root));       //json结构体转换成字符串;
//        cJSON *temp = cJSON_GetObjectItem(root,key);
//        if(NULL==temp){
//            LOGD("json not key:%s",key);
//        }else{
//            if(temp->type == cJSON_String){
//                value=temp->valuestring;             //获取某个元素
//            }
//        }
//        cJSON_Delete(root);
//    }
//    return value;
//}

//int cjson_parse_number(char *str,char* key){
//    int value=0;
//    cJSON *root= cJSON_Parse(str);                   //字符串解析成json结构体
//    if(NULL==root){
//        LOGD("json error\n");
//    }else{
//        LOGD("json: %s\n",cJSON_Print(root));        //json结构体转换成字符串;
//        cJSON *temp = cJSON_GetObjectItem(root,key);
//        if(NULL==temp){
//            LOGD("json not key:%s",key);
//        }else{
//            if(temp->type == cJSON_Number){
//                value=temp->valueint;                //获取某个元素
//            }
//        }
//        cJSON_Delete(root);
//    }
//    return value;
//}


//typedef struct {
//    char *key;
//    int value;
//}map_int;
//typedef struct {
//    char *key;
//    char *value;
//}map_str;

////map_int map[10];
////map[0].key="123";
////map[0].value=1;





////生成JSON包
//char* get_json(){
//    cJSON *root=cJSON_CreateObject();
//    cJSON_AddNumberToObject(root,"1",1);
//    cJSON_AddNumberToObject(root,"1",2);
//    char *s =cJSON_Print(root);
//    cJSON_Delete(root);                 //在运行p=cJSON_Print(root);后务必要要进行如下释放操作

//    char *t=s;                          //目标指针先指向原串头
//    while( *s != '\0' ){                //遍历字符串s
//        if ( *s != '\n' && *s != '\t' ){//如果当前字符不是要删除的，则保存到目标串中
//            *t++=*s;
//        }
//        s++ ;                           //检查下一个字符
//    }
//    *t='\0';                            //置目标串结束符。
//    free(s);
//    return s;
//}

//在CJSON直接加上一句 void my_free(void* p) { cJSON_free(p); } 就可以了，解决泄露

