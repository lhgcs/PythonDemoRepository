#!/opt/anaconda1/bin
#_*_ coding:utf-8 _*_

__author__ = 'Administrator'

import sys
from operator import itemgetter
from itertools import  groupby
def read_mapper_output(file,separator='\t"'):
    for line in file:
        yield line.rstrip().split(separator,1)

def main():
    data=read_mapper_output(sys.stdin)

    for current_word,group in data:
        for word in groupby(data,itemgetter(0)):
            total_count = sum(int(count) for current_word,count in group)
            print ("%s%s%d" % (current_word,,'\t',total_count))

if __name__ == '__main__':
    main()

#echo "a b c d"|/opt/anaconda2/bin/python hdfs_map.py  | sort -k1,1 |/opt/anaconda2/bin/python hdfs_reduce.py









