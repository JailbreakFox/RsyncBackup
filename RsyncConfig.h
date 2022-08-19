#ifndef RSYNCCONFIG_H
#define RSYNCCONFIG_H

#include <string>

typedef struct ServerConfig {
	std::string log_file;      // 日志路径
	std::string port;          // 监听端口
	std::string auth_users;    // 登陆用户
	std::string auth_password; // 登陆密码
	std::string module_name;   // 模块名
	std::string module_path;   // 模块路径
} ServerConfig;

class RsyncConfig
{
public:
	RsyncConfig();
	~RsyncConfig();

	/* 启动Rsync服务端 */
	bool startServer(const ServerConfig& config);
};

#endif