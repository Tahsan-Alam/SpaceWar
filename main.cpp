/*********************
Name: Tahsan Ul Alam
Date: 12/4/2024

Description:
Adding new functionality to the asteroids game written by Fam Trinli

Part 3:
1.The ufo appears from the right side from a specific position
  at a time interval. At the first time, it is 3 sec and from the
  second time, it is 8 sec


Added by me for part 4:
1.added player bullet sound
2.added explosion sound
3.added player bullet limit:
    there are 15 bullets. After firing 15 bullets the weapon gets overheats
    after pressing R the weapon gets loaded again with 15 bullets

4.added enemy boss:
    enemy boss will appear if player can get a spefic num
    of kills. player can get kill by shooting ufo and asteroids
    ufo and all asteroids are removed during the boss fight
    Player has 300 health and enemy has 500 health
    if player wins the round, enemy's health will increse by 100
    if player losses the round, enemy's health will decrease by 100

5.added enemy bullet (used red bullet):
    enemy spaceship fires 25 bullets at random postion between max and min position
    at 2 sec interval. The bullet does damage to the player. Player also loses
    health by hiting the spaceship

6.added enemy bullet sound
7.added fight sound
8.added player win and lose sound
9.change player spaceship movement keys
    w -> thrust
    a -> left
    d -> right

Ufo image source:
Author: bevouliin.com
Publish Date: Tuesday, April 12, 2016 - 02:58
License: CC0 1.0
Link: https://opengameart.org/content/ufo-enemy-game-character

Ufo sound source:
Author: NHumphrey (Freesound)
Publish Date: August 15, 2022
License: Pixabay Content License
Link: https://pixabay.com/sound-effects/ufo-87476/

Bullet sound source:
Author: Freesound Community
Publish Date: August 15, 2022
License: Pixabay Content License
Link: https://pixabay.com/sound-effects/060130-laser-bullet-86975/

Fight sound source:
Author: gis_sweden (Freesound)
Publish Date: June 8, 2022
License: Pixabay Content License
Link: https://pixabay.com/sound-effects/bass-drum-through-driver-2-bars-145-38674/

Player win sound source:
Author: FunWithSound (Freesound)
Publish Date: August 4, 2021
License: Pixabay Content License
Link: https://pixabay.com/sound-effects/success-fanfare-trumpets-6185/

Player lost sound source:
Author: Audiosto
Publish Date: April 2, 2024
License: Pixabay Content License
Link: https://pixabay.com/sound-effects/game-over-39-199830/
******************************/

#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <cmath>
#include <SFML/Audio.hpp>

using namespace sf;

const int W = 1200; // Window width
const int H = 800; // Window height

float DEGTORAD = 0.017453f; // 1 radian = pi/180 = 0.017453 to convert degree to radian

class Animation
{
public:
    float Frame, speed; // Current frame index and speed of animation
    Sprite sprite;
    std::vector<IntRect> frames; // IntRect type vector of frames

    Animation() //Default Constructor
    {
    }

    // Constructor
    Animation(Texture& t, int x, int y, int w, int h, int count, float Speed)
    {
        Frame = 0; // Frame of the animation
        speed = Speed;

        for (int i = 0; i < count; i++)
        {
            //IntRec is used to create 2D rectangle with
            // Integer coordinates
            // x + i * w  as left coordinate of the rectangle
            // y as top coordinate of the rectangle
            // w as width of the rectangle
            // h as height of the rectangle
            frames.push_back(IntRect(x + i * w, y, w, h));
        }
        // Binds Texture t to sprite
        // as long as it exists
        sprite.setTexture(t);

        // Center of a sprite
        sprite.setOrigin(w / 2, h / 2);

        //set the sub-rectangle of the texture that the
        //sprite will display
        // it is set to the first elem of the frame vector
        sprite.setTextureRect(frames[0]);
    }

    // Update the animation to progress through frames based on the speed
    void update()
    {
        Frame += speed; //increment frame by speed
        int n = frames.size(); // length of vector frames (Type intRect)
        if (Frame >= n) // if Frame is greater than or equal to the length of vector frames
        {
            Frame -= n; // Decrement frame by n
        }
        if (n > 0) // if vector frames is not empty
        {
            sprite.setTextureRect(frames[int(Frame)]); // set the texture rectangle to frame vector's elements' sub rectangle
        }
    }

    //check if the animation has reached the final frame
    bool isEnd()
    {
        return Frame + speed >= frames.size(); // True if animation is at or past the last frame
    }
};


