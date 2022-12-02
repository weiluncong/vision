#!/bin/bash

file_name=$(basename $0) # 文件名
app_name=${file_name%\.*} # 文件名无后缀名

help_str="
参数说明:
        -h, --help:             打印帮助信息
        -d, --del:              删除lock.ini,tool_config.json文件
        -v, --version:          查看软件版本
"

# -q:禁止getopt输出错误报告 -o:定义短选项 -l:定义长选项
# --:后面为参数,前面为选项
getopt_cmd=$(getopt -q -o h:,d,v,t -l help,del,version,test -- "$@")

[ $? -ne 0 ] && exit 1  # 若执行getopt错误,需要终止脚本

# set --: 将getopt的输出作为新的命令行选项和参数
# eval: 将字符设为变量(处理getopt输出的参数的转义字符)
eval set -- "$getopt_cmd" 

if [ -n "$1" ]
then
	case "$1" in
        -h|--help)
		echo -e "$help_str" 
		exit 0 ;;
        -d|--del)
		cd /opt/${app_name}/dbc/
		rm lock.ini
		rm tool_config.json
		echo "rm lock.ini and tool_config.json"
		exit 0 ;;
        -v|--version)
		cd /opt/${app_name}/
		./${app_name}_ --version
		exit 0 ;;
		-t|--test)
		lib_root_path="$(dirname "$PWD")/third_party"
		cd ../
		shift ;;
		--)
		lib_root_path="/opt/${app_name}/third_party"
		cd /opt/${app_name}/
		shift ;;
		*)
		echo "$(basename $0): '$1' 不是有效命令. 查看 '$(basename $0) --help'."
		exit 0 ;;
        esac
        shift
fi

lib_path_list="${lib_root_path} 
            ${lib_root_path}/Qt/lib 
	    	${lib_root_path}/OpenCV/lib 
	    	${lib_root_path}/yaml 
            ${lib_root_path}/mdf4 
	    	${lib_root_path}/pack 
	    	${lib_root_path}/Qt/plugins
	    	${lib_root_path}/Boost 
	    	${lib_root_path}/Ogre
	    	${lib_root_path}/pcan
			${lib_root_path}/python3.7m/lib
			${lib_root_path}/capilot/lib
			${lib_root_path}/ffmpeg
			${lib_root_path}/DDS/lib"

for lib_path in $lib_path_list
do
	export LD_LIBRARY_PATH=${lib_path}:$LD_LIBRARY_PATH
done

./${app_name}_
