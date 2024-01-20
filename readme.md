# 实验环境
- 主机：Windows 11 家庭中文版 Insider Preview
  - IDE：vscode
  - Orange's运行于：Bochs 2.7 for windows
  - 代码在WSL（64位Ubuntu）下编译生成二进制文件：Ubuntu 22.04.3 LTS (GNU/Linux 5.15.137.3-microsoft-standard-WSL2 x86_64)

# 编译
可参考`run.cmd`
- 编译OS内核：在工作区根目录下运行`make image`
- 编译OS的可执行文件（和其它相关文件）：在`command/`下运行`make install`
  - 由于Orange's文件系统限制，无法同时载入较多