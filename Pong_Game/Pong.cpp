#include <iostream>
#include <SFML/Graphics.hpp>
#include <experimental/random>

const int windowWidth = 1600;
const int windowHeight = 1000;
const sf::Color white = sf::Color::White;

class Player
{
    public:
        int yPos{};
        int xPos{};
        int yVel{};
        int xVel{};
        sf::RectangleShape playerBody;
    Player()
    {
        yPos = windowHeight/2 - 75;
        xPos = windowWidth - 25;
        yVel = 0;
        playerBody.setSize(sf::Vector2f(25,100));
        playerBody.setFillColor(sf::Color::Cyan);
        playerBody.setPosition(xPos - 75,yPos);
    }
    void update(bool up, bool down, bool reset_left, bool reset_right)
    {   
        if (reset_left || reset_right)
        {
            reset();
        }
        //Moves the Player Up & Down 
        if (up){yVel = -5;}
        if (down){yVel = 5;}
        if (!up && !down){yVel = 0;}
        playerBody.move(sf::Vector2f(0,yVel));
        collide();
    }
    void collide()
    {
        if (playerBody.getPosition().y + (playerBody.getLocalBounds().height) > 900){playerBody.setPosition(xPos - 75, 900 - playerBody.getLocalBounds().height);}
        if (playerBody.getPosition().y < 100){playerBody.setPosition(xPos - 75, 100);}
    }
    void reset()
    {
        yPos = windowHeight/2 - 75;
        xPos = windowWidth - 25;
        playerBody.setPosition(xPos - 75,yPos);
    }
};

class AiBot
{
    public:
        int yPos{};
        int xPos{};
        int yVel{};
        int xVel{};
        sf::RectangleShape AiBody;
    AiBot()
    { 
        yPos = windowHeight/2 - 75;
        xPos = 85;
        yVel = 0;
        AiBody.setSize(sf::Vector2f(25,100));
        AiBody.setFillColor(sf::Color::Cyan);
        AiBody.setPosition(xPos,yPos);
    }
    void update(int BallPos_Y, int BallPos_X, bool reset_left, bool reset_right)
    {   
        //Make the bot follows the ball when it reaches his side of the camp
        if (BallPos_X  < 800)
        {
            if (BallPos_Y < AiBody.getPosition().y){AiBody.move(sf::Vector2f(0,-5.0));}
            if (BallPos_Y > AiBody.getPosition().y){AiBody.move(sf::Vector2f(0,5.0));}
        }
        collide();
        if (reset_left || reset_right)
        {
            reset();
        }
    }
    void collide()
    {   
        //Doesn't let the AiBot go past the border line
        if ((AiBody.getPosition().y + AiBody.getLocalBounds().height) > 900){AiBody.setPosition(xPos, 900 - AiBody.getLocalBounds().height);}
        if (AiBody.getPosition().y < 100){AiBody.setPosition(xPos, 100);}
    }
    void reset()
    {
        yPos = windowHeight/2 - 75;
        xPos = 85;
        AiBody.setPosition(xPos,yPos);
    }
};

