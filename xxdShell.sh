#!/bin/bash
startmemNum=`expr $1 \* 512`
echo "开始位置:${startmemNum}; "
xxd -u -a -g 1 -c 16 -s ${startmemNum} -l 512 80m.img