<div align="center">
<p>
 <img width="100px" src="https://raw.githubusercontent.com/NekoSilverFox/NekoSilverfox/403ab045b7d9adeaaf8186c451af7243f5d8f46d/icons/silverfox.svg" align="center" alt="NekoSilverfox" />
 <p align="center"><b><font size=6>FoxCloud</font></b></p>
 <p align="center"><b>高负载的分布式存储解决方案</b></p>
</p>


[![License](https://img.shields.io/badge/license-Apache%202.0-brightgreen)](LICENSE)
![FDFS](https://img.shields.io/badge/FDFS-v6.12.2-orange)
![libfastcommon](https://img.shields.io/badge/libfastcommon-v1.0.75-orange)
![libserverframe](https://img.shields.io/badge/libserverframe-v1.2.5-orange)

![PostgreSQL](https://img.shields.io/badge/PostgreSQL-v16-red)
![redis](https://img.shields.io/badge/redis-vXX-red)

![nginx](https://img.shields.io/badge/nginx-vXX-blue)
![FastCGI](https://img.shields.io/badge/FastCGI-vXX-blue.svg)
![FastDFS](https://img.shields.io/badge/FastDFS-vXX-yellow)


<div align="left">
<!-- 顶部至此截止 -->



[toc]



# 安装 FastDFS

## 通过 Docker 安装

1. 安装并启动 Docker

2. 拉取并进入仓库

    ```bash
    git clone git@github.com:NekoSilverFox/FoxClould.git
    cd ./FoxClould
    ```

3. 修改 `Dockerfile` 里面的 `YOUR_PASSWORD` 为 SSH 登录配置密码

4. 构建 FoxCloudServer 的 Docker 镜像

    ```bash
    docker build -t FoxCloudServer .
    ```

5. 启动容器（节点），其中需要配置的：

    | 配置项           | 说明                                         |
    | ---------------- | -------------------------------------------- |
    | `SSH_PORT`       | SSH 映射端口（根据需要启动）                 |
    | `TRACKER_PORT`   | `Tracker` 节点的映射端口，内部默认端口 22122 |
    | `STORGE_PORT`    | `Storge` 节点的映射端口，内部默认端口 23000  |
    | `CONTAINER_NAME` | 自定义容器名                                 |

    - 启动单独的 `Tracker` 节点

        ```
        docker run -id -p SSH_PORT:22 -p TRACKER_PORT:22122 --name TRACKER_CONTAINER_NAME FoxCloudServer
        ```

        

    - 启动单独的 `Storge` 节点

        ```
        docker run -id -p SSH_PORT:22 -p STORGE_PORT:23000 --name STORGE_CONTAINER_NAME FoxCloudServer
        ```

        

    - 如果 `Tracker` 节点和 `Storge` 节点在一台主机上，可使用

        ```
        docker run -id -p SSH_PORT:22 -p TRACKER_PORT:22122 -p STORGE_PORT:23000 --name CONTAINER_NAME FoxCloudServer
        ```

        

6. 使用 SSH 登录或者本地进入 Docker 容器后，进行配置



## 配置

**FDFS 的默认其配置文件都位于 `/etc/fdfs`**，我们需要配置 tracker、storage、client

### tracker

至少配置以下行

| 配置           | 说明                                                         |
| -------------- | ------------------------------------------------------------ |
| `bind_addr = ` | tracker 所在的**服务器IP地址**（不能是 127.0.0.1 和 localhost） |
| `port = `      | tracker 所运行的端口                                         |
| `base_path = ` | 存储 tracker 数据和日志的路径(要保证该目录已经实际存在)      |

### storage

至少配置以下行

| 配置                 | 说明                                                         |
| -------------------- | ------------------------------------------------------------ |
| `group_name = `      | 当前 storage 存储节点属于哪一个组                            |
| `bind_addr = `       | storage 所在的主机的 IP（不能是 127.0.0.1 和 localhost）     |
| `port = `            | storage 所运行的端口                                         |
| `base_path = `       | 存储 storage 数据和日志的路径(要保证该目录已经实际存在)      |
| `store_path_count =` | 存储目录的数量                                               |
| `store_path0 =`      | 实际存储数据路径，如果 `store_path_count` 指定了几个路径，就要有对应的几个 |
| `tracker_server = `  | tracker 的 IP 及端口                                         |

### client

至少配置以下行

| 配置                | 说明                                                  |
| ------------------- | ----------------------------------------------------- |
| `base_path = `      | client 数据和日志的路径(要保证该目录已经实际存在)     |
| `tracker_server = ` | tracker 的 IP 及端口（不能是 127.0.0.1 和 localhost） |

## 启动

**注意：如果启动之后不报错，但是使用使用 `ps aux | grep fdfs*` 查看 fdfs 服务实际也并没有启动。说明配置文件有问题，需要检查 `bind_addr` 是否配置正确（不能是 127.0.0.1 和 localhost），或者检查防火墙。**

> `fdfs_trackerd` 好 `fdfs_storaged` 后面的 `d` 代表启动后是守护进程

1. 启动/停止 `tracker`，**tracker 必须第一个启动，因为客户端和存储节点都要去被动的连接他**:

    ```
    fdfs_trackerd /etc/fdfs/tracker.conf start/stop
    ```

    

2. 启动/停止 `storage`:

    ```
    fdfs_storaged /etc/fdfs/storage.conf start/stop
    ```



## 验证

| 命令                                                 | 说明                                                         |
| ---------------------------------------------------- | ------------------------------------------------------------ |
| `fdfs_test`                                          | 验证 FDFS 的安装                                             |
| `ps aux | grep fdfs*`                                | 验证服务是否启动                                             |
| `fdfs_monitor /etc/fdfs/client.conf`                 | 执行查看状态，如果状态为 `ACTIVE` 则说明正常                 |
| `fdfs_upload_file /etc/fdfs/client.conf FILE`        | 使用 client 直接上传文件，会生成这么一段：`group1/M00/00/00/rBEAAmcNnNWACcJSAACXlD6bcgU7205557` |
| `fdfs_download_file /etc/fdfs/client.conf FILE_NAME` |                                                              |

上传文件时候生成字符串的解释：

**group1/M00/00/00/rBEAAmcNnNWACcJSAACXlD6bcgU7205557**

- `group1` - 组名
- `M00` - 虚拟磁盘路径，对应 `storage.conf` 中的 `store_path*=XXX`
- `00/00` 二级目录，存储上传文件的目录
- `rBEAAmcNnNWACcJSAACXlD6bcgU7205557` - 采用 Base64 进行编码的文件名所包含的信息：
    - 源 storage server IP 地址
    - 文件创建时间
    - 文件大小
    - 文件 CRC32 校验码（循环冗余校验）
    - 随机数

## 服务默认端口

| 说明    | 端口  |
| ------- | ----- |
| tracker | 22122 |
| storage | 23000 |
|         |       |



