#include <raylib/raylib.h>
#include <stdio.h>
#include <iostream>


// In prima parte am creat structurile care reprezinta componentele principale ale jocului, in cazul nostru paletele si mingea
struct Ball
{
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw()
	{
		DrawCircle((int)x, (int)y, radius, WHITE);
	}
};

struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}

	void Draw()
	{
		DrawRectangleRec(GetRect(), WHITE);
	}
};




int main()
{

	// am inceput in int main() prin declararea tuturor variabilelor care vor afecta gameplayul
	InitWindow(800, 600, "Pong");
	SetWindowState(FLAG_VSYNC_HINT);

	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 5;
	ball.speedX = 300;
	ball.speedY = 300;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 200;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;

	const char* winnerText = nullptr;
	bool gameStart = false;
	bool isInMenu = true;
	int scoreLeft = 0;
	int scoreRight = 0;
	int difficulty = 1;
	int screen = 0;

	while (!WindowShouldClose())
	{
	// Menu ------------------------------	
		// Aici am folosit functia switch pentru a crea un menu foarte simplu
		//case 0 va fi title screenul de unde poate fi pornit jocul sau se poate accesa case 1 
		//case 1 va contine o optiune de schimbare a unei variabile pentru dificultate care modifica ulterior viteza oponentului
		//meniul va functiona doar cand bool isInMenu e adevarata astfel incat sa ne putem intoarce in menu in timpul jocului
		if (isInMenu)
		{
			switch (screen)
			{
			case 0:
				DrawText("PONG", 300, 50, 70, GREEN);
				DrawText("press ENTER to begin", 120, 420, 50, GREEN);
				DrawText("press O to open settings", 90, 500, 50, GREEN);
				
				if (IsKeyPressed(KEY_ENTER)) isInMenu = false;
				if (IsKeyPressed(KEY_O)) screen = 1;				
				break;

			case 1:
				DrawText("Select difficulty", 200, 100, 50, GREEN);
				DrawText("(J,K,L)", 330, 160, 40, GREEN);
				if (IsKeyPressed(KEY_J)) difficulty = 1;
				if (IsKeyPressed(KEY_K)) difficulty = 2;
				if (IsKeyPressed(KEY_L)) difficulty = 3;
				if (difficulty == 1) DrawText("EASY", 330, 420, 50, DARKGREEN);
				if (difficulty == 2) DrawText("MEDIUM", 300, 420, 50, YELLOW);
				if (difficulty == 3) DrawText("HARD", 330, 420, 50, RED);
				DrawText("Press O to go back", 300, 500, 20, GREEN);
				if (IsKeyPressed(KEY_O)) screen = 0;

			}
	}
	// Gameplay -------------------------------	
		// in sectiunea aceasta se intampla totul ce tine de joc in sine
		if (!isInMenu)
		{  
    // UI --------------------------------------------------
			DrawText(TextFormat("Press P to pause", scoreLeft), 350, 30, 10, YELLOW);
			DrawText(TextFormat("SCORE %d", scoreRight), 550, 50, 30, WHITE);
			DrawText(TextFormat("SCORE %d", scoreLeft), 100, 50, 30, WHITE);
    //viteza si traiectoria initiala a bilei
			ball.x += ball.speedX * GetFrameTime();
			ball.y += ball.speedY * GetFrameTime();
  
			if (ball.y < 0)
			{
				ball.y = 0;
				ball.speedY *= -1;
			}
			if (ball.y > GetScreenHeight())
			{
				ball.y = GetScreenHeight();
				ball.speedY *= -1;
			}
    // paleta oponenta se va misca in functie de viteza data in setari anterior si se va misca odata cu coordonata y a bilei
			if (ball.y <= leftPaddle.y)
			{
				leftPaddle.y -= (leftPaddle.speed * difficulty) * GetFrameTime ();
			}
			if (ball.y >= leftPaddle.y)
			{
				leftPaddle.y += (leftPaddle.speed * difficulty) * GetFrameTime();
			}
    // paleta jucatorului se va misca la o viteza constanta setata anterior in inceputul int main()
			if (IsKeyDown(KEY_UP))
			{
				rightPaddle.y -= rightPaddle.speed * GetFrameTime();
			}
			if (IsKeyDown(KEY_DOWN))
			{
				rightPaddle.y += rightPaddle.speed * GetFrameTime();
			}
    // aici se intampla modificarea vitezei si a traiectoriei bilei in functie de locul unde a lovit paleta
			if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
			{
				if (ball.speedX < 0)
				{
					ball.speedX *= -1.1f;
					ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
				}
			}
			if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
			{
				if (ball.speedX > 0)
				{
					ball.speedX *= -1.1f;
					ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
				}
			}



    // aici se afla conditai de castig a jocului
			if (ball.x <= 0 && winnerText == nullptr )
			{ 
				winnerText = "     Right Player Wins! \n press SPACE to continue";
				scoreRight++;
			}
			if (ball.x >= GetScreenWidth() && winnerText == nullptr)
			{
				winnerText = "     Left Player Wins! \n press SPACE to continue";
				scoreLeft++;
			}


    //Aici se repeta jocul cand se apasa tasta specifica, atata tim cat exista un castigator in acel moment
			if (winnerText && IsKeyPressed(KEY_SPACE))
			{
				ball.x = GetScreenWidth() / 2;
				ball.y = GetScreenHeight() / 2;
				ball.speedX = 300;
				ball.speedY = 300;
				winnerText = nullptr;
			}
		}
		
	//aici se deseneaza jocul
		if (!isInMenu)
		if (IsKeyPressed(KEY_P)) isInMenu = true;

			BeginDrawing();
			ClearBackground(BLACK);

			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();
		
			// mingea se va oprii cand exista un castigator pentru a provenii memory leakurile potentiale
			if (winnerText)
			{
				ball.speedX = 0;
				ball.speedY = 0;
				int textWidth = MeasureText(winnerText, 60);
				DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
			}
		
			DrawFPS(10, 10);
			EndDrawing();
			
		
	}
	CloseWindow();

	return 0;
}