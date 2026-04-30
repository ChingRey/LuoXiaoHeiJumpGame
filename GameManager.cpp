#include "GameManager.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <stdio.h>

GameManager::GameManager(int screenWidth, int screenHeight)
    : m_screenWidth(screenWidth), m_screenHeight(screenHeight),
      m_player(nullptr), m_gameState(GAME_OVER), m_score(0), m_highScore(0),
      m_gameTime(0), m_frameCount(0), m_platformSpeed(3),
      m_platformWidth(80), m_platformHeight(15),
      m_platformGapMin(40), m_platformGapMax(100),
      m_lastPlatformY(screenHeight - 50),
      m_lastPlatformBelowY(screenHeight + 150),
      m_pauseButtonX(screenWidth - 50), m_pauseButtonY(40), m_pauseButtonRadius(20),
      m_leftButtonX(50), m_leftButtonY(screenHeight - 50),
      m_rightButtonX(110), m_rightButtonY(screenHeight - 50),
      m_buttonRadius(25), m_backgroundScrollY(0),
      m_wasJumping(false), m_scrollOffset(0)
{
    srand(static_cast<unsigned>(time(nullptr)));
}

GameManager::~GameManager()
{
    if (m_player)
        delete m_player;

    for (auto p : m_platforms)
        delete p;
    m_platforms.clear();
}

void GameManager::Init()
{
    // Create game window
    initgraph(m_screenWidth, m_screenHeight);

    // Load high score
    LoadHighScore();

    // Camera setup
    m_fixedPlayerScreenY = m_screenHeight / 2;  // Player fixed at middle of screen
    m_cameraY = 0;  // Start at top of world

    // Create player at world position
    m_player = new Role(m_screenWidth / 2, 200);

    // Adjust camera to center player on screen
    m_cameraY = m_player->GetY() - m_fixedPlayerScreenY;

    // 在玩家位置生成一个固定宽度的初始板子（确保小猫在上面）
    Platform* startPlatform = new Platform(m_screenWidth / 2, m_player->GetY() + m_player->GetRadius() + 7,
                                           m_platformWidth, m_platformHeight, m_platformSpeed);
    m_platforms.push_back(startPlatform);

    // 生成初始平台（在玩家上方）
    m_lastPlatformY = m_player->GetY() + 80;
    for (int i = 0; i < 20; i++)
    {
        GeneratePlatform();
    }

    // 在玩家下方也生成一些平台（确保掉落时有板子可跳）
    double belowPlatformY = m_player->GetY() - 80;
    m_lastPlatformBelowY = belowPlatformY;
    for (int i = 0; i < 10; i++)
    {
        GeneratePlatformBelow(belowPlatformY);
    }

    // Start game
    m_gameState = PLAYING;
    m_score = 0;
    m_gameTime = 0;
}

void GameManager::Run()
{
    BeginBatchDraw();

    while (true)
    {
        // Handle input
        HandleInput();

        if (m_gameState == PLAYING)
        {
            Update();
        }

        Draw();

        // Control frame rate: 60fps
        FlushBatchDraw();
        Sleep(16);
    }

    EndBatchDraw();
}

