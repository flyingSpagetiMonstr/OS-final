# 实验环境
- 主机：Windows 11 家庭中文版 Insider Preview
  - IDE：vscode
  - Orange's运行于：Bochs 2.7 for windows
  - 代码在WSL（64位Ubuntu）下编译生成二进制文件：Ubuntu 22.04.3 LTS (GNU/Linux 5.15.137.3-microsoft-standard-WSL2 x86_64)

# 编译
参考`run.cmd`
1. 编译OS内核：在工作区根目录下运行`make image`
2. 编译OS的可执行文件（和其它相关文件）：在`command/`下运行`make install`
  - 由于Orange's文件系统限制，无法同时载入较多文件，故一次运行只载入部分文件，见`command/Makefile: line15`，这里`#`后被注释掉的均不会被载入文件系统。
    ```makefile
    BIN = cat ticker mod stack target kill # echo pwd lseektest ls payload  
    ```
3. 运行Bochs
   - Bochs配置文件见`bochsrc.sh`

# 常量与设置
- 栈检查：由在`kernel/proc.c: line45`左右的`#define STACK_CHECK 1`控制是否启用
- 静态检测（可执行文件完整性）：由在`mm/exec.c: line24`左右的`#define TAMPER_CHK 0`控制是否启用

# 其它
- 在打开`stack_chk`时，运行`command/stack`时需要加上`&`，否则shell会因为等不到进程结束而一直阻塞

**实验报告见`others/操作系统实验结课报告`**
