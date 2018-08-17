
# 安装 rabbitMQ 的C语言开发库rabbitmq-c SimpleAmqpClient 的依赖项
sudo apt-get install  net-tools git libboost-dev openssl  libssl-dev cmake vim autoconf automake libtool libffi-dev doxygen graphviz libpopt-dev xmlto libboost-all-dev libmysqlcppconn-dev
cd ../
# 安装 rabbitmq-c
if [ ! -d "rabbitmq-c" ]; then
    git clone https://github.com/alanxz/rabbitmq-c
    cd rabbitmq-c
    if [ ! -d "build" ]; then
        mkdir build 
        cd build
        cmake ..
        cmake --build .
        make
        sudo make install
        cp librabbitmq/librabbitmq.so* ../../Run-Project/so/
        # 切换回主目录
        cd ../../
    fi
fi

# 安装 SimpleAmqpClient
if [ ! -d "SimpleAmqpClient" ]; then
    git clone https://github.com/alanxz/SimpleAmqpClient
    cd SimpleAmqpClient
    if [ ! -d "build" ]; then
        mkdir build
        cd build
        cmake ..
        make
        sudo make install
        cp libSimpleAmqpClient.so* ../../Run-Project/so/
        # 切换回主目录
        cd ../../
    fi
fi

# 安装 rapidjson
if [ ! -d "rapidjson" ]; then
    git clone https://github.com/Tencent/rapidjson.git
    cd rapidjson
    if [ ! -d "build" ]; then
        mkdir build
        cd build
        cmake ..
        make
        sudo make install
        # 切换回主目录
        cd ../../
    fi
fi

# 下载libcurl
if [ ! -f "curl-7.60.0.tar.gz" ]; then
    wget https://curl.haxx.se/download/curl-7.60.0.tar.gz
fi
# 解压libcurlls
if [ ! -d "curl-7.60.0" ]; then
    tar -zxf curl-7.60.0.tar.gz
    # 编译安装libcurl
    cd curl-7.60.0
    sudo ./configure
    sudo make
    sudo make install
    cd ../
fi

# 下载libuuid
if [ ! -f "libuuid-1.0.3.tar.gz" ]; then
    wget https://cfhcable.dl.sourceforge.net/project/libuuid/libuuid-1.0.3.tar.gz
fi
# 解压libuuid
if [ ! -d "libuuid-1.0.3" ]; then
    tar -zxf libuuid-1.0.3.tar.gz
    # 编译安装libuuid
    cd libuuid-1.0.3
    sudo ./configure
    sudo make
    sudo make install
    cp ./.libs/libuuid.so* ../../Run-Project/so/
    cd ../
fi

# 回到 common-base 目录
cd common-base/
# 编译生成Base库
make clean
make install
