FROM ubuntu:16.04

# 配置为自己的
ENV POSTGRE_USER=your_pg_user_name \
    POSTGRE_PASSWORD=your_pg_password \
    POSTGRE_DB=your_pg_database \
    SSH_ROOT_PASSWORD=your_ssh_password

########################### Install other packages ###########################
RUN apt-get update
RUN apt-get install -y vim net-tools git

########################### Install SSH ###########################
RUN apt-get install -y openssh-server
RUN mkdir /var/run/sshd

# Set root password for SSH access
RUN echo "root:${SSH_ROOT_PASSWORD}" | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
EXPOSE 22

########################### Install fdfs (libfastcommon) ###########################
# install other packages
RUN apt-get install -y gcc g++ make
ENV FDFS_BASE_PATH=/root/FoxCloud/fdfs
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

########################### Install fdfs (libserverframe) ###########################
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

########################### Install fdfs ###########################
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

########################### Install Redis ###########################
ENV REDIS_BASE_PATH=/root/FoxCloud/redis
RUN mkdir $REDIS_BASE_PATH && \
    cd $REDIS_BASE_PATH && \
    git clone https://github.com/redis/redis.git && \
    cd redis && \
    make && \
    make install

RUN cd $REDIS_BASE_PATH && \
    touch redis.log

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


########################### Docker 容器每次启动时候需要运行的命令 ###########################
ENV PROJECT_BASE_PATH=/root/FoxCloud/
RUN cd $PROJECT_BASE_PATH && \
    git clone https://github.com/NekoSilverFox/FoxCloud.git && \
    cd FoxCloud && \
    chmod 755 entrypoint.sh

ENTRYPOINT ["/root/FoxCloud/FoxCloud/entrypoint.sh"]
