#include "Platform.h"

Platform::Platform(double x, double y, double width, double height, double speed)
    : m_x(x), m_y(y), m_width(width), m_height(height), m_speed(speed)
{
}

void Platform::Update()
{
    // 平台持续向下移动
    m_y += m_speed;
}

void Platform::Draw()
{
    // ========= 下方替换为森林树干/石板纹理 =========
    // 绘制深棕色矩形平台
    setfillcolor(RGB(101, 67, 33));  // 深棕色
    fillrectangle(
        m_x - m_width / 2,
        m_y - m_height / 2,
        m_x + m_width / 2,
        m_y + m_height / 2
    );

    // 绘制边框增加质感
    setlinecolor(RGB(80, 50, 20));  // 更深的棕色
    setlinestyle(PS_SOLID, 2);
    rectangle(
        m_x - m_width / 2,
        m_y - m_height / 2,
        m_x + m_width / 2,
        m_y + m_height / 2
    );
    // ============================================
}

bool Platform::IsOutOfScreen(int screenHeight)
{
    return m_y - m_height / 2 > screenHeight;
}
