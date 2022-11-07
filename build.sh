#! /bin/bash

del_build=off # 删除build目录
create_package=off # 生成deb安装包
help_str="
参数说明:
	-h, --help:		打印帮助信息
	-r, --remove:		删除build目录
	-p, --package:		生成deb安装包	
"
# -q:禁止getopt输出错误报告 -o:定义短选项 -l:定义长选项
# --:后面为参数,前面为选项
getopt_cmd=$(getopt -q -o h,r,p -l help,remove,package -- "$@")

[ $? -ne 0 ] && exit 1	# 若执行getopt错误,需要终止脚本

# set --: 将getopt的输出作为新的命令行选项和参数
# eval: 将字符设为变量(处理getopt输出的参数的转义字符)
eval set -- "$getopt_cmd"

while [ -n "$1" ]
do
	case "$1" in
	-h|--help)
		echo -e "$help_str"
		exit 0 ;;
	-r|--remove)
		del_build=on ;;
	-p|--package)
		create_package=on ;;
	--)	shift ;;
	*)
		echo "$(basename $0): '$1' 不是有效命令. 查看 '$(basename $0) --help'."
		exit 0 ;;
	esac
	shift
done

root_path=$(pwd)
build_path=$root_path/build
if [ -e build ]
then
	cd build
	if [[ $del_build == on ]] 
	then
		rm -r ./*
    		echo "build exist! " 
	fi
else
	mkdir build
	cd build
fi

cmake ..
make -j8
if [[ $create_package == on ]]
then
	make package
fi
