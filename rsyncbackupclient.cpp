#include "RsyncExec.h"

int main(int argc, char* argv[])
{
	BackupRule rule;
	bool is_backup = true;;
	for (int i = 0; i < argc; ++i)
	{
		std::string str = argv[i];
		if ("--BackupLocal" == str || "-bl" == str)
		{
			rule.enable = true;
			rule.backup_type = BACKUP_LOCAL;
			rule.path_src = "/home/xyh/tmp/backup_dir";
			std::vector<std::string> paths_exclude;
			paths_exclude.push_back("1.md");
			rule.paths_exclude = paths_exclude;
			rule.path_dst = "/home/xyh/tmp";
		}
		else if ("--BackupRemote" == str || "-br" == str)
		{
			rule.enable = true;
			rule.backup_type = BACKUP_REMOTE;
			rule.path_src = "/home/xyh/tmp/backup_dir";
			std::vector<std::string> paths_exclude;
			paths_exclude.push_back("1.md");
			rule.paths_exclude = paths_exclude;
			rule.path_dst = "/tmp";
			RemoteDetail detail;
			detail.remote_ip = argv[i + 1];
			detail.remote_port = "873";
			detail.auth_users = "My_Rsync";
			detail.auth_password = "My_Rsync@2022";
			detail.module_name = "Backup";
			rule.remote_detail = detail;
		}
		else if ("--RestoreLocal" == str || "-rl" == str)
		{
			rule.enable = true;
			rule.backup_type = BACKUP_LOCAL;
			rule.path_src = "/home/xyh/tmp/127.0.0.1-2022-08-18/backup_dir";
			std::vector<std::string> paths_exclude;
			paths_exclude.push_back("2.md");
			paths_exclude.push_back("3.md");
			rule.paths_exclude = paths_exclude;
			rule.path_dst = "/home/xyh/tmp2";
			is_backup = false;
		}
		else if ("--RestoreRemote" == str || "-rr" == str)
		{
			rule.enable = true;
			rule.backup_type = BACKUP_REMOTE;
			rule.path_src = "/tmp/192.168.225.135-2022-08-18/backup_dir";
			std::vector<std::string> paths_exclude;
			paths_exclude.push_back("2.md");
			paths_exclude.push_back("3.md");
			rule.paths_exclude = paths_exclude;
			rule.path_dst = "/home/xyh/tmp2";
			RemoteDetail detail;
			detail.remote_ip = argv[i + 1];
			detail.remote_port = "873";
			detail.auth_users = "My_Rsync";
			detail.auth_password = "My_Rsync@2022";
			detail.module_name = "Backup";
			rule.remote_detail = detail;
			is_backup = false;
		}
	}

	RsyncExec exec;
	is_backup ? exec.execBackup(rule) : exec.execRestore(rule);

    return 0;
}
