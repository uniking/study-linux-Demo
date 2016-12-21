两个客户端，一个去找远程服务器，一个找本地端口，两个之间进行完全的数据转发

远程客户端叫 CRemoteClient
本地客户端叫 CLocalClient
可以new 十万个CLocalClient来模拟十万个人访问网站

之间的传输叫 CPool