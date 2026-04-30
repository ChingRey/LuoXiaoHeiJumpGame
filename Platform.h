#pragma once
#include <graphics.h>

// 平台类
class Platform
{
public:
    // 构造函数：创建一个平台
    // x, y: 平台中心坐标
    // width, height: 平台宽高
    // speed: 下落速度
    Platform(double x, double y, double width, double height, double speed);

    // 更新平台位置（向下移动）
    void Update();

    // 绘制平台：深棕色矩形（后期替换为森林树干/石板纹理）
    void Draw();

    // 检查平台是否超出屏幕底部（应删除）
    bool IsOutOfScreen(int screenHeight);

    // 获取平台属性
    double GetX() const { return m_x; }
    double GetY() const { return m_y; }
    double GetWidth() const { return m_width; }
    double GetHeight() const { return m_height; }

private:
    double m_x, m_y;          // 平台中心坐标
    double m_width, m_height; // 平台宽高
    double m_speed;           // 下落速度
};
