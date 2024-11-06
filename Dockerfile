FROM ubuntu:16.04

########################### SSH ###########################
RUN apt-get update && apt-get install -y openssh-server
RUN mkdir /var/run/sshd

# Set root password for SSH access (change 'YOUR_PASSWORD' to your desired password)
RUN echo 'root:YOUR_PASSWORD' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
EXPOSE 22

########################### fdfs (libfastcommon) ###########################
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

########################### fdfs (libfastcommon) ###########################
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

########################### fdfs ###########################
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

########################### PosrgreSQL ###########################
RUN apt-get install -y postgresql

########################### install other packages ###########################
RUN apt-get install -y vim net-tools

CMD ["/usr/sbin/sshd", "-D"]

