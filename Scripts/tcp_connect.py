# -*- coding: utf-8 -*-
"""
Created on Sat Jun 17 15:33:54 2023

@author: Administrator
"""

import socket
import time

def check_tcp_connection(host, port, msg):
    try:
        # 创建TCP套接字
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # 设置超时时间为3秒
        sock.settimeout(3)
        # 尝试连接
        result = sock.connect_ex((host, port))
        if result == 0:
            print(msg + "TCP连接正常" )
        else:
            print(msg + "TCP连接异常")
        # 关闭套接字
        sock.close()
    except socket.error as e:
        print(msg + "发生错误：", e)

trade_host_msg = "checking simnow 7x24 trade host: "
trade_host = "180.168.146.187"
trade_port = 10130

market_host_msg = "checking simnow 7x24 market host: "
market_host = "180.168.146.187"
market_port = 10131

# 检查TCP连接
while True:
    check_tcp_connection(trade_host, trade_port, trade_host_msg)
    check_tcp_connection(market_host, market_port, market_host_msg)
    time.sleep(5)