class Entity
{
public:
    float x, y; //Position of the entity on the screen
    float dx, dy; //Velocity of the entity in x and y directions
    float R; //Radius used for collision detection
    float angle; //Angle of rotation of the entity
    bool life; //Indicates whether the entity is alive or dead
    std::string name; //Nmae of the entity
    Animation anim; //Defining animation

    //Default constructor initializes life to true
    Entity()
    {
        life = true;
    }

    void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1)
    {
        anim = a; //Assign animation to the entity's anim property
        x = X; //set initial position (x)
        y = Y; //set initial position (y)
        angle = Angle; // set initial angle
        R = radius; //set collision radius
    }

    //Polymorphism
    //Derived classes override to define behaviors
    virtual void update()
    {
    };

    void draw(RenderWindow& app)
    {
        //Set the position and rotation of the entity's sprite
        anim.sprite.setPosition(x, y);
        anim.sprite.setRotation(angle + 90);
        app.draw(anim.sprite); //Draw the sprite

        //Draw a red circle around the entity to visualize its radius
        CircleShape circle(R); //Create a circle with radius R
        circle.setFillColor(Color(255, 0, 0, 170)); //Semi-transparent red
        circle.setPosition(x, y); //Set position of the circle
        circle.setOrigin(R, R); //Center the circle
        //app.draw(circle);
    }

    //Polymorphism
    //Virtual destructor
    virtual ~Entity()
    {
    };
};


class asteroid : public Entity
{
public:
    //Constructor initializes the asteroid's properties
    static unsigned int count; // to count asteroid
    asteroid()
    {
        //Assign random velocities in the x and y directions
        // rand() % 8 - 4 generates a random number in the range [-4,4]
        dx = rand() % 8 - 4;
        dy = rand() % 8 - 4;

        //Set the name of the entity to "asteroid"
        name = "asteroid";
        count++; //to spawn asteroid
    }

    ~asteroid()
    {
        count--; //to delete asteroid
    }
    //Updates the asteroid if it goes out of the screen
    void update()
    {
        //increment x and y position based on velocity
        x += dx;
        y += dy;

        if (x > W) //Wrap to the left if it moves beyond the right edge
        {
            x = 0;
        }
        if (x < 0) //Wrap to the right if it moves beyond the left edge
        {
            x = W;
        }
        if (y > H)//Wrap to the top if it moves beyond the bottom edge
        {
            y = 0;
        }
        if (y < 0)// Wrap to the bottom if it moves beyond the top edge
        {
            y = H;
        }
    }

};

unsigned int asteroid::count = 0;

class bullet : public Entity //The bullet class inherits from the Entity class
{
public:
    //Default constructor
    bullet()
    {
        name = "bullet"; //assigns the name "bullet" to name
    }

    void update()
    {
        //Calculate the velocity (dx and dy) of the bullet using trigometry
        //The angle of the bullet (in degress) is converted to radians using DEGTORAD
        dx = cos(angle * DEGTORAD) * 6; //Horizontal velocity
        dy = sin(angle * DEGTORAD) * 6; //Vertical velocity
        // angle+=rand()%6-3;

        //Update the bullet's position by adding its velocity to its current position
        x += dx; //Move the bullet horizontally
        y += dy; //Move the bullet vertically

        //if the bullet moves outside the game window, life becomes 0
        if (x > W || x < 0 || y > H || y < 0)
        {
            life = false; //The bullet will be removed
        }
    }
};


class player : public Entity // base class Entity Derived class player
{
public:
    bool thrust;

    //default constructor
    player()
    {
        name = "player";
    }

    void update()
    {
        //spaceship velocity in x and y direction
        if (thrust)
        {
            // decompose vector into x and y component
            dx += cos(angle * DEGTORAD) * 0.2;
            dy += sin(angle * DEGTORAD) * 0.2;
        }

        else
        {
            dx *= 0.99; // change in x direction if no thrust
            dy *= 0.99; // change in y direction if no thrust
        }

        //Limit the maximum speed to prevent the player from moving too fast
        int maxSpeed = 15; //Define the maximum speed
        float speed = sqrt(dx * dx + dy * dy); //calculate the current speed using pythagoras theorem
        if (speed > maxSpeed) //if the speed exceeds the maximum
        {
            dx *= maxSpeed / speed; //Scale down horizontal velocity
            dy *= maxSpeed / speed; // Scale down vertical velocity
        }

        //update the player's x and y position by adding velocity to the current position
        x += dx;
        y += dy;

        // if spaceship goes out of window in x and y diretion
        // reset the position of the spaceship inside the window
        if (x > W) // If the player moves off the right edge, reappear on the left.
        {
            x = 0;
        }
        if (x < 0)// If the player moves off the left edge, reappear on the right.
        {
            x = W;
        }
        if (y > H) // If the player moves off the bottom edge, reappear at the top
        {
            y = 0;
        }
        if (y < 0)// If the player moves off the top edge, reappear at the bottom.
        {
            y = H;
        }
    }
};

