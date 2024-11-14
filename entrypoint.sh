#!/bin/bash
set -e

# 启动 PostgreSQL 服务
service postgresql start

# 启动 SSH 服务
/usr/sbin/sshd -D

