#version 330 core

void main()
{
    // gl_FragDepth = gl_FragCoord.z;//由于我们没有颜色缓冲，最后的片元不需要任何处理
    //我们可以取消这行的注释，来显式设置深度，但是这个（指注释掉那行之后）是更有效率的，因为底层无论如何都会默认去设置深度缓冲
}
