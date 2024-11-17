FROM ubuntu:16.04

LABEL description="FoxCloud Server docker image"
LABEL maintainer="nekosilverfox"

# 配置为自己的
ENV POSTGRE_USER=your_pg_user_name
ENV POSTGRE_PASSWORD=your_pg_password
ENV POSTGRE_DB=your_pg_database
ENV SSH_ROOT_PASSWORD=your_ssh_password

##################### Install other packages #####################
RUN apt-get update
RUN apt-get install -y vim net-tools git


##################### 克隆仓库到容器（需要利用仓库中的配置文件） #####################
ENV PROJECT_BASE_PATH=/root/foxcloudservers/
RUN mkdir $PROJECT_BASE_PATH && \
    cd $PROJECT_BASE_PATH && \
    git clone https://github.com/NekoSilverFox/foxcloud.git


##################### 开机自动启动的服务 #####################
ENV ENTRYPOINT_SH=${PROJECT_BASE_PATH}entrypoint.sh
RUN touch $ENTRYPOINT_SH && \
    chmod 755 $ENTRYPOINT_SH
RUN echo "#!/bin/bash" >> $ENTRYPOINT_SH && \
    echo "set -e" >> $ENTRYPOINT_SH

########################## Install SSH ##########################
RUN apt-get install -y openssh-server
RUN mkdir /var/run/sshd

# Set root password for SSH access
RUN echo "root:${SSH_ROOT_PASSWORD}" | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
EXPOSE 22

# 开机启动
RUN echo "/usr/sbin/sshd -D" >> $ENTRYPOINT_SH

################# Install fdfs (libfastcommon) #################
# install other packages
RUN apt-get install -y gcc g++ make
ENV FDFS_BASE_PATH=${PROJECT_BASE_PATH}fdfs
RUN mkdir -p $FDFS_BASE_PATH

# We use the last version in 14.10.2024
# Install Fastdfs (libfastcommon-1.0.75)
RUN wget -P $FDFS_BASE_PATH https://github.com/happyfish100/libfastcommon/archive/refs/tags/V1.0.75.tar.gz && \
    tar -xzvf $FDFS_BASE_PATH/V1.0.75.tar.gz -C $FDFS_BASE_PATH && \
    rm $FDFS_BASE_PATH/V1.0.75.tar.gz

# 切换工作目录（如果不切换，sh 脚本无法正常执行），并执行 make.sh
RUN cd $FDFS_BASE_PATH/libfastcommon-1.0.75 && \
    chmod +x make.sh && \
    echo "Current directory:" && pwd && \
    echo "Files in current directory:" && ls -l && \
    ./make.sh clean && \
    ./make.sh && \
    ./make.sh install

################# Install fdfs (libserverframe) #################
RUN wget -P $FDFS_BASE_PATH https://github.com/happyfish100/libserverframe/archive/refs/tags/V1.2.5.tar.gz && \
    tar -xzvf $FDFS_BASE_PATH/V1.2.5.tar.gz -C $FDFS_BASE_PATH && \
    rm $FDFS_BASE_PATH/V1.2.5.tar.gz

# 切换工作目录（如果不切换，sh 脚本无法正常执行），并执行 make.sh
RUN cd $FDFS_BASE_PATH/libserverframe-1.2.5 && \
    chmod +x make.sh && \
    echo "Current directory:" && pwd && \
    echo "Files in current directory:" && ls -l && \
    ./make.sh clean && \
    ./make.sh && \
    ./make.sh install

##################### Install fdfs #####################
# Download, extract, and clean up Fastdfs
RUN wget -P $FDFS_BASE_PATH https://github.com/happyfish100/fastdfs/archive/refs/tags/V6.12.2.tar.gz && \
    tar -xzvf $FDFS_BASE_PATH/V6.12.2.tar.gz -C $FDFS_BASE_PATH && \
    rm $FDFS_BASE_PATH/V6.12.2.tar.gz

# 切换工作目录（如果不切换，sh 脚本无法正常执行），并执行 make.sh
RUN cd $FDFS_BASE_PATH/fastdfs-6.12.2 && \
    chmod +x make.sh && \
    echo "Current directory:" && pwd && \
    echo "Files in current directory:" && ls -l && \
    ./make.sh clean && \
    ./make.sh && \
    ./make.sh install

