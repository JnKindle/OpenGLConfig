



/*
 导入GLTool着色管理器（shader manager）类，没有着色器，我们就不能再OpenGL（核心框架进行着色）。
着色器管理器不仅允许我们创建并管理着色器，还提供一组“存储着色器”，他们能进行一些初步基本的渲染操作
 */
#include "GLShaderManager.h"
//GLTool.h头文件包含了大部分GLTool中类似C语言的独立函数
#include "GLTools.h"
//在MAC系统下，我们使用freeglut的静态库版本并且需要添加一个宏
#include <GLUT/GLUT.h>


GLShaderManager shaderManager; //定义一个着色管理器
GLBatch triangleBatch; //定义一个三角形批次容器，此处，我们绘制一个三角形


/*
 ChangeSize函数
 自定义函数，通过glutReshaperFunc（函数名）注册为重塑函数
 ** 触发条件：
 1、当屏幕大小发生变化
 2、第一次创建窗口时，会调用该函数
 ** 处理业务：
 1、设置OpenGL视口
 2、设置OpenGl投影方式等
 */
void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}


/*
 SetUpRC函数
 自定义函数，设置你需要渲染的图形的相关顶点数据/颜色数据等数据装备工作
 ** 触发条件
 1、手动main函数触发
 
 ** 处理业务
 1、设置窗口背景颜色
 2、初始化存储着色器shaderManager
 3、设置图形顶点数据
 4、利用三角形批次类，将数据传递到着色器
 */
void SetUpRC()
{
    //设置背景颜色
    glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
    
    //初始化着色器管理器
    shaderManager.InitializeStockShaders();
    
    //设置三角形，其中数组vVert包含所有3个顶点的x,y,笛卡尔坐标对
    GLfloat vVerts[] = {
        -0.5f,0.0f,0.0f,
        0.5f,0.0f,0.0f,
        0.0f,0.5f,0.0f,
    };
    
    //批次处理
    triangleBatch.Begin(GL_TRIANGLES, 3);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}


/*
 自定义函数
 通过glutDisplayFunc(函数名)注册为显示渲染函数
 当屏幕发生变化/或者开发者主动渲染会调用，用来实现数据->渲染过程
 ** 触发条件
 1、系统自动触发
 2、开发者手动调用函数触发
 ** 处理业务
 1、清理缓存区（颜色，深度，模板缓存区等）
 2、使用存储着色器
 3、绘制图形
 */
void RenderScene(void)
{
    //清楚缓存区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    //设置一组表示颜色的浮点数
    GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};
    
    //传递存储着色器，
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    
    //提交着色器
    triangleBatch.Draw();
    
    //在后台缓冲区进行渲染，然后在结束时交换到前台
    glutSwapBuffers();
}



/*
 main函数
 程序入口，OpenGL是面向过程编程。
 所以OpenGL处理图形/图像都是链式形式。以及其他一些基于OpenGL封装的图像处理框架也是链式编程。
 */
int main(int argc, char* argv[])
{
    //设置当前工作目录
    gltSetWorkingDirectory(argv[0]);
    //初始化话GLUT库
    glutInit(&argc, argv);
    //初始化双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH |GLUT_STENCIL);
    //初始化GLUT窗口大小
    glutInitWindowSize(300, 200);
    //设置GLUT窗口标题
    glutCreateWindow("Triangle");
    
    //注册回调函数
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    
    //驱动程序的初始化有没有出现问题
    GLenum error = glewInit();
    if (GLEW_OK != error) {
        fprintf(stderr, "glew error:%s\n",glewGetErrorString(error));
        return 1;
    }
    
    //调用SetUpRC函数,设置相关数据
    SetUpRC();
    
    //类似于iOS runloop运行循环
    glutMainLoop();
    
    return 0;
}








