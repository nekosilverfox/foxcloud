FROM ubuntu:16.04

LABEL description="FoxCloud Server docker image"
LABEL maintainer="nekosilverfox"

# 配置为自己的
ENV SSH_ROOT_PASSWORD=your_ssh_password
ENV DB_USERNAME=your_db_username_name
ENV DB_PASSWORD=your_db_password


ENV DB_NAME=db_foxcloud

# 定义构建参数 TARGETARCH
ARG TARGETARCH

###############################################################
# Install other packages
###############################################################
RUN apt-get update
RUN apt-get install -y vim net-tools git


###############################################################
# 克隆 Github 仓库到容器（需要利用仓库中的配置文件）
# 由于 Docker 缓存机制如果不再次 pull 会造成 仓库代码不是最新版
###############################################################
ENV PROJECT_BASE_PATH=/root/foxcloudservers
RUN mkdir ${PROJECT_BASE_PATH}

ENV PROJECT_SOURCE_CODE_PATH=${PROJECT_BASE_PATH}/foxcloud
RUN cd ${PROJECT_BASE_PATH} && \
    git clone https://github.com/nekosilverfox/foxcloud.git && \
    cd ${PROJECT_SOURCE_CODE_PATH} && \
    git pull


###############################################################
# 创建开机自动启动的服务的脚本
###############################################################
ENV ENTRYPOINT_SH=${PROJECT_BASE_PATH}/entrypoint.sh
RUN touch $ENTRYPOINT_SH && \
    chmod 755 $ENTRYPOINT_SH
RUN echo "#!/bin/bash" >> $ENTRYPOINT_SH

###############################################################
# Install SSH
###############################################################
RUN apt-get install -y openssh-server
RUN mkdir /var/run/sshd

# Set root password for SSH access
RUN echo "root:${SSH_ROOT_PASSWORD}" | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
EXPOSE 22

# Autostart service when booting
RUN echo "echo \"Starting server [SSH]\"" >> $ENTRYPOINT_SH && \
    echo "/usr/sbin/sshd" >> $ENTRYPOINT_SH

###############################################################
# Install FDFS dependencies libfastcommon v1.0.75
###############################################################
RUN apt-get install -y gcc g++ make

# FastDFS package install path and also data and log save path
ENV FDFS_BASE_PATH=${PROJECT_BASE_PATH}/fdfs
RUN mkdir -p $FDFS_BASE_PATH

RUN wget -P $FDFS_BASE_PATH https://github.com/happyfish100/libfastcommon/archive/refs/tags/V1.0.75.tar.gz && \
    tar -xzvf $FDFS_BASE_PATH/V1.0.75.tar.gz -C $FDFS_BASE_PATH && \
    rm $FDFS_BASE_PATH/V1.0.75.tar.gz

RUN cd $FDFS_BASE_PATH/libfastcommon-1.0.75 && \
    chmod +x make.sh && \
    echo "Current directory:" && pwd && \
    echo "Files in current directory:" && ls -l && \
    ./make.sh clean && \
    ./make.sh && \
    ./make.sh install

###############################################################
# Install FDFS dependencies libserverframe v1.2.5
###############################################################
RUN wget -P $FDFS_BASE_PATH https://github.com/happyfish100/libserverframe/archive/refs/tags/V1.2.5.tar.gz && \
    tar -xzvf $FDFS_BASE_PATH/V1.2.5.tar.gz -C $FDFS_BASE_PATH && \
    rm $FDFS_BASE_PATH/V1.2.5.tar.gz

RUN cd $FDFS_BASE_PATH/libserverframe-1.2.5 && \
    chmod +x make.sh && \
    echo "Current directory:" && pwd && \
    echo "Files in current directory:" && ls -l && \
    ./make.sh clean && \
    ./make.sh && \
    ./make.sh install

