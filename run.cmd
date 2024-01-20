@REM ubuntu run make image

@REM cd command/
@REM ubuntu run make install 
@REM cd ..

set arg=%1
set root=D:\Program Files\Bochs-2.7\bochs
"%root%%arg%.exe" -f bochsrc.sh
