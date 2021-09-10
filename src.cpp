#include <iostream>
#include <graph2d.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <stdlib.h>
using namespace std;
using namespace graph;
void addShapes(const char *filename,char *ch);
unsigned char red=102;
unsigned char blue=255;
unsigned char green=204;//默认色为天依蓝
float sx=500,sy=400;//center point
float mpx,mpy,mmx,mmy;//save press data and move data
int keyb=0;//keyb为键值记录
int sb=0;//sb为哨兵,99代表按下了右键
int pn=5;//边数，同时作用于curve和rectangle
int randnum()//随机数生成函数
{
    int a;
    srand((unsigned)time(NULL));
    a = rand() % 4;
    return a;
}
void reset()//默认值
{
    int a =randnum();
    sx=500;
    sy=400;
    keyb=0;
    sb=0;
    pn=5;
    if(a==0)
    {
        red=80;
        green=118;
        blue=245;
    }
    else if (a==1)
    {
        red=116;
        green=185;
        blue=191;
    }
    else if (a==2)
    {
        red=50;
        green=98;
        blue=141;
    }
    else if(a==3)
    {
        red=102;
        green=204;
        blue=255;
    }

}
class CPoint2D
{
    float x, y;
public:
    CPoint2D(float x=0, float y=0)
    {
        this->x = x;
        this->y = y;
    }
    void Translate(float x, float y)
    {
        this->x = this->x + x;
        this->y = this->y + y;
    }
    void Scale(float r)
    {
        x = r*x;
        y = r*y;
    }
    friend class CShape;
    friend class CTriangle;
    friend class CRectangle;
    friend class CEllipse;
    friend class CDonut;
    friend class CCurve;
    friend class CPolygon;
    friend class CCircle;
};
class CShape
{
protected:
    CPoint2D center;
    ULONG color;
public:
    CShape():center(CPoint2D(400,300))
    {
        color = 0x66CCFF;
    }
    CShape(float x, float y, ULONG color):center(x, y)
    {
        this->color = color;
    }
    void Translate(float x, float y)
    {
        center.Translate(x, y);
    }
    virtual void Draw() =0;
    virtual void Scale(float r) = 0;
};
class CRectangle: public CShape
{
    float len;
    float wid;
public:
    CRectangle()
    {
        len = 300;
        wid = 200;
        color = 0x5076F5;
    }
    CRectangle(float x, float y, float len,
               float wid, ULONG color):CShape(x, y, color)
    {
        this->len = len;
        this->wid = wid;
    }
    void Draw()
    {
        CPoint2D v1(center.x-len/2, center.y+wid/2);
        CPoint2D v2(center.x+len/2, center.y-wid/2);
        setColor(color);
        fillRectangle(v1.x, v1.y, v2.x, v2.y);
        setColor(BLACK);
        rectangle(v1.x, v1.y, v2.x, v2.y);
    }
    void Scale(float r)
    {
        center.Scale(r);
        len = len*r;
        wid = wid*r;
    }
};
class CEllipse: public CShape
{
protected:
    float xRadius;
    float yRadius;
public:
    CEllipse()
    {
        xRadius = 150;
        yRadius = 100;
    }
    CEllipse(float x, float y, float rx,
             float ry, ULONG color):CShape(x, y, color)
    {
        this->xRadius = 0.5*rx;
        this->yRadius = 0.5*ry;
    }
    void Draw()
    {
        setColor(color);
        fillEllipse(center.x, center.y, xRadius, yRadius);
        setColor(BLACK);
        ellipse(center.x, center.y, xRadius, yRadius);
    }
    void Scale(float r)
    {
        center.Scale(r);
        xRadius = xRadius*r;
        yRadius = yRadius*r;
    }
};
class CCircle: public CShape
{
protected:
    float rad;
public:
    CCircle()
    {
        rad=50;
    }
    CCircle(float x, float y, float rad, ULONG color):CShape(x, y, color)
    {
        this->rad=rad;
    }
    void Draw()
    {
        setColor(color);
        fillCircle(center.x,center.y,rad);
        setColor(BLACK);
        circle(center.x,center.y,rad);
    }
    void Scale(float r)
    {
        center.Scale(r);
        rad=rad*r;
    }
};
class CDonut: public CEllipse
{
    float ratio;
public:
    CDonut()
    {
        ratio = 0.5;
    }
    CDonut(float x, float y, float rx, float ry,
           float r, ULONG color):CEllipse(x, y, rx, ry, color)
    {
        this->ratio = r;
    }
    void Draw()
    {
        setColor(color);
        fillDonut(center.x, center.y, xRadius, yRadius, ratio);
        setColor(BLACK);
        donut(center.x, center.y, xRadius, yRadius, ratio);
    }
};
class CTriangle:public CShape
{
    float len;
    float wid;
public:
    CTriangle()
    {
        len = 300;
        wid = 200;
    }
    CTriangle(float x, float y, float len,
              float wid, ULONG color):CShape(x, y, color)
    {
        this->len = len;
        this->wid = wid;
    }
    void Draw()
    {
        CPoint2D v1(center.x-len/2, center.y-wid/2);
        CPoint2D v2(center.x+len/2, center.y-wid/2);
        CPoint2D v3(center.x, center.y+wid/2);
        setColor(color);
        fillTriangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);
        setColor(BLACK);
        triangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);
    }
    void Scale(float r)
    {
        center.Scale(r);
        len = len*r;
        wid = wid*r;
    }
};
class CPolygon:public CShape
{
protected:
    int num;//边数
    float rad;//外接圆半径
public:
    CPolygon()
    {
        num=5;
        rad=50;
    }
    CPolygon(float x, float y, float rad,
             int num, ULONG color):CShape(x, y, color)
    {
        this->num=num;
        this->rad=rad;
    }
    void Draw()
    {
        double px[num]= {0};
        double py[num]= {0};
        for(int i=0; i<num; i++)
        {
            px[i]=center.x+rad*cos(i*2*M_PI/num+M_PI/2);
            py[i]=center.y+rad*sin(i*2*M_PI/num+M_PI/2);
        }
        setColor(color);
        fillPolygon(num,px,py);
        setColor(BLACK);
        polygon(num,px,py);
    }
    void Scale(float r)
    {
        center.Scale(r);
        rad=rad*r;
    }
};
class CCurve:public CShape
{
    int num;//波浪数
public:
    CCurve()
    {
        num=10;
    }
    CCurve(float x, float y, int num,ULONG color):CShape(x, y, color)
    {
        this->num=num;
    }
    void Draw()
    {
        setLineWidth(3);
        for(int i=0; i<num; i++)
        {
            CPoint2D v1(center.x+(36*(i)), center.y-16);
            CPoint2D v2(center.x-18+(36*(i+1)), center.y+17);
            setColor(color);
            arc(v1.x,v1.y,20,60,120);
            arc(v2.x,v2.y,20,240,300);
        }
        setLineWidth(1);
    }
    void Scale(float r)
    {
        num=num+(int)(r*100-100);
        if(num<0)
            num=2;
    }
};