###############################################################
# Install fdfs v6.12.2
###############################################################
RUN wget -P $FDFS_BASE_PATH https://github.com/happyfish100/fastdfs/archive/refs/tags/V6.12.2.tar.gz && \
    tar -xzvf $FDFS_BASE_PATH/V6.12.2.tar.gz -C $FDFS_BASE_PATH && \
    rm $FDFS_BASE_PATH/V6.12.2.tar.gz

RUN cd $FDFS_BASE_PATH/fastdfs-6.12.2 && \
    chmod +x make.sh && \
    echo "Current directory:" && pwd && \
    echo "Files in current directory:" && ls -l && \
    ./make.sh clean && \
    ./make.sh && \
    ./make.sh install

# Create base path to save data and log for FastDFS
RUN mkdir -p ${FDFS_BASE_PATH}/tracker && \
    mkdir -p ${FDFS_BASE_PATH}/storage/base && \
    mkdir -p ${FDFS_BASE_PATH}/storage/store && \
    mkdir -p ${FDFS_BASE_PATH}/client

# Copy config file to FastDFS
RUN cp ${PROJECT_SOURCE_CODE_PATH}/server/config/tracker.conf /etc/fdfs/tracker.conf && \
    cp ${PROJECT_SOURCE_CODE_PATH}/server/config/storage.conf /etc/fdfs/storage.conf && \
    cp ${PROJECT_SOURCE_CODE_PATH}/server/config/client.conf /etc/fdfs/client.conf

EXPOSE 22122 23000

# Autostart service when booting
RUN echo "echo \"Starting server [fdfs_trackerd]\"" >> $ENTRYPOINT_SH && \
    echo "fdfs_trackerd /etc/fdfs/tracker.conf start" >> $ENTRYPOINT_SH

RUN echo "echo \"Starting server [fdfs_storaged]\"" >> $ENTRYPOINT_SH && \
    echo "fdfs_storaged /etc/fdfs/storage.conf start" >> $ENTRYPOINT_SH

###############################################################
# Install Redis 
###############################################################
ENV REDIS_BASE_PATH=${PROJECT_BASE_PATH}/redis
RUN mkdir $REDIS_BASE_PATH && \
    cd $REDIS_BASE_PATH && \
    git clone https://github.com/redis/redis.git && \
    cd redis && \
    git checkout 7.4.1 && \
    make && \
    make install

# 用于记录 redis pid
RUN cd $REDIS_BASE_PATH && \
    touch redis.pid

# 用于记录 redis 日志的文件
RUN cd $REDIS_BASE_PATH && \
    touch redis.log

# Copy config file to FastDFS
RUN cp ${PROJECT_SOURCE_CODE_PATH}/server/config/redis.conf ${REDIS_BASE_PATH}/redis/redis.conf

# Autostart service when booting
RUN echo "echo \"Starting server [Redis]\"" >> $ENTRYPOINT_SH && \
    echo "redis-server /root/foxcloudservers/foxcloud/server/config/redis.conf" >> $ENTRYPOINT_SH

###############################################################
# Install HiRedis
###############################################################
RUN cd $REDIS_BASE_PATH && \
    git clone https://github.com/redis/hiredis.git && \
    cd hiredis && \
    make  && \
    make install

###############################################################
# Install PosrgreSQL v9.5
###############################################################
# RUN apt-get install -y postgresql postgresql-contrib

# # 配置 PostgreSQL 监听所有 IP 地址
# RUN echo "listen_addresses='*'" >> /etc/postgresql/9.5/main/postgresql.conf

# # 配置 pg_hba.conf 以允许所有远程连接
# RUN echo "host all all 0.0.0.0/0 md5" >> /etc/postgresql/9.5/main/pg_hba.conf

# # 启动 PostgreSQL 服务并设置默认用户、数据库
# RUN service postgresql start && \
#     su - postgres -c "psql -c \"CREATE USER $DB_USERNAME WITH PASSWORD '$DB_PASSWORD';\"" && \
#     su - postgres -c "psql -c \"CREATE DATABASE $DB_NAME OWNER $DB_USERNAME;\"" && \
#     su - postgres -c "psql -c \"ALTER USER $DB_USERNAME WITH SUPERUSER;\""

