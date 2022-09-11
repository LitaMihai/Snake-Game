#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

struct Snake {
    int x, y;
}snake[100];

struct Apple {
    int x, y;
}apple;

int N = 30, M = 20;
int size = 16;
int w = size * 3/2 *N;
int h = size * M;

int dir, num = 2;

void Tick() {
    for (int i = num; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }
    if (dir == 0) snake[0].y += 1;
    if (dir == 1) snake[0].x -= 1;
    if (dir == 2) snake[0].x += 1;
    if (dir == 3) snake[0].y -= 1;

    if ((snake[0].x == apple.x) && (snake[0].y == apple.y)) {
        num++;
        apple.x = rand() % N;
        apple.y = rand() % M;
        for (int i = 0; i < num; i++) 
            if ((snake[i].x == apple.x) && (snake[i].y == apple.y)) {
                apple.x = rand() % N;
                apple.y = rand() % M;
            }
    }

    if (snake[0].x > N - 1) snake[0].x = 0;
    if (snake[0].y > M - 1) snake[0].y = 0;
    if (snake[0].x < 0) snake[0].x = N;
    if (snake[0].y < 0) snake[0].y = M;

    for (int i = 1; i < num; i++)
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            //Cut
            num = i;
        }
}

int main()
{
    srand(time(0));

    sf::RenderWindow window;
    window.create(sf::VideoMode(w, h), "Snake");

    window.setVerticalSyncEnabled(true);

    sf::Texture t1, t2, t3, t4;
    t1.loadFromFile("images/Background.png");
    t2.loadFromFile("images/Apple.png");
    t3.loadFromFile("images/Body.png");
    t4.loadFromFile("images/Head.png");

    sf::Sprite Background(t1);
    sf::Sprite Apple(t2);
    sf::Sprite Body(t3);
    sf::Sprite Head(t4);


    ////// Controls \\\\\\

    sf::Image Controls;
    Controls.loadFromFile("images/Controls_Snake.png");

    sf::Texture Texture_Controls;
    Texture_Controls.loadFromImage(Controls);

    Texture_Controls.setSmooth(true);

    sf::Sprite Sprite_Controls;
    Sprite_Controls.setTexture(Texture_Controls);

    Sprite_Controls.setPosition(N * size, 0);


    sf::Clock clock;
    double timer = 0, delay = 0.1;

    apple.x = 10;
    apple.y = 10;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        sf::Event e;

        while (window.pollEvent(e))
        {
            switch (e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            
            case sf::Event::Resized:
                //std::cout << event.size.width << " " << event.size.height<<"\n";
                window.setSize(sf::Vector2u(w, h));
                break;
            
            default:
                break;
            }
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && dir != 2)
            dir = 1;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && dir != 1)
            dir = 2;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && dir != 0)
            dir = 3;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && dir != 3)
            dir = 0;

        if (timer > delay) {
            timer = 0;
            Tick();
        }

        //////////DRAW\\\\\\\\\\

        window.clear();

        for(int i = 0; i < N; i++)
            for (int j = 0; j < M; j++) {
                Background.setPosition(i * size, j * size);
                window.draw(Background);
            }

        Head.setPosition(snake[0].x * size, snake[0].y * size);
        window.draw(Head);

        for (int i = 1; i < num; i++) {
            Body.setPosition(snake[i].x * size, snake[i].y * size);
            window.draw(Body);
        }

        window.draw(Sprite_Controls); //Right image

        Apple.setPosition(apple.x * size, apple.y * size);
        window.draw(Apple);

        window.display();
    }

    return 0;
}