vector <CShape *> g_vShape;

void ReadShapes(const char *fileName)
{
    FILE *fp=NULL;
    char str[512];
    float x, y;
    float l, w, h, rx, ry, s,r;
    int clr,n;

    fp = fopen(fileName, "r");
    if (fp==NULL)
        {
            cout<<"未找到对应数据文件，请按0初始化"<<endl;
            return;
        }
    fscanf(fp, "%s", str);
    while (0!=strcmp(str, "Exit"))
    {
        if (0==strcmp(str, "Rectangle"))
        {
            cout << str << ":";
            fscanf(fp, "%f%f%f%f%x", &x, &y, &l, &w, &clr);
            cout << x << " " << y << " " << l << " " << w  << " " << clr << endl;
            g_vShape.push_back(new CRectangle(x, y, l, w, clr));
        }
        else if (0==strcmp(str, "Ellipse"))
        {
            cout << str << ":";
            fscanf(fp, "%f%f%f%f%x", &x, &y, &rx, &ry, &clr);
            cout << x << " " << y << " " << rx << " " << ry  << " " << clr << endl;
            g_vShape.push_back(new CEllipse(x, y, rx, ry, clr));
        }
        else if (0==strcmp(str, "Donut"))
        {
            cout << str << ":";
            fscanf(fp, "%f%f%f%f%f%x", &x, &y, &rx, &ry, &s, &clr);
            cout << x << " " << y << " " << rx << " " << ry << " " << s << " " << clr << endl;
            g_vShape.push_back(new CDonut(x, y, rx, ry, s, clr));
        }
        else if(0==strcmp(str, "Triangle"))
        {
            cout << str << ":";
            fscanf(fp, "%f%f%f%f%x", &x, &y, &l, &h, &clr);
            cout << x << " " << y << " " << l << " " << h  << " " << clr << endl;
            g_vShape.push_back(new CTriangle(x, y, l, h, clr));
        }
        else if(0==strcmp(str, "Curve"))
        {
            cout << str << ":";
            fscanf(fp, "%f%f%d%x", &x, &y, &n, &clr);
            cout << x << " " << y << " " << n  << " " << clr << endl;
            g_vShape.push_back(new CCurve(x,y,n,clr));
        }
        else if(0==strcmp(str, "Polygon"))
        {
            cout << str << ":";
            fscanf(fp, "%f%f%f%d%x", &x, &y,&r, &n, &clr);
            cout << x << " " << y << " " << r << " " << n << " " << clr << endl;
            g_vShape.push_back(new CPolygon(x,y,r,n,clr));
        }
        else if(0==strcmp(str, "Circle"))
        {
            cout << str << ":";
            fscanf(fp, "%f%f%f%x", &x, &y,&r, &clr);
            cout << x << " " << y << " " << r  << " " << clr << endl;
            g_vShape.push_back(new CCircle(x,y,r,clr));
        }
        fscanf(fp, "%s", str);
    }
}

