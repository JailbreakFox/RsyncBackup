#include "RsyncExec.h"
#include "common.hpp"

#include <unistd.h>
#include <sstream>

#define DEFAULT_ENCRYPTION "123"                  // 默认加密密码
#define PATH_TMP "./tmp"                          // 中转文件夹路径
#define PATH_EXCLUDE "./tmp/exclude.list"         // 排除文件exclude.list存放路径
#define PATH_LOG_FILE "./runlog/rsync_c.log"      // 日志路径
#define PATH_PASSWORD_FILE "./tmp/rsync.password" // 密码文件路径

#define DEFAULT_PORT "873"                        // 默认端口号
#define DEFAULT_AUTH_USERS "My_Rsync"             // 默认登陆用户
#define DEFAULT_AUTH_PASSWORD "My_Rsync@2022"     // 默认登陆密码
#define MODULE_NAME "Backup"                      // 模块名

RsyncExec::RsyncExec()
{
	
}

RsyncExec::~RsyncExec()
{

}

bool RsyncExec::backupLocal(const BackupRule& rule)
{
	std::string path_src = rule.path_src;
	std::string path_dst = rule.path_dst;
	
	if (path_src.empty() || path_dst.empty())
	{
		return false;
	}

	if ((0 != access(path_src.c_str(), F_OK)) || (0 != access(path_dst.c_str(), F_OK)))
	{
		return false;
	}

	// 按日期存放备份目录
	std::string str_systime = "";
	time_t sys_time = time(NULL);  
	std::stringstream ss;
	ss << sys_time; 
	str_systime = ss.str();
	path_dst += "/" + str_systime;

	std::string cmd = "./rsync -avL --append-verify --delete";
	std::string path_exclude_list = PATH_EXCLUDE;
	std::string path_log_file = PATH_LOG_FILE;

	// 新建排除文件列表
	std::string exclude_list = "";
	std::vector<std::string> paths_exclude = rule.paths_exclude;
	for (unsigned int i = 0; i < paths_exclude.size(); ++i)
	{
		std::string path_exclude = paths_exclude[i];

		exclude_list += path_exclude + "\n";
	}
	if (!exclude_list.empty())
	{
		create_file(path_exclude_list.c_str(), exclude_list.c_str(), "w+", 0600);

		cmd += " --exclude-from=" + path_exclude_list;
	}

	// 将备份保存到目标目录下
	cmd += " " + path_src + " " + path_dst;
	cmd += " >" + path_log_file + " 2>&1"; // 输出日志
	system(cmd.c_str());

	// 删除排除列表
	remove(path_exclude_list.c_str());

	return true;
}

bool RsyncExec::backupRemote(const BackupRule& rule)
{
	std::string path_src = rule.path_src;
	std::string path_dst = rule.path_dst;
	RemoteDetail remote_detail = rule.remote_detail;
	std::string remote_ip = remote_detail.remote_ip;
	
	if (path_src.empty() || path_dst.empty() || remote_ip.empty())
	{
		return false;
	}

	if (0 != access(path_src.c_str(), F_OK))
	{
		return false;
	}

	std::string remote_port = DEFAULT_PORT;
	std::string auth_users = DEFAULT_AUTH_USERS;
	std::string auth_password = DEFAULT_AUTH_PASSWORD;
	std::string module_name = MODULE_NAME;
	std::string path_password_file = PATH_PASSWORD_FILE;
	std::string path_log_file = PATH_LOG_FILE;
	std::string path_exclude_list = PATH_EXCLUDE;

	if (!remote_detail.remote_port.empty())
		remote_port = remote_detail.remote_port;

	if (!remote_detail.auth_users.empty())
		auth_users = remote_detail.auth_users;

	if (!remote_detail.auth_password.empty())
		auth_password = remote_detail.auth_password;

	if (!remote_detail.module_name.empty())
		module_name = remote_detail.module_name;

	create_file(path_password_file.c_str(), auth_password.c_str(), "w+", 0600);

	// 按日期存放备份目录
	std::string str_systime = "";
	time_t sys_time = time(NULL);  
	std::stringstream ss;
	ss << sys_time; 
	str_systime = ss.str();
	path_dst += "/" + str_systime;

	// 新建排除文件列表
	std::string cmd = "./rsync -avL --append-verify --delete";
	std::string exclude_list = "";
	std::vector<std::string> paths_exclude = rule.paths_exclude;
	for (unsigned int i = 0; i < paths_exclude.size(); ++i)
	{
		std::string path_exclude = paths_exclude[i];

		exclude_list += path_exclude + "\n";
	}
	if (!exclude_list.empty())
	{
		create_file(path_exclude_list.c_str(), exclude_list.c_str(), "w+", 0600);

		cmd += " --exclude-from=" + path_exclude_list;
	}

	// 将本地目录传输到指定远程目录
	cmd += " " + path_src;
	cmd += " rsync://" + auth_users + "@" + remote_ip + ":" + remote_port + "/" + module_name + "/" + path_dst;
	cmd += " --password-file=" + path_password_file;
	cmd += " >" + path_log_file + " 2>&1"; // 输出日志
	system(cmd.c_str());

	// 删除排除列表
	remove(path_exclude_list.c_str());
}

