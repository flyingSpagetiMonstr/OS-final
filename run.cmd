ubuntu run make image

cd command/
ubuntu run make install 
cd ..

set arg=%1
set root=D:\Program Files\Bochs-2.7\bochs
"%root%%arg%.exe" -f bochsrc.sh
