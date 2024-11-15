#!/bin/bash
set -e

# 启动 PostgreSQL 服务
service postgresql start

# 启动 Redis
redis-server /root/foxcloudservers/foxcloud/config/redis.conf

# 启动 SSH 服务
/usr/sbin/sshd -D

