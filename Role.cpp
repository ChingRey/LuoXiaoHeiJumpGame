#include "Role.h"
#include <cmath>
#include <algorithm>

Role::Role(double x, double y)
    : m_x(x), m_y(y), m_vx(0), m_vy(0), m_radius(15),
      m_moveAccel(0.8), m_moveDecel(0.6), m_maxMoveSpeed(8),
      m_jumpPower(-20), m_gravity(0.5),
      m_jumpCount(2), m_isOnGround(true), m_targetVx(0)
{
}

void Role::Update()
{
    // 应用水平速度平滑过渡
    m_vx += (m_targetVx - m_vx) * 0.15;

    // 更新水平位置
    m_x += m_vx;

    // 屏幕环绕：左右移出屏幕时从另一侧出现（保留此功能）
    if (m_x < -m_radius)
        m_x = getwidth() + m_radius;
    if (m_x > getwidth() + m_radius)
        m_x = -m_radius;

    // 更新竖直速度（重力加速）
    m_vy += m_gravity;

    // 速度限制（防止下落过快）
    if (m_vy > 20)
        m_vy = 20;

    // 更新竖直位置（在世界坐标中）
    m_y += m_vy;

    // 如果不在地面上，且未触发空中跳跃，设置跳跃次数为1
    if (!m_isOnGround && m_jumpCount == 2)
    {
        m_jumpCount = 1;
    }
}

void Role::Move(int direction)
{
    if (direction == 0)
    {
        // 静止：逐渐减速
        m_targetVx *= m_moveDecel;
        if (std::abs(m_targetVx) < 0.1)
            m_targetVx = 0;
    }
    else if (direction == -1)
    {
        // 左移
        m_targetVx = -m_maxMoveSpeed;
    }
    else if (direction == 1)
    {
        // 右移
        m_targetVx = m_maxMoveSpeed;
    }
}

void Role::Jump()
{
    if (m_jumpCount > 0)
    {
        m_vy = m_jumpPower;
        m_jumpCount--;
        m_isOnGround = false;
    }
}

bool Role::CheckLandOnPlatform(double platformY, double platformX, double platformWidth, double platformHeight)
{
    // 碰撞检测：只有从上方踩到平台顶部才算落地
    // 检查水平范围
    if (m_x < platformX - platformWidth / 2 || m_x > platformX + platformWidth / 2)
        return false;

    // 检查竖直范围：角色底部在平台顶部附近
    if (m_y + m_radius >= platformY - platformHeight / 2 &&
        m_y + m_radius <= platformY - platformHeight / 2 + 10 &&
        m_vy > 0)  // 确保是向下运动时触发
    {
        // 自动跳跃：碰到板子就向上跳
        m_vy = m_jumpPower;
        m_jumpCount = 1;  // 保持一次跳跃次数
        m_isOnGround = false;
        return true;
    }

    return false;
}

bool Role::IsOutOfScreen(int screenHeight)
{
    return m_y - m_radius > screenHeight;
}

void Role::Draw()
{
    // Drawing is now handled by GameManager with camera offset
    // This method is kept for compatibility but not used
}

void Role::Reset(double x, double y)
{
    m_x = x;
    m_y = y;
    m_vx = 0;
    m_vy = 0;
    m_targetVx = 0;
    m_jumpCount = 2;
    m_isOnGround = true;
}