# # 暴露 PostgreSQL 默认端口
# EXPOSE 5432

# # Autostart service when booting
# RUN echo "echo \"Starting server [PosrgreSQL]\"" >> $ENTRYPOINT_SH && \
#     echo "service postgresql start" >> $ENTRYPOINT_SH


###############################################################
# 安装 MySQL Server
###############################################################
# 环境变量
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y mysql-server libmysqlclient-dev

# 配置 MySQL
# 修改配置文件，允许远程连接
RUN sed -i "s/^bind-address\s*=.*/bind-address = 0.0.0.0/" /etc/mysql/mysql.conf.d/mysqld.cnf

# 添加 UTF-8 编码和表名不区分大小写的配置
RUN echo "[mysqld]" >> /etc/mysql/mysql.conf.d/mysqld.cnf && \
    echo "character-set-server=utf8mb4" >> /etc/mysql/mysql.conf.d/mysqld.cnf && \
    echo "collation-server=utf8mb4_general_ci" >> /etc/mysql/mysql.conf.d/mysqld.cnf && \
    echo "lower_case_table_names=1" >> /etc/mysql/mysql.conf.d/mysqld.cnf && \
    echo "[client]" >> /etc/mysql/mysql.conf.d/mysqld.cnf && \
    echo "default-character-set=utf8mb4" >> /etc/mysql/mysql.conf.d/mysqld.cnf

# 安装 envsubst
RUN apt-get install -y gettext-base

# 复制 init.sql.template 并替换环境变量生成 init.sql
COPY init.sql /docker-entrypoint-initdb.d/init.sql
RUN envsubst < /docker-entrypoint-initdb.d/init.sql > /docker-entrypoint-initdb.d/init_processed.sql


# 初始化数据库
RUN service mysql start && \
    mysql -u root < /docker-entrypoint-initdb.d/init_processed.sql

# 暴露 MySQL 默认端口
EXPOSE 3306

# Autostart service when booting
RUN echo "echo \"Starting server [MySQL]\"" >> $ENTRYPOINT_SH && \
    echo "service mysql start" >> $ENTRYPOINT_SH

    
###############################################################
# Install Nginx dependencies openssl v3.4.0
###############################################################
ENV NGINX_BASE_PATH=${PROJECT_BASE_PATH}/nginx
RUN mkdir $NGINX_BASE_PATH && \
    cd $NGINX_BASE_PATH && \
    wget https://github.com/openssl/openssl/releases/download/openssl-3.4.0/openssl-3.4.0.tar.gz && \
    tar -xzvf openssl-3.4.0.tar.gz && \
    rm openssl-3.4.0.tar.gz && \
    cd ./openssl-3.4.0 && \
    ./config && \
    make && \
    make install

###############################################################
# Install Nginx dependencies pcre v8.45
###############################################################
RUN cd $NGINX_BASE_PATH && \
    wget https://sourceforge.net/projects/pcre/files/pcre/8.45/pcre-8.45.tar.gz  && \
    tar -xzvf pcre-8.45.tar.gz && \
    rm pcre-8.45.tar.gz && \
    cd pcre-8.45 && \
    ./configure && \
    make && \
    make install

###############################################################
# Install Nginx dependencies zlib v1.3.1
###############################################################
RUN cd $NGINX_BASE_PATH && \
    wget https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz && \
    tar -xzvf zlib-1.3.1.tar.gz && \
    rm zlib-1.3.1.tar.gz && \
    cd ./zlib-1.3.1 && \
    ./configure && \
    make && \
    make install

###############################################################
# Install nginx v1.16.1
# 由于 FDFS 兼容性问题，所以使用 1.16.1 版本
# Nginx 的配置位于 /usr/local/nginx/
###############################################################
RUN cd $NGINX_BASE_PATH && \
    wget https://nginx.org/download/nginx-1.16.1.tar.gz && \
    tar -xzvf nginx-1.16.1.tar.gz && \
    rm nginx-1.16.1.tar.gz && \
    cd ./nginx-1.16.1 && \
    ./configure && \
    make && \
    make install && \
    ln -s /usr/local/nginx/sbin/nginx /usr/bin/nginx

