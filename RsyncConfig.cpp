#include "RsyncConfig.h"
#include "common.hpp"

#include <stdio.h>

#define DEFAULT_LOG_FILE "./runlog/rsync_s.log"   // 默认日志路径
#define DEFAULT_PORT "873"                        // 默认端口号
#define DEFAULT_AUTH_USERS "My_Rsync"             // 默认登陆用户
#define DEFAULT_AUTH_PASSWORD "My_Rsync@2022"     // 默认登陆密码
#define PATH_CONFIG "./tmp/rsyncd.conf"           // 配置文件路径
#define PATH_PASSWORD_FILE "./tmp/rsync.password" // 密码文件路径
#define MODULE_NAME "Backup"                      // 模块名
#define MODULE_PATH "/"                           // 模块路径

RsyncConfig::RsyncConfig()
{

}

RsyncConfig::~RsyncConfig()
{

}

bool RsyncConfig::startServer(const ServerConfig& config)
{
	// 新建临时目录、日志目录
	create_dir("./tmp", 0755);
	create_dir("./runlog", 0755);

	std::string config_str;
	std::string log_file = DEFAULT_LOG_FILE;
	std::string auth_port = DEFAULT_PORT;
	std::string auth_users = DEFAULT_AUTH_USERS;
	std::string auth_password = DEFAULT_AUTH_PASSWORD;
	std::string path_config = PATH_CONFIG;
	std::string path_password_file = PATH_PASSWORD_FILE;
	std::string module_name = MODULE_NAME;
	std::string module_path = MODULE_PATH;

	if (!config.log_file.empty())
		log_file = config.log_file;

	if (!config.port.empty())
		auth_port = config.port;

	if (!config.auth_users.empty())
		auth_users = config.auth_users;

	if (!config.auth_password.empty())
		auth_password = config.auth_password;

	if (!config.module_name.empty())
		module_name = config.module_name;

	if (!config.module_path.empty())
		module_path = config.module_path;

	create_file(path_password_file.c_str(), (auth_users + ":" + auth_password).c_str(), "w+", 0600);

	config_str += "uid = 0\n";
	config_str += "gid = 0\n";
	config_str += "use chroot = no\n";
	config_str += "reverse lookup = false\n";
	config_str += "strict modes = false\n";
	config_str += "read only = false\n";
	config_str += "log file = " + log_file + "\n";
	config_str += "port = " + auth_port + "\n";
	config_str += "auth users = " + auth_users + "\n";
	config_str += "secrets file = " + path_password_file + "\n";
	config_str += "[" + module_name + "]\n";
	config_str += "path = " + module_path + "\n";

	create_file(path_config.c_str(), config_str.c_str(), "w+", 0600);

	std::string cmd = "./rsync --daemon --config=" + path_config;
	system(cmd.c_str());

	return true;
}