void GameManager::HandleInput()
{
    // Check mouse input for pause button
    MOUSEMSG msg;
    while (MouseHit())
    {
        msg = GetMouseMsg();
        if (msg.uMsg == WM_LBUTTONDOWN)
        {
            // Check pause button
            double dx = msg.x - m_pauseButtonX;
            double dy = msg.y - m_pauseButtonY;
            if (sqrt(dx * dx + dy * dy) <= m_pauseButtonRadius)
            {
                if (m_gameState == PLAYING)
                    m_gameState = PAUSED;
                else if (m_gameState == PAUSED)
                    m_gameState = PLAYING;
            }

            // Check restart button when game over
            if (m_gameState == GAME_OVER && CheckRestartButtonClick(msg.x, msg.y))
            {
                // Reset game
                for (auto p : m_platforms)
                    delete p;
                m_platforms.clear();

                delete m_player;

                // Reinitialize
                m_fixedPlayerScreenY = m_screenHeight / 2;
                m_cameraY = 0;
                m_scrollOffset = 0;
                m_player = new Role(m_screenWidth / 2, 200);
                m_cameraY = m_player->GetY() - m_fixedPlayerScreenY;

                // 在玩家位置生成一个固定宽度的初始板子
                Platform* startPlatform = new Platform(m_screenWidth / 2, m_player->GetY() + m_player->GetRadius() + 7,
                                                       m_platformWidth, m_platformHeight, m_platformSpeed);
                m_platforms.push_back(startPlatform);

                m_lastPlatformY = m_player->GetY() + 80;
                for (int i = 0; i < 20; i++)
                {
                    GeneratePlatform();
                }

                // 下方平台初始化
                m_lastPlatformBelowY = m_player->GetY() - 80;
                for (int i = 0; i < 10; i++)
                {
                    GeneratePlatformBelow(m_lastPlatformBelowY);
                }

                m_gameState = PLAYING;
                m_score = 0;
                m_gameTime = 0;
                m_frameCount = 0;
            }
        }
    }

    // Check keyboard input
    if (m_gameState == PLAYING)
    {
        int moveDirection = 0;

        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            moveDirection = -1;
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            moveDirection = 1;

        m_player->Move(moveDirection);

        // 已删除空格键跳跃功能 - 小猫碰到板子自动跳跃
    }
}

