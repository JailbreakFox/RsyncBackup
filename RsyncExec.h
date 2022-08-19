#ifndef RSYNCEXEC_H
#define RSYNCEXEC_H

#include <string>
#include <vector>

enum BackupType {
	BACKUP_LOCAL, // 本地备份
	BACKUP_REMOTE // 远程备份
};

typedef struct RemoteDetail {
	std::string remote_ip;     // 远程IP
	std::string remote_port;   // 远程端口
	std::string auth_users;    // 远程用户名
	std::string auth_password; // 远程密码
	std::string module_name;   // 远程模块名
} RemoteDetail;

typedef struct BackupRule {
	bool enable;                            // 规则开关
	BackupType backup_type;                 // 备份方式
	RemoteDetail remote_detail;             // 远程详细内容(只有远程方式起效)
	std::string path_src;                   // 源路径
	std::vector<std::string> paths_exclude; // 路径排除
	std::string path_dst;                   // 目标路径
} BackupRule;

class RsyncExec
{
public:
	RsyncExec();
	~RsyncExec();

	/* 启用一条备份命令 */
	bool execBackup(const BackupRule& rule);
	/* 启用一条还原命令 */
	bool execRestore(const BackupRule& rule);

private:
	/* 本地备份 */
	bool backupLocal(const BackupRule& rule);
	/* 远程备份 */
	bool backupRemote(const BackupRule& rule);

	/* 本地还原 */
	bool restoreLocal(const BackupRule& rule);
	/* 远程还原 */
	bool restoreRemote(const BackupRule& rule);
};

#endif