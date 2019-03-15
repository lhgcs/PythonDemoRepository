#ifndef GETMD5_H
#define GETMD5_H

#include "md5/md5.h"

class GetMd5
{
public:
    GetMd5();
    static char *get_str_md5(const md5_byte_t *data);
    static char *get_file_md5(char *filename);
};

#endif // GETMD5_H
