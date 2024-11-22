#!/bin/bash
set -e

# 启动 fdfs 服务
fdfs_trackerd /etc/fdfs/tracker.conf start
fdfs_storaged /etc/fdfs/storage.conf start

# 启动 PostgreSQL 服务
service postgresql start

# 启动 Redis
redis-server /root/foxcloudservers/foxcloud/server/config/redis.conf

# 启动 SSH 服务
/usr/sbin/sshd

# 启动 nginx 服务
nginx