class ufo:public Entity
{
public:
    static bool isAlive; //to check if ufo is alive or not

    ufo()
    {
        dx = -5; //to move ufo from right side
        name = "ufo"; //name of the object
    }

    void update()
    {
        //increment x and y position based on velocity
        x += dx;
    }

};
bool ufo::isAlive = true; //initial value


class enemy:public Entity
{
public:
    static bool alive; //to check if enemy is alive or not
    enemy()
    {
        name = "enemy"; //name of the object
    }

};

bool enemy::alive = false; //initial value

class enemBullet:public bullet // base class bullet inherit class enemBullet
{
public:
    enemBullet()
    {
        name = "enemBullet"; //name of the object
    }

};

bool isCollide(Entity* a, Entity* b)
{
    //(b->x - a->x) is the difference in the x coordinates of the entities
    //(b->y - a->y) is the difference in the y coordinates of the entities
    // calclute the linear distance between entity a(a->x, a-> y) and entity b(b->x, b->y)
    // then check if the distance is less than the square of sum of their radii
    return (b->x - a->x) * (b->x - a->x) +
        (b->y - a->y) * (b->y - a->y) <
        (a->R + b->R) * (a->R + b->R);
}


int main()
{
    //Initializes the random number generator using the current time as the seed
    srand(time(0));

    //Game Window W for width and H for Height
    //title for the window as third parameter
    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60); // set frame limit to 60 fps

    // loading texture
    // t1 -> spaceship
    // t2 -> background
    // t3 -> explosions
    // t4 -> rock
    // t5 -> blue bullet
    // t6 -> small rock
    // t7 -> explosions
    // t8 -> ufo
    // t9 -> spaceship
    // t10 -> red fire
    Texture t1, t2, t3, t4, t5, t6, t7, t8, t9,t10;
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");
    t8.loadFromFile("images/ufo_game_enemy.png");
    t9.loadFromFile("images/Spaceship_tut.png");
    t10.loadFromFile("images/fire_red.png");

    //setting t1 and t2 texture smooth
    t1.setSmooth(true);
    t2.setSmooth(true);


    Sprite background(t2); // load sprite t2 as background

    // Animation to set up properties of the objects
    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sEnemBullet(t10,0,0,32,64,16,0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
    Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);
    Animation sUfo(t8,0,0,641,546,1,0);
    Animation sEnemy(t9,0,0,164,241,1,0);

    sUfo.sprite.setScale(0.2f,0.2f); //to make the ufo sprite small
    sEnemy.sprite.setScale(4,2.5f); //to make the enemy spaceship large

    //The list is containing pointers to base class Entity object
    //Polymorphism
    std::list<Entity*> entities;

    //creating player object
    player* p = new player();

    //calling settings of class Entity
    //to set up x and y position and radius
    p->settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(p); // add player object to entities list

    int time = 3; //Delay time of spawning ufo
    Time totalTime = seconds(time); //total delay time
    Time passedTime = seconds(0.01f);//to substract from total time to get the current time passed

    int pause = 2; //Delay time of lauching bullets from enemy spaceship
    Time totalPause = seconds(pause); //total delay time
    Time passed = seconds(0.01f);

    //loading Ufo Sound
    SoundBuffer buffer;
    if(!buffer.loadFromFile("sounds/ufo-sound.ogg"))
    {
        return -1;
    }
    Sound sound;
    sound.setBuffer(buffer);

    //loading bullet sound of Ufo
    SoundBuffer buffer2;
    if(!buffer2.loadFromFile("sounds/laser-bullet.ogg"))
    {
        return -1;
    }
    Sound sound2;
    sound2.setBuffer(buffer2);

    //loading explosion sound
    SoundBuffer buffer3;
    if(!buffer3.loadFromFile("sounds/explosion.ogg"))
    {
        return -1;
    }
    Sound sound3;
    sound3.setBuffer(buffer3);

    //loading Boss Fight sound
    SoundBuffer buffer4;
    if(!buffer4.loadFromFile("sounds/BossFight.ogg"))
    {
        return -1;
    }
    Sound fightSound;
    fightSound.setBuffer(buffer4);

    //loading enemy bullet sound
    SoundBuffer buffer5;
    if(!buffer5.loadFromFile("sounds/enemy-laser.ogg"))
    {
        return -1;
    }
    Sound enemLaser;
    enemLaser.setBuffer(buffer5);

    //loading player lost sound
    SoundBuffer buffer6;
    if(!buffer6.loadFromFile("sounds/game-over.ogg"))
    {
        return -1;
    }
    Sound enemWin;
    enemWin.setBuffer((buffer6));

    //loading player win sound
    SoundBuffer buffer7;
    if(!buffer7.loadFromFile("sounds/success.ogg"))
    {
        return -1;
    }
    Sound playerWin;
    playerWin.setBuffer(buffer7);

    //loading font
    Font font;
    if(!font.loadFromFile("fonts/Phelya.ttf"))
    {
        return -1;
    }

    //text for bullet
    Text text;
    text.setFont(font);

    text.setCharacterSize(24); //Font size
    text.setFillColor(Color::Red); //Font color
    text.setPosition(600,100); //Font position
    text.setString("Bullets: 15"); //to show num of bullets

    //text for enemy health
    Text text2;
    text2.setFont(font);

    text2.setCharacterSize(24); //Font size
    text2.setFillColor(Color::Red);//Font color

    //text for player health and fight result
    Text text3;
    text3.setFont(font);

    text3.setCharacterSize(24); //font size
    text3.setFillColor(Color::Red);//font color
    text3.setPosition(600,200);//font position

    int bulletLimit = 15; //player bullet limit
    int kill = 0; //kill
    int target = 20; //target to fight with enemy boss
    int enemyInitHealth = 500; //enemy boss initial health
    int enemyLive = enemyInitHealth; //enemy health
    int enemyBulletLimit = 15; //enemy bullet limit
    int playerHealth = 300; //player health during boss fight

    /////main loop/////
    while (app.isOpen())
    {

        Event event;
        //Event loop
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                app.close();
            }

            //to check if space is pressed to
            //fire bullets
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Space && bulletLimit > 0)
                {
                    bulletLimit -= 1; //decrementing bullet
                    text.setString("Bullets " + std::to_string(bulletLimit));//showing num of bullet
                    //creating bullet object
                    bullet* b = new bullet();

                    //setting up x and y movement, angle and radius
                    b->settings(sBullet, p->x, p->y, p->angle, 10);
                    sound2.play(); //bullet sound
                    entities.push_back(b); //adding bullet object to entities list
                }

            }

        }

        //if there is no asteroid and enemy boss is not spawned
        if(asteroid::count == 0 && enemy::alive == false)
        {
            //spawn 15 random asteroid on the space
            for (int i = 0; i < 15; i++)
            {
                asteroid* a = new asteroid();

                //assigning rock animation, height, width, angle and radius
                a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
                entities.push_back(a); // adding asteroid to the list(type: entity class)
            }
        }

        if(bulletLimit == 0) //if there is no bullet
        {
            //Weapon overheats
            //press R to lauch bullet again
            if(Keyboard::isKeyPressed(Keyboard::R))
            {
                bulletLimit = 15; //reset bullet limit
                text.setString("Bullets: 15");
            }
            else
            {
                text.setString("Overheat!! Press R"); //Instruction
            }
        }

        //to turn right
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            p->angle += 3; // rotation of right side
        }

        //to turn left
        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            p->angle -= 3; // rotation of left side
        }

        //to move up
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            p->thrust = true; //enable thrust from player class
        }
        else
        {
            p->thrust = false; // stop thrust i.e stop moving
        }

        if(enemy::alive) //if enemy is spawned
        {
            //nested loop iterate through list of objects
            for (auto a : entities)
            {
                for (auto b : entities)
                {
                    //if objects' names are enemy and bullet
                    if (a->name == "enemy" && b->name == "bullet")
                    {
                        //if player bullet hits enemy
                        if (isCollide(a, b))
                        {
                            //if enemy is alive
                            if (enemyLive > 0 && enemy::alive == true)
                            {
                                enemyLive -= 1; //decrement life of enemy
                                text2.setString("Enemy Health: " + std::to_string(enemyLive)); //to show enemy health
                            }

                        }
                    }
                    //if objects' names are player and enemy or enemyBullet
                    if(a->name == "player" && (b->name == "enemy" || b->name == "enemBullet"))
                    {
                        //if player hits enemy spaceship or enemy bullet hits player
                        if (isCollide(a, b))
                        {
                            //if it is enemy bullet
                            if(b->name == "enemyBullet")
                            {
                                b->life = false; //to delete enemy bullet
                            }
                            playerHealth -= 5; //decrement player health
                            text3.setString("Player Health: " + std::to_string(playerHealth)); //to show player health
                        }
                    }

                }
            }
            //if player loses all health and enemy spaceship still has health
            if(playerHealth == 0 && enemyLive > 0)
            {
                enemy::alive = false; //remove enemy spaceship
                playerHealth = 300; //reset player health
                enemyLive = enemyInitHealth - 100; //decrease player health to make the next boss less hard
                text3.setString("Player lost!!!");//show player lost
                text2.setString(" "); //blank enemy health
                fightSound.stop(); //stop fight sound
                enemWin.play(); //play player lost sound
            }
            if (enemyLive == 0 && playerHealth > 0)
            {
                enemy::alive = false; //remove spaceship
                enemyLive = enemyInitHealth + 100; //increase enemy health to make the next boss hard
                playerHealth = 300; //reset player health
                text3.setString("Enemy has fallen!!!"); //player win state
                text2.setString(" "); //blank enemy health
                fightSound.stop(); //stop fight sound
                playerWin.play(); //play player win sound
            }
            //to remove ufo and all asteroids from the space when
            //enemy spaceship appears
            for (auto a : entities)
            {
                if(a->life == true) // if a object still exists
                {
                    //if a is ufo or asteroid
                    if(a->name == "ufo" || a->name == "asteroid")
                    {
                        a->life = false; //to remove the object
                    }
                    //if a is enemy boss or enemy bullet and enemy is dead
                    if((a->name == "enemy" || a->name == "enemBullet") && enemy::alive == false)
                    {
                        a->life = false; // remove from the game
                    }
                }
            }
            //if enemy bullet is not 0
            while(enemyBulletLimit != 0)
            {
                //create enemBullet object
                enemBullet *b = new enemBullet;
                int max = 950; // max range of bullet position
                int min = 100; // min range of bullet position
                //spawn bullet towards player at radom position between max and min
                b->settings(sEnemBullet,960,rand()%(max - min + 1) + min, -180,10);
                entities.push_back(b); //added bullet to entities list
                enemLaser.play(); //play enemy bullet sound
                enemyBulletLimit -= 1; //decrement enemy bullet
            }
            if(totalPause.asSeconds() > 0) //check if the delay time has passed
            {
                totalPause -= passed; //decrement time
            }
            else // if time is 0
            {
                enemyBulletLimit = 25; //reset enemy bullet to spawn bullets again
                totalPause = seconds(pause); // reset total time
            }
        }


        if (enemy::alive == false) //if enemy is removed
        {
            for (auto a : entities)
            {
                for (auto b : entities)
                {
                    //to check asteroid,bullet and ufo sequence from the list
                    if ((a->name == "asteroid" || a->name == "ufo") && b->name == "bullet")
                    {
                        //if isCollide function returns true
                        //it returns true when asteroid and bullet collide
                        if (isCollide(a, b))
                        {
                            //if a is asteroid or ufo
                            if (a->name == "asteroid" || a->name == "ufo")
                            {
                                a->life = false; // entity a has been marked as destroyed
                            }

                            b->life = false; // entity b has been marked as destroyed

                            Entity* e = new Entity(); // new Entity object is created
                            e->settings(sExplosion, a->x, a->y); //set its property
                            sound3.play();
                            e->name = "explosion"; // Explosive is marked as a name of the object
                            entities.push_back(e); // Add explosion to the entities list

                            //if a is ufo
                            if (a->name == "ufo")
                            {
                                kill += 1; //increment kill count by 1
                                ufo::isAlive = false; //mark ufo as dead
                                sound.stop(); //stop ufo sound
                                sound3.play(); //play explosion sound
                            }
                            else
                            {
                                kill += 1; //increment kill count by 1
                                //Break asteroid into smaller pieces
                                for (int i = 0; i < 2; i++)
                                {
                                    //if the astroid has already split into smaller parts
                                    //Skip the iteration
                                    if (a->R == 15)
                                    {
                                        continue;
                                    }
                                    Entity* e = new asteroid(); //create new asteroid object
                                    e->settings(sRock_small, a->x, a->y, rand() % 360, 15); //set its property
                                    entities.push_back(e); //add asteroid object to entities list
                                }
                            }
                        }
                    }

                    //to check player, asteroid, and ufo sequence from the list
                    if (a->name == "player" && (b->name == "asteroid" || b->name == "ufo"))
                    {
                        //to check if player and asteroid collided
                        if (isCollide(a, b))
                        {
                            b->life = false; //mark asteroid as dead

                            Entity* e = new Entity(); //create new entity object
                            e->settings(sExplosion_ship, a->x, a->y); //set properties of entity
                            sound3.play(); //play explosion sound
                            e->name = "explosion"; //set entity's name as explosion
                            entities.push_back(e); //added entity object to entities list
                            if (b->name == "ufo")
                            {
                                ufo::isAlive = false;
                                sound.stop();
                                sound3.play();
                            }
                            //set player p's settings
                            //move player at the middle of the screen
                            p->settings(sPlayer, W / 2, H / 2, 0, 20);
                            p->dx = 0; //Reset x velocity
                            p->dy = 0; //Reset y velocity
                        }
                    }
                }
            }
        }

        //check if ufo will spawn or not and check if enemy
        //spaceship is not on the screen
        if(ufo::isAlive && enemy::alive == false)
        {
            //if time is 0
            if(totalTime.asSeconds() == 0)
            {
                //create a ufo object
                ufo *e = new ufo();
                //set up properties of ufo
                //ufo comes from right side of the screen
                e->settings(sUfo,1200,400,-90,30);
                entities.push_back(e); //added to entities list
                sound.play(); //play ufo sound
                time = 8; //increase time interval of ufo apperance
                totalTime = seconds(time); //reset total time
                ufo::isAlive = false; //mark ufo as dead
            }

            //if time is not 0
            if(totalTime.asSeconds() > 0)
            {
                totalTime -= passedTime; //decrement time
            }
        }
        else
        {
            ufo::isAlive = true; //mark ufo as alive
        }

        //if player caught enough kills to spawn enemy boss
        if(kill == target)
        {
            target += 20; //increase target for next boss apperance
            kill = 0; //reset kill count

            //create enemy object
            enemy *e = new enemy();
            //set up enemy's properties
            e->settings(sEnemy, 960, 400, -180, 200);
            entities.push_back(e); //added enemy object to entities list
            fightSound.setLoop(true); //to repeat the fight sound
            fightSound.play(); //play boss fight sound
            text2.setString("Enemy Health: " + std::to_string(enemyLive)); //to show enemy health
            text3.setString("Player Health: " + std::to_string(playerHealth)); //to show player health
            enemy::alive = true; //mark enemy as alive
        }


        if (p->thrust) // if thrust is activate
        {
            p->anim = sPlayer_go; //switch the thrusting animation (blue color from the spaceship)
        }
        else
        {
            p->anim = sPlayer; //switch to the default animation when not thrusting
        }

        //looping through entities list
        for (auto e : entities)
        {
            //if the entity's name is explosion
            if (e->name == "explosion")
            {
                //check if the explosion animation has completed
                if (e->anim.isEnd())
                {
                    e->life = false; //mark the explosion entity as dead
                }
            }

        }

        //Random chance spawn a new asteroid
        //if the number is zero then a new asteroid is created
        if (rand() % 150 == 0 && kill < target)
        {
            asteroid* a = new asteroid(); // a new asteroid is created

            //set asteroid's animation, position, rotation, and size
            a->settings(sRock, 0, rand() % H, rand() % 360, 25);
            entities.push_back(a); //add the new asteroid to the entities list
        }

        // i is entity pointer interator
        // entities.begin() -> first entity in entities list

        for (auto i = entities.begin(); i != entities.end();)
        {
            Entity* e = *i; // e is a pointer to an entity, i is an iterator. deferencing i to have value of *e

            e->update(); //Polymorphism base class overriden to update entity
            e->anim.update(); // Polymorphism to update animation

            if (e->life == false) // if entity is dead
            {
                // first .erase removes the dead entity from list
                // then the method returns the iterator pointing to
                // the next entity of the list
                // thus iterator i gets updated
                i = entities.erase(i);
                delete e; // deallocate memory
            }
            else
            {
                i++; // increment iterator if no deletion occurs
            }
        }

        //////draw//////
        app.draw(background);
        app.draw(text); // draw num of bullets
        app.draw(text2); // draw enemy health
        app.draw(text3); // draw player health and fight result
        //looping through entities list
        for (auto i : entities)
        {

            i->draw(app); //drawing all objects
        }

        app.display();

    }

    return 0;
}
