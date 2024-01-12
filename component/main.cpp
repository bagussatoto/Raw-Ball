#include <sl.h>
#include <random>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

// Define Ball and Sball structures
struct Ball {
    double posX, posY, velX, velY, radius;
};

struct Sball {
    double posX, posY, radius;
};

// Define Plat structure
struct Plat {
    double posX, posY;
};

int main() {
    slWindow(600, 900, "Ball", false);

    // Load assets
    int imgSky = slLoadTexture("sky.png");
    int imgDuri1 = slLoadTexture("duri1.png");
    int imgDuri2 = slLoadTexture("duri2.png");
    int imgPlat = slLoadTexture("plat.png");
    int imgAwan1 = slLoadTexture("awan1.png");
    int imgAwan2 = slLoadTexture("awan2.png");
    int imgAwan3 = slLoadTexture("awan3.png");
    int imgAwan4 = slLoadTexture("awan4.png");
    int imgAwan5 = slLoadTexture("awan5.png");
    int imgBall = slLoadTexture("ball.png");
    int imgSball = slLoadTexture("spikeball.png");
    int bgm = slLoadWAV("bgm.wav");
    int pop = slLoadWAV("pop.wav");
    int font1 = slLoadFont("gothicb.ttf");
    int font2 = slLoadFont("gothici.ttf");

    vector<int> explode = {
        slLoadTexture("ball1.png"),
        slLoadTexture("ball2.png"),
        slLoadTexture("ball3.png"),
        slLoadTexture("ball4.png"),
        slLoadTexture("ball5.png"),
        slLoadTexture("ball6.png"),
        slLoadTexture("ball7.png"),
        slLoadTexture("ball8.png"),
    };

    // PLay Music 
    void ingame();
    {
        slSoundLoop(bgm);
    };

    // Gravity
    const double gravity = -0.01;
    random_device rd;

    int wan = 600;
    double scrollY = 0.0;
    double ballX = 300;

    int score = 0;
    int lives = 3;

    int currentExplodeFrame = 0;
    double explodeFrameDuration = 0.8;
    double explodeFrameTimer = 0.0;

    // Randomize platforms
    Plat plat{ 300, 0 };
    vector<Plat> platt;
    const int numPlatforms = 12;
    const double platformSpacing = 150.0;

    for (int i = 0; i < numPlatforms; ++i) {
        double x = 250 - i * platformSpacing;
        double y = (rd() % 10 + 1) * platformSpacing;
        platt.push_back({ x, y });
    }
    Plat currentPlat{ 0, 0 };

    // Randomize spike balls
    Sball sball{ 300, -10, 25 };
    vector<Sball> sballs;

    for (int i = 0; i < 3; ++i) {
        double x = 100 - i / 70;
        double y = (rd() % 5 + 1) * 70;
        sballs.push_back({ x, y, 15 });
    }

    // Initialize the ball
    Ball ball{ 300, 720, 0, 0, 40 };

    bool gameOver = false;

    // Main game loop
    while (!slShouldClose()) {
        score += 1;

        if (slGetKey(SL_KEY_LEFT)) {
            ball.posX -= 3.0;
        }

        if (slGetKey(SL_KEY_RIGHT)) {
            ball.posX += 3.0;
        }

        //PLatform and Ball touch logic
        currentPlat = { 0, 0 };
        for (const Plat& plat : platt) {
            if (ball.posY + 40 >= plat.posY && ball.posY <= plat.posY + 39 && plat.posX - 90 < ball.posX && ball.posX < plat.posX + 90) {
                currentPlat = plat;
                break;
            }
        }

        if (currentPlat.posY != 0) {
            if (ball.posY <= currentPlat.posY + 39 && ball.posY >= currentPlat.posY) {
                ball.posY = currentPlat.posY + 39;
                ball.velY = 0;
            }
        }

        if (ball.posY == currentPlat.posY + 39 && currentPlat.posX - 90 < ball.posX && ball.posX < currentPlat.posX + 90) {
            ball.velY = -0.5;
        }

        // Ball hit the top and bottom spike
        if (ball.posY <= 55 || ball.posY >= 840) {
            ball.posY = 720;
            ball.velY = 0;
            lives--;
            slSoundPlay(pop);

            currentExplodeFrame = 0;
            explodeFrameTimer = explodeFrameDuration;
        }

        // Spike Ball Collision
        for (const Sball& sball : sballs) {
            double distance = sqrt(pow((ball.posX - sball.posX), 2) + pow((ball.posY - sball.posY), 2));
            double combinedRadius = ball.radius + sball.radius;

            if (distance <= combinedRadius) {
                ball.posY = 720;
                ball.velY = 0;
                ball.posX = 300;
                lives--;

                slSoundPlay(pop);

                currentExplodeFrame = 0;
                explodeFrameTimer = explodeFrameDuration;
            }
        }

        // Sky background
        slSetSpriteScroll(0.0, scrollY);
        slSprite(imgSky, 300, 450, 600, 900);

        scrollY += 0.0005;
        if (scrollY >= 1.0)
            scrollY = 0.0;

        slSetSpriteScroll(0.0, 0.0);

        // Spike ball
        if (sball.posY = 910) {
            for (Sball& m : sballs) {
                m.posY += 2.5;
                if (m.posY >= 910) {
                    m.posY = 0;
                    m.posX = (rd() % 3 + 1) * 250;
                }
            }
            for (const Sball& sball : sballs)
                slSprite(imgSball, sball.posX, sball.posY, 50, 50);
        }

        //Platform
        if (plat.posY = 890) {
            for (Plat& m : platt) {
                m.posY += 1;
                if (m.posY >= 900) {
                    m.posY = 0;
                    m.posX = (rd() % 6 + 1) * 125;
                }
            }
            for (const Plat& plat : platt)
                slSprite(imgPlat, plat.posX, plat.posY, 150, 30);
        }

        // Ball reset position
        if (ball.posY <= 0) {
            ball.posY = 720;
            ball.velY = 0;
        }

        // Ball teleporting between right and left screen
        if (ball.posX <= -10)
            ball.posX = 605;

        if (ball.posX >= 610)
            ball.posX = -5;

        // Ball with gravity
        ball.velY += gravity;
        ball.posY += ball.velY;

        slSprite(imgBall, ball.posX, ball.posY, 40, 40);

        // Spike
        slSprite(imgDuri1, 300, 860, 600, 85);
        slSprite(imgDuri2, 300, 39, 600, 85);

        // Cloud
        slSprite(imgAwan1, wan, 15, 290, 120);
        --wan;
        if (wan <= -60) {
            wan = 680;
        }

        // Score and lives text
        slSetForeColor(1,1,1,1);
        slSetFont(font1, 40);
        slSetTextAlign(SL_ALIGN_LEFT);
        slText(10, 870, ("Score: " + to_string(score)).c_str());
        slText(400, 870, ("Lives: " + to_string(lives)).c_str());

        // Animating ball explosion
        if (currentExplodeFrame < explode.size()) {
            explodeFrameTimer -= slGetDeltaTime();

            if (explodeFrameTimer <= 0.0) {
                slSprite(explode[currentExplodeFrame], ball.posX, ball.posY, 50, 50);
                currentExplodeFrame++;
                explodeFrameTimer = explodeFrameDuration;
            }
        }

        if (lives == 0) {
            gameOver = true;
        }

        // Game Over Status
        if (gameOver) {
            slSetForeColor(0, 5, 3, 0.5);
            slSetFont(font1, 50);
            slSetTextAlign(SL_ALIGN_CENTER);
            slText(300, 470, ("Game Over\nScore: " + to_string(score)).c_str());
            slSetFont(font2, 35);
            slText(300, 360, "Press 'C' to Continue");
            slText(300, 330, "or Press 'Q' to Quit");
            slSetForeColor(1, 0, 0, 1);
            score--;
            slSoundPause(bgm);
            


            if (lives-=1)
                lives++;

            if (slGetKey('C')) {
                // Reset game variables and continue
                lives = 3;
                score = 0;
                ball.posY = 720;
                ball.velY = 0;
                gameOver = false;
            }

            if (slGetKey('Q')) {
                slClose(); // Quit the game
                break;
            }
        }


        slRender();
    }

    slClose();

    system("pause");

    return 0;
}
