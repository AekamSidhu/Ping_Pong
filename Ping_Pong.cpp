#include <stdio.h>
#include <raylib.h>

struct racket {
    Vector2 position;
    Vector2 speed;
    Texture2D texture;
};

struct ball {
    Vector2 position;
    Vector2 speed;
    float radius;
    Texture2D texture;
};

float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int main() {
    InitWindow(1600, 1000, "ping pong");

    int fps = GetMonitorRefreshRate(GetCurrentMonitor());
    SetTargetFPS(fps);

    const char* bgPath = "items\\bg1.png";
    Image bgImage = LoadImage(bgPath);
    Texture2D bgTexture = LoadTextureFromImage(bgImage);
    UnloadImage(bgImage);

    const char* racketPath1 = "items\\racket1.png";
    Image racketImage1 = LoadImage(racketPath1);
    Texture2D racketTexture1 = LoadTextureFromImage(racketImage1);
    UnloadImage(racketImage1);

    const char* racketPath2 = "items\\racket2.png";
    Image racketImage2 = LoadImage(racketPath2);
    Texture2D racketTexture2 = LoadTextureFromImage(racketImage2);
    UnloadImage(racketImage2);

    const char* ballPath = "items\\ball.png";
    Image ballImage = LoadImage(ballPath);
    Texture2D ballTexture = LoadTextureFromImage(ballImage);
    UnloadImage(ballImage);

    Font font = LoadFont("items\\font2.ttf");

    struct racket player1
    {
        { 50.0f, 400.0f }, { 0.0f, 5.0f }, racketTexture1
    };


    struct racket player2
    {
        { 1500.0f, 400.0f }, { 0.0f, 5.0f }, racketTexture2
    };


    struct ball ball1
    {
        { 800.0f, 500.0f }, { 6.0f, 0.0f }, 10.0f, ballTexture
    };

    int scorePlayer1 = 0;
    int scorePlayer2 = 0;
    bool gamePaused = false;



    while (!WindowShouldClose()) {
        if (gamePaused) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                gamePaused = false;
            }
        }
        else {
            if (IsKeyDown(KEY_W)) player1.position.y -= player1.speed.y;
            if (IsKeyDown(KEY_S)) player1.position.y += player1.speed.y;
            if (IsKeyDown(KEY_UP)) player2.position.y -= player2.speed.y;
            if (IsKeyDown(KEY_DOWN)) player2.position.y += player2.speed.y;

            player1.position.y = Clamp(player1.position.y, 0.0f, 1000.0f - player1.texture.height);
            player2.position.y = Clamp(player2.position.y, 0.0f, 1000.0f - player2.texture.height);

            ball1.position.x += ball1.speed.x;
            ball1.position.y += ball1.speed.y;

            if (ball1.position.y - ball1.radius <= 0) {
                ball1.speed.y *= -1;
                ball1.position.y = ball1.radius;
            }
            else if (ball1.position.y + ball1.radius >= 1000) {
                ball1.speed.y *= -1;
                ball1.position.y = 1000 - ball1.radius;
            }

            Rectangle player1Rect = { player1.position.x, player1.position.y, (float)player1.texture.width, (float)player1.texture.height };
            Rectangle player2Rect = { player2.position.x, player2.position.y, (float)player2.texture.width, (float)player2.texture.height };
            Rectangle ballRect = { ball1.position.x - ball1.radius, ball1.position.y - ball1.radius, ball1.radius * 2.0f, ball1.radius * 2.0f };

            if (CheckCollisionRecs(ballRect, player1Rect)) {
                ball1.speed.x *= -1;
                ball1.position.x = player1.position.x + player1.texture.width + ball1.radius;
                ball1.speed.y = (ball1.position.y + ball1.radius - (player1Rect.y + player1Rect.height / 2)) / 5.0f;
            }
            else if (CheckCollisionRecs(ballRect, player2Rect)) {
                ball1.speed.x *= -1;
                ball1.position.x = player2.position.x - ball1.radius;
                ball1.speed.y = (ball1.position.y + ball1.radius - (player2Rect.y + player2Rect.height / 2)) / 5.0f;
            }


            if (ball1.position.x - ball1.radius <= 0) {
                scorePlayer2++;
                ball1.position = { 800.0f, 500.0f };
                ball1.speed = { 5.0f, 5.0f };
                gamePaused = true;
            }
            else if (ball1.position.x + ball1.radius >= 1600) {
                scorePlayer1++;
                ball1.position = { 800.0f, 500.0f };
                ball1.speed = { -5.0f, 5.0f };
                gamePaused = true;
            }
        }


        BeginDrawing();

        ClearBackground(WHITE);

        DrawTexture(bgTexture, 0, 0, WHITE);
        DrawTexture(player1.texture, (int)player1.position.x, (int)player1.position.y, WHITE);
        DrawTexture(player2.texture, (int)player2.position.x, (int)player2.position.y, WHITE);
        DrawTexture(ball1.texture, (int)(ball1.position.x - ball1.radius), (int)(ball1.position.y - ball1.radius), WHITE);

        char scoreText1[10];
        char scoreText2[10];
        sprintf_s(scoreText1, "%d", scorePlayer1);
        sprintf_s(scoreText2, "%d", scorePlayer2);
        DrawText(scoreText1, 100, 60, 100, BLACK);
        DrawText(scoreText2, 1500, 60, 100, BLACK);

        if (gamePaused) {
            DrawText("Press ENTER/SPACE to continue", 500, 500, 40, BLACK);
        }

        DrawTextEx(font, "PING PONG", { 420,30 }, 130, 5, BLACK);

        EndDrawing();
    }

    UnloadTexture(bgTexture);
    UnloadTexture(player1.texture);
    UnloadTexture(player2.texture);
    UnloadTexture(ball1.texture);
    UnloadFont(font);

    CloseWindow();

    return 0;
}