# 开机启动
RUN echo "fdfs_trackerd /etc/fdfs/tracker.conf start" >> $ENTRYPOINT_SH
RUN echo "fdfs_storaged /etc/fdfs/storage.conf start" >> $ENTRYPOINT_SH


########################### Install Redis ###########################
ENV REDIS_BASE_PATH=${PROJECT_BASE_PATH}redis
RUN mkdir $REDIS_BASE_PATH && \
    cd $REDIS_BASE_PATH && \
    git clone https://github.com/redis/redis.git && \
    cd redis && \
    make && \
    make install

# 用于记录 redis 日志
RUN cd $REDIS_BASE_PATH && \
    touch redis.log

# 开机启动
RUN echo "redis-server /root/foxcloudservers/foxcloud/server/config/redis.conf" >> $ENTRYPOINT_SH

########################### Install HiRedis ###########################
RUN cd $REDIS_BASE_PATH && \
    git clone https://github.com/redis/hiredis.git && \
    cd hiredis && \
    make  && \
    make install

########################### Install PosrgreSQL ###########################
RUN apt-get install -y postgresql postgresql-contrib

# 配置 PostgreSQL 监听所有 IP 地址
RUN echo "listen_addresses='*'" >> /etc/postgresql/9.5/main/postgresql.conf

# 配置 pg_hba.conf 以允许所有远程连接
RUN echo "host all all 0.0.0.0/0 md5" >> /etc/postgresql/9.5/main/pg_hba.conf

# 启动 PostgreSQL 服务并设置默认用户、数据库
RUN service postgresql start && \
    su - postgres -c "psql -c \"CREATE USER $POSTGRE_USER WITH PASSWORD '$POSTGRE_PASSWORD';\"" && \
    su - postgres -c "psql -c \"CREATE DATABASE $POSTGRE_DB OWNER $POSTGRE_USER;\"" && \
    su - postgres -c "psql -c \"ALTER USER $POSTGRE_USER WITH SUPERUSER;\""

# 暴露 PostgreSQL 默认端口
EXPOSE 5432

# 开机启动
RUN echo "service postgresql start" >> $ENTRYPOINT_SH

########################### Nginx 依赖包 openssl ###########################
ENV NGINX_BASE_PATH=${PROJECT_BASE_PATH}nginx
RUN mkdir $NGINX_BASE_PATH && \
    cd $NGINX_BASE_PATH && \
    wget https://github.com/openssl/openssl/releases/download/openssl-3.4.0/openssl-3.4.0.tar.gz && \
    tar -xzvf openssl-3.4.0.tar.gz && \
    rm openssl-3.4.0.tar.gz && \
    cd ./openssl-3.4.0 && \
    ./config && \
    make && \
    make install

########################### Nginx 依赖包 pcre ###########################
RUN cd $NGINX_BASE_PATH && \
    wget https://sourceforge.net/projects/pcre/files/pcre/8.45/pcre-8.45.tar.gz  && \
    tar -xzvf pcre-8.45.tar.gz && \
    rm pcre-8.45.tar.gz && \
    cd pcre-8.45 && \
    ./configure && \
    make && \
    make install

########################### Nginx 依赖包 zlib ###########################
RUN cd $NGINX_BASE_PATH && \
    wget https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz && \
    tar -xzvf zlib-1.3.1.tar.gz && \
    rm zlib-1.3.1.tar.gz && \
    cd ./zlib-1.3.1 && \
    ./configure && \
    make && \
    make install

########################### Nginx ###########################
# 由于 FDFS 兼容性问题，所以使用 1.16.1 版本
# Nginx 的配置位于 /usr/local/nginx/
RUN cd $NGINX_BASE_PATH && \
    wget https://nginx.org/download/nginx-1.16.1.tar.gz && \
    tar -xzvf nginx-1.16.1.tar.gz && \
    rm nginx-1.16.1.tar.gz && \
    cd ./nginx-1.16.1 && \
    ./configure && \
    make && \
    make install && \
    ln -s /usr/local/nginx/sbin/nginx /usr/bin/nginx

# 开机启动
RUN echo "nginx" >> $ENTRYPOINT_SH

########################### 自动启动服务 ###########################
ENTRYPOINT ["sh", "-c", "$ENTRYPOINT_SH"]
