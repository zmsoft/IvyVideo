OBJ=libivyffmpeg.so
SRC_PATH=../libs/armeabi-v7a/
DST_PATH=../../libs/armeabi-v7a/

ndk-build 2>/tmp/err.log
cp -f $SRC_PATH/$OBJ $DST_PATH/
