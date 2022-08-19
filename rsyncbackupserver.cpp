#include "RsyncConfig.h"

#include <string>
#include <unistd.h>
#include <signal.h>
#include <iostream>

int main(int argc, char* argv[])
{
	ServerConfig config;
	config.log_file = "./runlog/rsync_s.log";
	config.port = "873";
	config.auth_users = "My_Rsync";
	config.auth_password = "My_Rsync@2022";
	config.module_name = "Backup";
	config.module_path = "/";

	RsyncConfig rsync_config;
	rsync_config.startServer(config);

    return 0;
}
