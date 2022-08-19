#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

bool create_file(const std::string &path, const std::string &content, const std::string& addmode, const mode_t mode)
{
	bool rt = true;
	do{
		FILE* fp;
		fp = fopen (path.c_str(), addmode.c_str());
		fprintf(fp, "%s", content.c_str());
		fclose(fp);
		
		/*修改文件属性*/
		if(chmod(path.c_str(), mode) != 0)
		{
			rt = false;
			break;
		}
	}while(0);
	
	return rt;
}

bool create_dir(const std::string &dir_name, const mode_t mode)
{
    bool rt = true;
    do{
        //参数检查
        if(dir_name.empty())
        {
            return false;
        }

        //文件是否存在
        if(access(dir_name.c_str(), F_OK) == 0)
        {
            return false;
        }

        if(mkdir(dir_name.c_str(), 0) < 0)/*创建目录*/
        {
            return false;
        }

        /*修改文件属性*/
        if(chmod(dir_name.c_str(), mode) != 0)
        {
            return false;
        }

    }while(0);

    return true;
}