void GameManager::Update()
{
    m_frameCount++;

    // Update game time every 60 frames
    if (m_frameCount % 60 == 0)
    {
        m_gameTime += 1.0;
        m_score += 1;
    }

    // 记录上一帧的y速度
    double prevVy = m_player->GetVelocityY();

    // Update player physics
    m_player->Update();

    // 检测玩家是否开始向上跳跃（从向下运动变为向上运动）
    bool isJumpingUp = (prevVy >= 0 && m_player->GetVelocityY() < 0);

    // 只有向上跳跃时才让背景向下滚动固定的距离
    if (isJumpingUp)
    {
        m_scrollOffset += 100;  // 每次跳跃滚动100像素
    }

    // Update camera position - 结合滚动偏移
    m_cameraY = m_player->GetY() - m_fixedPlayerScreenY + m_scrollOffset;

    // Update all platforms (they scroll down with platform speed)
    for (auto p : m_platforms)
    {
        p->Update();
    }

    // Generate new platforms at the top when needed
    if (m_lastPlatformY < m_cameraY - 100)
    {
        GeneratePlatform();
    }

    // 如果上方平台距离太远，在下方也生成平台（确保掉落时有板子）
    double playerBelowY = m_cameraY + m_screenHeight + 200;
    if (playerBelowY > m_lastPlatformBelowY - 100)
    {
        GeneratePlatformBelow(playerBelowY);
    }

    // Collision detection with platforms
    for (auto p : m_platforms)
    {
        m_player->CheckLandOnPlatform(p->GetY(), p->GetX(), p->GetWidth(), p->GetHeight());
    }

    // Remove platforms that are far below the camera
    auto it = m_platforms.begin();
    while (it != m_platforms.end())
    {
        if ((*it)->GetY() > m_cameraY + m_screenHeight + 100)
        {
            delete *it;
            it = m_platforms.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Game over condition: player falls too far below
    if (m_player->GetY() > m_cameraY + m_screenHeight + 50)
    {
        m_gameState = GAME_OVER;

        // Update high score
        if (m_score > m_highScore)
        {
            m_highScore = m_score;
            SaveHighScore();
        }
    }
}

void GameManager::Draw()
{
    cleardevice();

    // Draw background
    DrawBackground();

    // Draw all platforms with camera offset
    for (auto p : m_platforms)
    {
        double platformScreenY = p->GetY() - m_cameraY;

        // Only draw if visible on screen
        if (platformScreenY > -100 && platformScreenY < m_screenHeight + 100)
        {
            int screenX = (int)p->GetX();
            int screenY = (int)platformScreenY;
            int width = (int)p->GetWidth();
            int height = (int)p->GetHeight();

            // Draw platform
            setfillcolor(RGB(101, 67, 33));
            fillrectangle(
                screenX - width / 2,
                screenY - height / 2,
                screenX + width / 2,
                screenY + height / 2
            );

            setlinecolor(RGB(80, 50, 20));
            setlinestyle(PS_SOLID, 2);
            rectangle(
                screenX - width / 2,
                screenY - height / 2,
                screenX + width / 2,
                screenY + height / 2
            );
        }
    }

    // Draw player at fixed center position
    int playerScreenX = (int)m_player->GetX();
    int playerRadius = (int)m_player->GetRadius();

    // Player body
    setfillcolor(BLACK);
    fillcircle(playerScreenX, m_fixedPlayerScreenY, playerRadius);

    // Eyes
    setfillcolor(YELLOW);
    fillcircle(playerScreenX - 5, m_fixedPlayerScreenY - 3, 3);
    fillcircle(playerScreenX + 5, m_fixedPlayerScreenY - 3, 3);

    setfillcolor(BLACK);
    fillcircle(playerScreenX - 5, m_fixedPlayerScreenY - 3, 1);
    fillcircle(playerScreenX + 5, m_fixedPlayerScreenY - 3, 1);

    // Ears
    setfillcolor(YELLOW);
    fillcircle(playerScreenX - 8, m_fixedPlayerScreenY - playerRadius, 4);
    fillcircle(playerScreenX + 8, m_fixedPlayerScreenY - playerRadius, 4);

    // Draw UI
    DrawUI();

    // Draw pause screen if paused
    if (m_gameState == PAUSED)
    {
        DrawPauseScreen();
    }

    // Draw game over screen
    if (m_gameState == GAME_OVER)
    {
        DrawGameOverScreen();
    }
}

void GameManager::GeneratePlatform()
{
    // 获取难度系数
    double difficultyMultiplier = GetDifficultyMultiplier();

    // 固定行间距（上下板子距离相等）
    double fixedGap = 120;  // 固定间距

    // 生成一排平台（每排1-3个，数量随机）
    int platformsInRow = 1 + rand() % 3;  // 随机1-3个板子

    // 一排板子的总宽度范围
    double minX = m_platformWidth / 2 + 20;
    double maxX = m_screenWidth - m_platformWidth / 2 - 20;

    // 随机生成这一排的起始位置
    double startX = minX + rand() % (int)(maxX - minX);

    // 为每个平台计算位置（确保不重叠）
    for (int i = 0; i < platformsInRow; i++)
    {
        // 随机位置（可能重叠）
        double newX = minX + rand() % (int)(maxX - minX);

        // 根据难度调整平台宽度
        double adjustedWidth = m_platformWidth * (1.0 - difficultyMultiplier * 0.15);
        if (adjustedWidth < 50.0)
            adjustedWidth = 50.0;

        // 创建平台在世界坐标
        Platform* platform = new Platform(newX, m_lastPlatformY, adjustedWidth, m_platformHeight, m_platformSpeed);
        m_platforms.push_back(platform);
    }

    // 更新下一排的Y坐标（固定间距）
    m_lastPlatformY -= fixedGap;
}

// 生成下方的平台（玩家掉落时使用）
void GameManager::GeneratePlatformBelow(double startY)
{
    // 获取难度系数
    double difficultyMultiplier = GetDifficultyMultiplier();

    // 固定间距
    double fixedGap = 120;

    // 生成一排平台（每排1-3个，数量随机）
    int platformsInRow = 1 + rand() % 3;

    double minX = m_platformWidth / 2 + 20;
    double maxX = m_screenWidth - m_platformWidth / 2 - 20;

    for (int i = 0; i < platformsInRow; i++)
    {
        double newX = minX + rand() % (int)(maxX - minX);

        double adjustedWidth = m_platformWidth * (1.0 - difficultyMultiplier * 0.15);
        if (adjustedWidth < 50.0)
            adjustedWidth = 50.0;

        Platform* platform = new Platform(newX, startY, adjustedWidth, m_platformHeight, m_platformSpeed);
        m_platforms.push_back(platform);
    }

    // 更新下方平台生成位置
    m_lastPlatformBelowY = startY + fixedGap;
}

double GameManager::GetDifficultyMultiplier()
{
    // Calculate difficulty multiplier based on score (0.0 - 1.0)
    // Difficulty increases more slowly now
    double multiplier = m_score / 1000.0;
    if (multiplier > 1.0)
        multiplier = 1.0;
    return multiplier;
}

void GameManager::DrawUI()
{
    // Top-left score panel
    int uiPanelX = 20;
    int uiPanelY = 20;

    // Light yellow background
    setfillcolor(RGB(255, 250, 205));
    fillrectangle(uiPanelX, uiPanelY, uiPanelX + 200, uiPanelY + 80);

    // Border
    setlinecolor(RGB(218, 185, 105));
    setlinestyle(PS_SOLID, 2);
    rectangle(uiPanelX, uiPanelY, uiPanelX + 200, uiPanelY + 80);

    // Text
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("SimSun"));

    wchar_t scoreText[50];
    swprintf_s(scoreText, 50, L"Score: %d", m_score);
    outtextxy(uiPanelX + 10, uiPanelY + 10, scoreText);

    wchar_t highScoreText[50];
    swprintf_s(highScoreText, 50, L"High: %d", m_highScore);
    outtextxy(uiPanelX + 10, uiPanelY + 40, highScoreText);

    // Top-right time panel
    int timePanelX = m_screenWidth - 200;
    int timePanelY = 20;

    // Light green background
    setfillcolor(RGB(200, 230, 201));
    fillrectangle(timePanelX, timePanelY, timePanelX + 180, timePanelY + 60);

    // Border
    setlinecolor(RGB(129, 199, 132));
    setlinestyle(PS_SOLID, 2);
    rectangle(timePanelX, timePanelY, timePanelX + 180, timePanelY + 60);

    // Text
    wchar_t timeText[50];
    swprintf_s(timeText, 50, L"Time: %ds", (int)m_gameTime);
    outtextxy(timePanelX + 10, timePanelY + 15, timeText);

    // Pause button (top-right)
    setfillcolor(RGB(200, 150, 100));
    fillcircle(m_screenWidth - 50, 40, m_pauseButtonRadius);

    // Pause symbol (two vertical lines)
    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 3);
    line(m_screenWidth - 56, 32, m_screenWidth - 56, 48);
    line(m_screenWidth - 44, 32, m_screenWidth - 44, 48);
}

