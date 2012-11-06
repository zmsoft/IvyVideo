if [[ $# != 1 && $# != 2 ]]; then
    echo "usage: $0 module"
    exit 1
fi

ext=so
if [ $# == 2 ]; then
    if [ $2 == "static" ]; then
        ext=a
    elif [ $2 == "shared" ]; then
        ext=so
    else
        echo "fail argument: $2, should be static or shared"
        exit 1
    fi
fi


mod=$1
modlist="x264 avutil avcodec avformat swscale postproc avfilter "
libpath=../libs/armeabi-v7a
dstpath=../../../../build/libs/armeabi-v7a

if [ $mod = "all" ]; then
    echo "[*] building all ...."
elif [ $mod = "clean" ]; then
    rm -f Android.mk Application.mk
    rm -rf ../obj/ ../libs/
    exit 0
else
    if [ ! -f ./Android-$mod.mk ]; then 
        echo "[*] no module $mod"
        exit 1
    fi
    modlist="$mod "
fi


for m in $modlist; 
do
    app=$m
    cat > Application.mk << EOF
APP_MODULES := $app
APP_OPTIM := release
APP_ABI := armeabi-v7a
APP_STL := stlport_static
EOF
    cp Android-$m.mk Android.mk
    if [ $ext == "a" ]; then
        sed -i s'/BUILD_SHARED_LIBRARY/BUILD_STATIC_LIBRARY/' Android.mk
    fi
    ndk-build 2>/tmp/err.log
    if [ $ext == "so" ]; then
        cp -f $libpath/lib$app.$ext $dstpath/
    fi
done

exit 0
