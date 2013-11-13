DUMPMACHINE = $$system($${QMAKE_CXX} -dumpmachine)
GCC_TARGET_ARCH = $$find(DUMPMACHINE, x86_64)

count(GCC_TARGET_ARCH, 0){
    message("Building for 32 bit")
    LIBVARIANT = x32
} else {
    message("Building for 64 bit")
    LIBVARIANT = x64
}

CONFIG(release, debug|release) {
    LIBVARIANT = $${LIBVARIANT}/release
} else {
    LIBVARIANT = $${LIBVARIANT}/debug
}
