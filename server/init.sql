-- 创建数据库
CREATE DATABASE IF NOT EXISTS ${DB_NAME};

-- 切换到创建的数据库
USE ${DB_NAME};

-- 创建用户和赋予权限
CREATE USER IF NOT EXISTS '${DB_USERNAME}'@'%' IDENTIFIED BY '${DB_PASSWORD}';
GRANT ALL PRIVILEGES ON ${DB_NAME}.* TO '${DB_USERNAME}'@'%';
FLUSH PRIVILEGES;

-- 创建表 user
CREATE TABLE user (
    id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR (128) NOT NULL,
    nickname VARCHAR (128) NOT NULL,
    password VARCHAR (128) NOT NULL,
    phone VARCHAR (15) NOT NULL,
    createtime VARCHAR (128),
    email VARCHAR (100),
    CONSTRAINT uq_nickname UNIQUE (nickname),
    CONSTRAINT uq_name UNIQUE (NAME)
);

-- 创建表 file_info
CREATE TABLE file_info (
    md5 VARCHAR (200) NOT NULL PRIMARY KEY,
    file_id VARCHAR (256) NOT NULL,
    url VARCHAR (512) NOT NULL,
    size BIGINT,
    type VARCHAR (20),
    count INT
);

-- 创建表 user_file_list
CREATE TABLE user_file_list (
    user VARCHAR (128) NOT NULL,
    md5 VARCHAR (200) NOT NULL,
    createtime VARCHAR (128),
    filename VARCHAR (128),
    shared_status INT,
    pv INT
);

-- 创建表 user_file_count
CREATE TABLE user_file_count (
    user VARCHAR (128) NOT NULL PRIMARY KEY,
    count INT
);

-- 创建表 share_file_list
CREATE TABLE share_file_list (
    user VARCHAR (128) NOT NULL,
    md5 VARCHAR (200) NOT NULL,
    createtime VARCHAR (128),
    filename VARCHAR (128),
    pv INT
);
