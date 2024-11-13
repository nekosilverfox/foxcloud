FROM ubuntu:16.04

########################### Install SSH ###########################
RUN apt-get update && apt-get install -y openssh-server
RUN mkdir /var/run/sshd

# Set root password for SSH access (change 'YOUR_PASSWORD' to your desired password)
RUN echo 'root:YOUR_PASSWORD' | chpasswd
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

########################### Install PosrgreSQL ###########################
RUN apt-get install -y postgresql


########################### Install Redis ###########################
ENV REDIS_BASE_PATH=/root/FoxCloud/redis
RUN mkdir $REDIS_BASE_PATH && \
    cd $REDIS_BASE_PATH && \
    wget https://download.redis.io/redis-stable.tar.gz && \
    tar -xzvf redis-stable.tar.gz && \
    cd redis-stable && \
    make && \
    make install && \
    cd ..  && \
    rm -rf redis-stable.tar.gz

########################### Install HiRedis ###########################
RUN cd $REDIS_BASE_PATH && \
    git clone https://github.com/redis/hiredis.git && \
    cd hiredis && \
    make  && \
    make install && \
    
########################### Install other packages ###########################

RUN apt-get install -y vim net-tools git

CMD ["/usr/sbin/sshd", "-D"]