void GameManager::DrawPauseScreen()
{
    // Semi-transparent black overlay
    setfillcolor(RGB(0, 0, 0));
    fillrectangle(0, 0, m_screenWidth, m_screenHeight);

    // Pause text
    setbkmode(TRANSPARENT);
    settextcolor(RGB(255, 255, 255));
    settextstyle(48, 0, _T("SimSun"));

    wchar_t pauseText[] = L"PAUSED";
    int textWidth = textwidth(pauseText);
    outtextxy((m_screenWidth - textWidth) / 2, m_screenHeight / 2 - 40, pauseText);

    settextstyle(24, 0, _T("SimSun"));
    wchar_t resumeText[] = L"Click pause button to continue";
    textWidth = textwidth(resumeText);
    outtextxy((m_screenWidth - textWidth) / 2, m_screenHeight / 2 + 40, resumeText);
}

void GameManager::DrawGameOverScreen()
{
    // Semi-transparent black overlay
    setfillcolor(RGB(0, 0, 0));
    fillrectangle(0, 0, m_screenWidth, m_screenHeight);

    // Game over text
    setbkmode(TRANSPARENT);
    settextcolor(RGB(255, 200, 100));
    settextstyle(40, 0, _T("SimSun"));

    wchar_t gameOverText[] = L"GAME OVER";
    int textWidth = textwidth(gameOverText);
    outtextxy((m_screenWidth - textWidth) / 2, m_screenHeight / 2 - 60, gameOverText);

    settextstyle(24, 0, _T("SimSun"));
    wchar_t subText[] = L"Keep trying!";
    textWidth = textwidth(subText);
    outtextxy((m_screenWidth - textWidth) / 2, m_screenHeight / 2 - 10, subText);

    // Show score
    settextstyle(28, 0, _T("SimSun"));
    wchar_t scoreText[50];
    swprintf_s(scoreText, 50, L"Score: %d", m_score);
    textWidth = textwidth(scoreText);
    outtextxy((m_screenWidth - textWidth) / 2, m_screenHeight / 2 + 40, scoreText);

    wchar_t highScoreText[50];
    swprintf_s(highScoreText, 50, L"High Score: %d", m_highScore);
    textWidth = textwidth(highScoreText);
    outtextxy((m_screenWidth - textWidth) / 2, m_screenHeight / 2 + 80, highScoreText);

    // Restart button
    int buttonX = m_screenWidth / 2;
    int buttonY = m_screenHeight / 2 + 150;
    int buttonWidth = 150;
    int buttonHeight = 50;

    setfillcolor(RGB(100, 200, 100));
    fillrectangle(buttonX - buttonWidth / 2, buttonY - buttonHeight / 2,
                  buttonX + buttonWidth / 2, buttonY + buttonHeight / 2);

    setlinecolor(RGB(50, 150, 50));
    setlinestyle(PS_SOLID, 3);
    rectangle(buttonX - buttonWidth / 2, buttonY - buttonHeight / 2,
             buttonX + buttonWidth / 2, buttonY + buttonHeight / 2);

    settextcolor(WHITE);
    settextstyle(24, 0, _T("SimSun"));
    wchar_t restartText[] = L"RESTART";
    textWidth = textwidth(restartText);
    outtextxy(buttonX - textWidth / 2, buttonY - 12, restartText);
}