class Ball
{
    public:
        int yPos{};
        int xPos{};
        int yVel{};
        int xVel{};
        bool Left_Wall_Activate;
        bool Right_Wall_Activate;
        sf::CircleShape shape;
    Ball()
    {
        Left_Wall_Activate = false;
        Right_Wall_Activate = false;
        yPos = windowHeight/2;
        xPos = windowWidth/2;
        xVel = 6;
        yVel = 6;
        shape.setRadius(25);
        shape.setFillColor(sf::Color::Cyan);
        shape.setPosition(xPos,yPos);
    }
    void update(Player player, AiBot bot)
    {   
        //Check if the ball touched the right or left bounderies of the border
        if (Left_Wall_Activate || Right_Wall_Activate)
        {
            reset();
        }
        //makes the ball move
        xPos += xVel;
        yPos += yVel;
        shape.move(sf::Vector2f(xVel, yVel));
        collide(xVel, yVel, player, bot);     
    }
    void collide(float xVelDelta, float yVelDelta, Player player, AiBot bot)
    {   
        // Check and handle collisions with window boundaries
        if (xPos + shape.getLocalBounds().width > windowWidth - 65)
        {
            Left_Wall_Activate = true;
            xVel = -xVel;
        }
        if (xPos  < 80)
        {
            Right_Wall_Activate = true;
            xVel = -xVel;
        }
        if (yPos + shape.getLocalBounds().height - 5 > windowHeight - 100 || yPos  < 105)
        {
            yVel = -yVel;
        }  
        //Check for collision for the player and bot
        // if true: Reverse velocity along the x-axis if colliding with player
        if (shape.getGlobalBounds().intersects(player.playerBody.getGlobalBounds()))
        {
            if (xVelDelta > 0){xVel = -abs(xVel);}
            else if (xVelDelta < 0){xVel = abs(xVel);}
        }
        // if true: Reverse velocity along the x-axis if colliding with bot
        else if (shape.getGlobalBounds().intersects(bot.AiBody.getGlobalBounds()))
        {
            if (xVelDelta > 0){xVel = -abs(xVel);}
            else if (xVelDelta < 0){xVel = abs(xVel);}
        }
    }
    //Resets the ball and gives a random direction
    void reset()
    {   
        yPos = windowHeight/2;
        xPos = windowWidth/2;
        shape.setPosition(xPos,yPos);
        std::srand(std::time(nullptr)); // using current os time for the seed
        int random_value = std::rand() % 4 + 1;
        switch (random_value)
        {
        case 1:
            xVel = 6;
            yVel = 6;
            break;
        case 2:
            xVel = -6;
            yVel = 6;
            break;
        case 3:
            xVel = -6;
            yVel = -6;
        case 4:
            xVel = 6;
            yVel = -6;
        }
        Left_Wall_Activate = false;
        Right_Wall_Activate = false;
        
    }
};

int main()
{   
    bool up,down = false;
    int AiScore = 0;
    int PlayerScore = 0;
    //Window settings and classes declarations
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Pong game!", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    Ball ball;
    Player player;
    AiBot bot;
    
    //Scoreboard setup
    sf::Font consola;
    consola.loadFromFile("Fonts/CONSOLA.TTF");
    //Player score
    sf::Text Player_Score("Player: ",consola,50);
    Player_Score.setPosition(850,25);
    Player_Score.setFillColor(sf::Color::Black);
    //Ai score
    sf::Text Ai_Score("Ai: ",consola,50);
    Ai_Score.setPosition(650,25);
    Ai_Score.setFillColor(sf::Color::Black);

    //Line in the middle of the camp
    sf::RectangleShape line;
    line.setPosition(sf::Vector2f(windowWidth/2, 100));
    line.setSize(sf::Vector2f(5,windowHeight - 200));
    line.setFillColor(sf::Color::Black);

    //Setting the lines around the camp
    sf::RectangleShape borderline;
    borderline.setPosition(sf::Vector2f(75,100));
    borderline.setSize(sf::Vector2f(1460,800));
    borderline.setFillColor(sf::Color::White);
    borderline.setOutlineThickness(10);
    borderline.setOutlineColor(sf::Color::Black);  

    //Circle of the line in the middle of the camp
    sf::CircleShape circle(150);
    circle.setPosition(sf::Vector2f(windowWidth/2 - circle.getRadius(),windowHeight/2 - circle.getRadius()));
    circle.setFillColor(sf::Color::White);
    circle.setOutlineThickness(5);
    circle.setOutlineColor(sf::Color::Black);
    circle.setPointCount(100);

    // run the program as long as the window is open
    while (window.isOpen())
    {   

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //Keyboard input control
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){up = true;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){down = true;}
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){up = false;}
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){down = false;}
        
        if (ball.Left_Wall_Activate)
        {   
            AiScore++;
            Ai_Score.setString("Ai: " + std::to_string(AiScore));
        }
        if (ball.Right_Wall_Activate)
        {   
            PlayerScore++;
            Player_Score.setString("Player: " + std::to_string(PlayerScore));
        }

        // clear the window with black color
        window.clear(sf::Color::White);
        //Drawing
        window.draw(Player_Score);
        window.draw(Ai_Score);
        window.draw(borderline);
        window.draw(circle);
        window.draw(line);
        window.draw(ball.shape);
        window.draw(player.playerBody);
        window.draw(bot.AiBody);
        //Updating
        bot.update(ball.yPos, ball.xPos, ball.Left_Wall_Activate, ball.Right_Wall_Activate);
        ball.update(player,bot);
        player.update(up, down, ball.Left_Wall_Activate, ball.Right_Wall_Activate);
        window.display();
    }

    return 0;
}