RUN cp ${PROJECT_SOURCE_CODE_PATH}/server/config/nginx.conf /usr/local/nginx/conf/nginx.conf
RUN cp -r ${PROJECT_SOURCE_CODE_PATH}/server/index-html  /usr/local/nginx/

EXPOSE 80

# Autostart service when booting
RUN echo "echo \"Starting server [nginx]\"" >> $ENTRYPOINT_SH && \
    echo "/usr/local/nginx/sbin/nginx" >> $ENTRYPOINT_SH

###############################################################
# Install FastCGI v2.4.1
# 如果是 arm 处理器使用以下 configure 指令参数
# ./configure --host=arm-linux  --prefix=/usr  --build=arm-linux

# 使用 C++ 编译 FastCGI 程序：
# g++ echo-cpp.cpp -o test -lfcgi++ -lfcgi -std=c++11
###############################################################
RUN cd $NGINX_BASE_PATH && \
    git clone https://github.com/Kuzmenko-Pavel/FastCGI.git && \
    cd FastCGI && \
    if [ "$TARGETARCH" = "arm64" ]; then \
        ./configure --host=arm-linux --prefix=/usr --build=arm-linux; \
    else \
        ./configure; \
    fi && \
    make && \
    make install

###############################################################
# Install spawn-fcgi v1.6.3
# 其用于作为 Nginx 与 FastCGI 的连接桥梁
# 使用 spawn-fcgi -a HOST -p PORT ./EXE 启动
# https://github.com/lighttpd/spawn-fcgi
###############################################################
RUN apt-get install -y automake
RUN cd $NGINX_BASE_PATH && \
    wget https://github.com/lighttpd/spawn-fcgi/archive/refs/tags/v1.6.3.tar.gz && \
    tar -xzvf v1.6.3.tar.gz && \
    rm v1.6.3.tar.gz && \
    cd ./spawn-fcgi-1.6.3 && \
    ./autogen.sh && \
    ./configure && \
    make && \
    make install

###############################################################
# Install and config fastdfs-nginx-module v1.24
###############################################################
RUN cd $FDFS_BASE_PATH && \
    git clone https://github.com/happyfish100/fastdfs-nginx-module.git && \
    cd fastdfs-nginx-module && \
    git checkout V1.24

RUN cd $NGINX_BASE_PATH && \
    cd nginx-1.16.1 && \
    ./configure --add-module=${FDFS_BASE_PATH}/fastdfs-nginx-module/src && \
    make && \
    make install
    
# Config fastdfs-nginx-module，以便FDFS能够通过 URL 直接访问内容
RUN cp ${PROJECT_SOURCE_CODE_PATH}/server/config/mod_fastdfs.conf /etc/fdfs/  && \
    cp ${FDFS_BASE_PATH}/fastdfs-6.12.2/conf/http.conf /etc/fdfs/ && \
    cp ${NGINX_BASE_PATH}/nginx-1.16.1/conf/mime.types /etc/fdfs/

###############################################################
# 编译 自己编写的 CGI 脚本
###############################################################
RUN cd ${PROJECT_SOURCE_CODE_PATH}/server && \
    make clean  && \
    make

# 使用脚本进行启动
RUN echo "cd ${PROJECT_SOURCE_CODE_PATH}/server" >> $ENTRYPOINT_SH && \
    echo "./start.sh" >> $ENTRYPOINT_SH

###############################################################
# 自动启动服务
###############################################################
RUN echo "echo \"Running servers on current moment:\"" >> $ENTRYPOINT_SH && \
    echo "ps aux" >> $ENTRYPOINT_SH

# 保持容器运行
RUN echo "tail -f /dev/null" >> $ENTRYPOINT_SH


###############################################################
# 清除不必要文件来减小镜像大小
###############################################################
RUN rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["sh", "-c", "$ENTRYPOINT_SH"]