void GameManager::DrawBackground()
{
    // Get difficulty multiplier to adjust background color
    double difficultyMultiplier = GetDifficultyMultiplier();

    // Background color: light green to darker green based on difficulty
    int greenBase = 180 - (int)(30 * difficultyMultiplier);

    setfillcolor(RGB(150, greenBase, 120));
    fillrectangle(0, 0, m_screenWidth, m_screenHeight);

    // Draw simple trees and mist decorations
    // Left side trees (background)
    setfillcolor(RGB(100, 130, 80));
    for (int i = 0; i < 3; i++)
    {
        fillcircle(50, 200 + i * 150, 40);
        fillrectangle(40, 200 + i * 150, 60, 300 + i * 150);
    }

    // Right side trees (background)
    for (int i = 0; i < 3; i++)
    {
        fillcircle(m_screenWidth - 50, 250 + i * 150, 40);
        fillrectangle(m_screenWidth - 60, 250 + i * 150, m_screenWidth - 40, 350 + i * 150);
    }

    // Draw mist effect (white circles)
    setfillcolor(RGB(220, 220, 220));
    for (int i = 0; i < 4; i++)
    {
        fillcircle(m_screenWidth / 4 + i * 150, 100 + (i * 50) % 100, 30);
    }
}

bool GameManager::CheckPauseButtonClick(int x, int y)
{
    double dx = x - m_pauseButtonX;
    double dy = y - m_pauseButtonY;
    return sqrt(dx * dx + dy * dy) <= m_pauseButtonRadius;
}

bool GameManager::CheckRestartButtonClick(int x, int y)
{
    int buttonX = m_screenWidth / 2;
    int buttonY = m_screenHeight / 2 + 150;
    int buttonWidth = 150;
    int buttonHeight = 50;

    return x >= buttonX - buttonWidth / 2 && x <= buttonX + buttonWidth / 2 &&
           y >= buttonY - buttonHeight / 2 && y <= buttonY + buttonHeight / 2;
}

void GameManager::LoadHighScore()
{
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, "highscore.txt", "r");
    if (err == 0 && file != nullptr)
    {
        fscanf_s(file, "%d", &m_highScore);
        fclose(file);
    }
    else
    {
        m_highScore = 0;
    }
}

void GameManager::SaveHighScore()
{
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, "highscore.txt", "w");
    if (err == 0 && file != nullptr)
    {
        fprintf(file, "%d", m_highScore);
        fclose(file);
    }
}
