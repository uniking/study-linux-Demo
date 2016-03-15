1，以root权限增加配置
ulimit -c unlimited
ulimit unlimited
2，不改变shell环境，以当前root运行可执行程序，崩溃后会出现core
3，使用core  gdb my.exe core.323776  来还原现场。