bool RsyncExec::restoreLocal(const BackupRule& rule)
{
	std::string path_src = rule.path_src;
	std::string path_dst = rule.path_dst;
	
	if (path_src.empty() || path_dst.empty())
	{
		return false;
	}

	if ((0 != access(path_src.c_str(), F_OK)) || (0 != access(path_dst.c_str(), F_OK)))
	{
		return false;
	}

	std::string cmd = "./rsync -avL --append-verify --delete";
	std::string path_exclude_list = PATH_EXCLUDE;
	std::string path_log_file = PATH_LOG_FILE;

	// 新建排除文件列表
	std::string exclude_list = "";
	std::vector<std::string> paths_exclude = rule.paths_exclude;
	for (unsigned int i = 0; i < paths_exclude.size(); ++i)
	{
		std::string path_exclude = paths_exclude[i];

		exclude_list += path_exclude + "\n";
	}
	if (!exclude_list.empty())
	{
		create_file(path_exclude_list.c_str(), exclude_list.c_str(), "w+", 0600);

		cmd += " --exclude-from=" + path_exclude_list;
	}

	// 将备份保存到目标目录下
	cmd += " " + path_src + " " + path_dst;
	cmd += " >" + path_log_file + " 2>&1"; // 输出日志
	system(cmd.c_str());

	// 删除排除列表
	remove(path_exclude_list.c_str());
}

bool RsyncExec::restoreRemote(const BackupRule& rule)
{
	std::string path_src = rule.path_src;
	std::string path_dst = rule.path_dst;
	RemoteDetail remote_detail = rule.remote_detail;
	std::string remote_ip = remote_detail.remote_ip;
	
	if (path_src.empty() || path_dst.empty() || remote_ip.empty())
	{
		return false;
	}

	if (0 != access(path_dst.c_str(), F_OK))
	{
		return false;
	}

	std::string remote_port = DEFAULT_PORT;
	std::string auth_users = DEFAULT_AUTH_USERS;
	std::string auth_password = DEFAULT_AUTH_PASSWORD;
	std::string module_name = MODULE_NAME;
	std::string path_password_file =  PATH_PASSWORD_FILE;
	std::string path_log_file = PATH_LOG_FILE;
	std::string path_exclude_list = PATH_EXCLUDE;

	if (!remote_detail.remote_port.empty())
		remote_port = remote_detail.remote_port;

	if (!remote_detail.auth_users.empty())
		auth_users = remote_detail.auth_users;

	if (!remote_detail.auth_password.empty())
		auth_password = remote_detail.auth_password;

	if (!remote_detail.module_name.empty())
		module_name = remote_detail.module_name;

	create_file(path_password_file.c_str(), auth_password.c_str(), "w+", 0600);

	// 新建排除文件列表
	std::string cmd = "./rsync -avL --append-verify --delete";
	std::string exclude_list = "";
	std::vector<std::string> paths_exclude = rule.paths_exclude;
	for (unsigned int i = 0; i < paths_exclude.size(); ++i)
	{
		std::string path_exclude = paths_exclude[i];

		exclude_list += path_exclude + "\n";
	}
	if (!exclude_list.empty())
	{
		create_file(path_exclude_list.c_str(), exclude_list.c_str(), "w+", 0600);

		cmd += " --exclude-from=" + path_exclude_list;
	}

	// 将远程目录拉取到指定位置
	cmd += " rsync://" + auth_users + "@" + remote_ip + ":" + remote_port + "/" + module_name + "/" + path_src;
	cmd += " " + path_dst;
	cmd += " --password-file=" + path_password_file;
	cmd += " >" + path_log_file + " 2>&1"; // 输出日志
	system(cmd.c_str());

	// 删除排除列表
	remove(path_exclude_list.c_str());
}

bool RsyncExec::execBackup(const BackupRule& rule)
{
	// 规则默认关闭情况
	if (!rule.enable)
	{
		return true;
	}

	// 新建临时目录、日志目录
	create_dir("./tmp", 0755);
	create_dir("./runlog", 0755);

	if (BACKUP_LOCAL == rule.backup_type)
	{
		/* 本地备份方式 */
		if (!backupLocal(rule))
		{
			return false;
		}
	}
	else if (BACKUP_REMOTE == rule.backup_type)
	{
		/* 远程备份方式 */
		if (!backupRemote(rule))
		{
			return false;
		}
	}

	return true;
}

bool RsyncExec::execRestore(const BackupRule& rule)
{
	// 规则默认关闭情况
	if (!rule.enable)
	{
		return true;
	}

	// 新建临时目录、日志目录
	create_dir("./tmp", 0755);
	create_dir("./runlog", 0755);

	if (BACKUP_LOCAL == rule.backup_type)
	{
		/* 本地还原方式 */
		if (!restoreLocal(rule))
		{
			return false;
		}
	}
	else if (BACKUP_REMOTE == rule.backup_type)
	{
		/* 远程还原方式 */
		if (!restoreRemote(rule))
		{
			return false;
		}
	}

	return true;
}