#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fdfs_client.h"
#include "log.h"

int fdfs_upload_file(const char* conf_file, const char* upload_file_path, char* file_id)
{
	LogInit();

	char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
	ConnectionInfo *pTrackerServer;
	int result;
	int store_path_index;
	ConnectionInfo storageServer;

	// 通过客户端配置文件初始化一些数据
	if ((result=fdfs_client_init(conf_file)) != 0)
	{
		return result;
	}
	LogInfo("FDFS-Client init successful");

	/*
	 * 通过从配置文件中读出的数据，连接追踪器 Tracker
	 *  通过得到的地址可以访问 Tracker
	 */
	pTrackerServer = tracker_get_connection();
	if (pTrackerServer == NULL)
	{
		fdfs_client_destroy();
		return errno != 0 ? errno : ECONNREFUSED;
	}

	*group_name = '\0';

	/* 通过 Tracker 得到存储节点的信息 */
	if ((result=tracker_query_storage_store(pTrackerServer, \
	                &storageServer, group_name, &store_path_index)) != 0)
	{
		fdfs_client_destroy();
		fprintf(stderr, "tracker_query_storage fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
		LogError("Failed to get Storage info, error code: %d", result);
		return result;
	}

	/* 文件上传 */
	result = storage_upload_by_filename1(pTrackerServer, \
			&storageServer, store_path_index, \
			upload_file_path, NULL, \
			NULL, 0, group_name, file_id);
	if (result == 0)
	{
		printf("%s\n", file_id);
		LogInfo("FDFS-Client upload successful, file ID: %s", file_id);
	}
	else
	{
		fprintf(stderr, "upload file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
		LogError("Failed to upload file, error code: %d", result);
	}

	tracker_close_connection_ex(pTrackerServer, true);
	fdfs_client_destroy();

	return result;
}

