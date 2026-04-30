#pragma once
#include <graphics.h>

// 罗小黑角色类
class Role
{
public:
    // 构造函数：初始化角色位置和属性
    Role(double x, double y);

    // 角色物理更新：受重力影响、速度更新
    void Update();

    // 移动输入：左右移动
    // direction: -1 左移，1 右移，0 静止
    void Move(int direction);

    // 跳跃：检查是否可以跳跃并执行
    // 在地面上可跳2次，空中可跳1次
    void Jump();

    // 碰撞检测：落地平台时重置状态
    // 返回true表示与平台发生碰撞（从上方踩到）
    bool CheckLandOnPlatform(double platformY, double platformX, double platformWidth, double platformHeight);

    // 检查是否掉出屏幕底部（游戏结束条件）
    bool IsOutOfScreen(int screenHeight);

    // 绘制角色：纯黑色圆形+简化五官（后期替换为罗小黑立绘）
    void Draw();

    // 获取角色位置和速度（用于游戏逻辑判断）
    double GetX() const { return m_x; }
    double GetY() const { return m_y; }
    double GetVelocityY() const { return m_vy; }
    double GetRadius() const { return m_radius; }

    // 重置角色状态（游戏开始/重启时调用）
    void Reset(double x, double y);

    // 设置是否在地面上（初始化时使用）
    void SetOnGround(bool onGround) { m_isOnGround = onGround; }

    // 重置跳跃次数（初始化时使用）
    void ResetJumpCount() { m_jumpCount = 2; }

private:
    double m_x, m_y;              // 角色中心坐标
    double m_vx, m_vy;            // 水平和竖直速度
    double m_radius;              // 角色半径
    double m_moveAccel;           // 移动加速度
    double m_moveDecel;           // 移动减速度
    double m_maxMoveSpeed;        // 最大水平移动速度
    double m_jumpPower;           // 跳跃力度
    double m_gravity;             // 重力加速度

    int m_jumpCount;              // 当前可跳跃次数（0=不可跳，1=可跳1次，2=可跳2次）
    bool m_isOnGround;            // 是否在地面/平台上
    double m_targetVx;            // 目标水平速度（用于平滑加速/减速）
};