void mousePress(int message, int x, int y)
{
    if(message==RIGHT_BUTTON_DOWN)
    {
        mpx=x;
        mpy=y;
        sb=99;
    }
}
void mouseMove(int x,int y)
{
    mmx=x;
    mmy=y;
}
void display()
{
    CCircle *cen;//center
    cen=new CCircle(sx,sy,5,RED);
    for(size_t i=0; i<g_vShape.size(); i++)
        g_vShape[i]->Draw();
    if(keyb==2)
    {
        CRectangle *p2;
        p2=new CRectangle(sx,sy,2*(sx-mmx),2*(sy-mmy),int(red)*16*16*16*16+int(green)*16*16+int(blue));
        p2->Draw();
        cen->Draw();
        if(sb==99)
        {
            delete p2;
            addShapes("shapedata.txt","Rectangle");
            sb=0;
            keyb=0;
        }
    }
    else if(keyb==1)
    {
        CTriangle *p1;
        p1=new CTriangle(sx,sy,(sx-mmx),(sy-mmy),int(red)*16*16*16*16+int(green)*16*16+int(blue));
        p1->Draw();
        cen->Draw();
        if(sb==99)
        {
            delete p1;
            addShapes("shapedata.txt","Triangle");
            sb=0;
            keyb=0;
        }
    }
    else if(keyb==3)
    {
        CEllipse *p3;
        p3=new CEllipse(sx,sy,(sx-mmx),(sy-mmy),int(red)*16*16*16*16+int(green)*16*16+int(blue));
        p3->Draw();
        cen->Draw();
        if(sb==99)
        {
            delete p3;
            addShapes("shapedata.txt","Ellipse");
            sb=0;
            keyb=0;
        }
    }
    else if(keyb==4)
    {
        CPolygon *p4;
        if(pn<=1)
            pn=2;
        p4=new CPolygon(sx,sy,sqrt((sx-mmx)*(sx-mmx)+(sy-mmy)*(sy-mmy)),pn,int(red)*16*16*16*16+int(green)*16*16+int(blue));
        p4->Draw();
        cen->Draw();
        if(sb==99)
        {
            delete p4;
            addShapes("shapedata.txt","Polygon");
            sb=0;
            keyb=0;
        }
    }
    else if(keyb==5)
    {
        CCurve *p5;
        if(pn<=2)
            pn=5;
        p5=new CCurve(sx,sy,pn,int(red)*16*16*16*16+int(green)*16*16+int(blue));
        p5->Draw();
        cen->Draw();
        if(sb==99)
        {
            delete p5;
            addShapes("shapedata.txt","Curve");
            sb=0;
            keyb=0;
        }
    }
    else if(keyb==6)
    {
        CCircle *p6;
        p6=new CCircle(sx,sy,sqrt((sx-mmx)*(sx-mmx)+(sy-mmy)*(sy-mmy)),int(red)*16*16*16*16+int(green)*16*16+int(blue));
        p6->Draw();
        cen->Draw();
        if(sb==99)
        {
            delete p6;
            addShapes("shapedata.txt","Circle");
            sb=0;
            keyb=0;
        }
    }

}
void addShapes(const char *filename,char *ch)
{
    FILE *fp=NULL;
    fp = fopen(filename, "r+");
    if (fp==NULL)
    {
        cout<<"无法保存时请按0初始化"<<endl;
        return;
    }
    if (0==strcmp("Rectangle", ch))
    {
        fseek(fp, -6L,2);//bug fixs
        fprintf(fp,"%s %.0f %.0f %.0f %.0f 0x%x\n", ch, sx, sy,2*(sx-mpx),2*(sy-mpy),int(red)*16*16*16*16+int(green)*16*16+int(blue));
        fseek(fp,0L,2) ;
        fprintf(fp,"Exit\n");
        rewind(fp) ;
        g_vShape.push_back(new CRectangle(sx,sy,2*(sx-mpx),2*(sy-mpy),int(red)*16*16*16*16+int(green)*16*16+int(blue)));
        reset();
    }
    else if (0==strcmp("Triangle", ch))
    {
        fseek(fp, -6L,2);
        fprintf(fp,"%s %.0f %.0f %.0f %.0f 0x%x\n", ch, sx, sy,(sx-mpx),(sy-mpy),int(red)*16*16*16*16+int(green)*16*16+int(blue));
        fseek(fp,0L,2) ;
        fprintf(fp,"Exit\n");
        rewind(fp) ;
        g_vShape.push_back(new CTriangle(sx,sy,(sx-mpx),(sy-mpy),int(red)*16*16*16*16+int(green)*16*16+int(blue)));
        reset();
    }
    else if (0==strcmp("Ellipse", ch))
    {
        fseek(fp, -6L,2);
        fprintf(fp,"%s %.0f %.0f %.0f %.0f 0x%x\n", ch, sx, sy,(sx-mpx),(sy-mpy),int(red)*16*16*16*16+int(green)*16*16+int(blue));
        fseek(fp,0L,2) ;
        fprintf(fp,"Exit\n");
        rewind(fp) ;
        g_vShape.push_back(new CEllipse(sx,sy,(sx-mmx),(sy-mmy),int(red)*16*16*16*16+int(green)*16*16+int(blue)));
        reset();
    }
    else if (0==strcmp("Polygon", ch))
    {
        fseek(fp, -6L,2);
        fprintf(fp,"%s %.0f %.0f %.0f %d 0x%x\n", ch, sx, sy,sqrt((sx-mmx)*(sx-mmx)+(sy-mmy)*(sy-mmy)),pn,int(red)*16*16*16*16+int(green)*16*16+int(blue));
        fseek(fp,0L,2) ;
        fprintf(fp,"Exit\n");
        rewind(fp) ;
        g_vShape.push_back(new CPolygon(sx,sy,sqrt((sx-mmx)*(sx-mmx)+(sy-mmy)*(sy-mmy)),pn,int(red)*16*16*16*16+int(green)*16*16+int(blue)));
        reset();
    }
    else if (0==strcmp("Curve", ch))
    {
        fseek(fp, -6L,2);
        fprintf(fp,"%s %.0f %.0f %d 0x%x\n", ch, sx, sy,pn,int(red)*16*16*16*16+int(green)*16*16+int(blue));
        fseek(fp,0L,2) ;
        fprintf(fp,"Exit\n");
        rewind(fp) ;
        g_vShape.push_back(new CCurve(sx,sy,pn,int(red)*16*16*16*16+int(green)*16*16+int(blue)));
        reset();
    }
    else if (0==strcmp("Circle",ch))
    {
        fseek(fp, -6L,2);
        fprintf(fp,"%s %.0f %.0f %.0f 0x%x\n", ch, sx, sy,sqrt((sx-mmx)*(sx-mmx)+(sy-mmy)*(sy-mmy)),int(red)*16*16*16*16+int(green)*16*16+int(blue));
        fseek(fp,0L,2) ;
        fprintf(fp,"Exit\n");
        rewind(fp) ;
        g_vShape.push_back(new CCircle(sx,sy,sqrt((sx-mmx)*(sx-mmx)+(sy-mmy)*(sy-mmy)),int(red)*16*16*16*16+int(green)*16*16+int(blue)));
        reset();
    }
}
void initdata(const char *filename)//初始化数据，同时如果没有文件则创建文件
{
    FILE *fp=NULL;
    fp= fopen (filename, "w") ;
    fputs ("Exit\n", fp) ;
    fclose (fp) ;
    cout<<"初始化完成"<<endl;
    reset();
}
void keyboard(unsigned char key)
{
    switch (key)
    {
    case 'w':
        for(size_t i=0; i<g_vShape.size(); i++)
            g_vShape[i]->Translate(0, 5);
        break;
    case 's':
        for(size_t i=0; i<g_vShape.size(); i++)
            g_vShape[i]->Translate(0, -5);
        break;
    case 'a':
        for(size_t i=0; i<g_vShape.size(); i++)
            g_vShape[i]->Translate(-5, 0);
        break;
    case 'd':
        for(size_t i=0; i<g_vShape.size(); i++)
            g_vShape[i]->Translate(5, 0);
        break;
    case 'z':
        for(size_t i=0; i<g_vShape.size(); i++)
            g_vShape[i]->Scale(1.05);
        break;
    case 'x':
        for(size_t i=0; i<g_vShape.size(); i++)
            g_vShape[i]->Scale(0.95);
        break;
    case '1':
        keyb=1;
        break;
    case '2':
        keyb=2;
        break;
    case '3':
        keyb=3;
        break;
    case '4':
        keyb=4;
        break;
    case '5':
        keyb=5;
        break;
    case '6':
        keyb=6;
        break;
    case '7':
        g_vShape.pop_back();
        break;
    case '8':
        saveScreenImage("yourdraw.bmp");
        break;
    case '0':
        initdata("shapedata.txt");
        break;
    case '9'://用于取消当前的图形
        keyb=9;
        break;
    case 'r':
        red+=20;
        break;
    case 'g':
        green+=20;
        break;
    case 'b':
        blue+=20;
        break;
    case 'n':
        pn+=1;
        break;
    case 'm':
        pn-=1;
        break;
    }
}
void specialKeyboard(int k)
{
    if(k==KEY_LEFT)
        sx=sx-5;
    else if(k==KEY_RIGHT)
        sx=sx+5;
    else if(k==KEY_UP)
        sy=sy+5;
    else if(k==KEY_DOWN)
        sy=sy-5;
}
int main()
{
    cout<<"若无法进入界面，请关闭程序、删除同目录下的shapedata.txt，并在程序启动后按“0”初始化\n"
    <<"若无法进入界面，请关闭程序、删除同目录下的shapedata.txt，并在程序启动后按“0”初始化\n"
    <<"若无法进入界面，请关闭程序、删除同目录下的shapedata.txt，并在程序启动后按“0”初始化"<<endl;
    cout<<"无法保存时请按“0”初始化\n"<<endl;
    cout<<"按键“1”三角形“2”矩形“3”椭圆“4”多边形，“5”曲线“6”圆“7”删除上一图形“8”保存为图片“rgb”调节颜色\n"
    <<"“9”取消图形“wsad”整体移动“zx”整体缩放“上下左右”移动当前图形“nm”调整多边形边数以及曲线长度，鼠标右键锁定当前图形\n"<<endl;
    ReadShapes("shapedata.txt");
    initGraph(display, keyboard,mousePress,mouseMove,specialKeyboard);
    for(size_t i=0; i<g_vShape.size(); i++)
        delete g_vShape[i];
    return 0;
}
