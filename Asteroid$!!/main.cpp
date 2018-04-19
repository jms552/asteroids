

/*
Julie Salah
Getting the missiles to hit the asteroids with Functions!
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
#include <stdlib.h>
#include <vector>

using namespace std;
using namespace sf;

//*******************  Function Prototypes *****************************************//

//buildmissilefunction
void setmissiles(vector<Sprite>&, const Texture &T2);

//buildasteroids and set position function
void setasteroids(vector<Sprite>&,const Texture &T, uniform_int_distribution<int>&, uniform_int_distribution<int>&, mt19937&, mt19937&);

//missilemovementfunction
void missilemovement(vector<Sprite>&, int count);

//asteroidmovementfunction
void asteroidmovement(vector<Sprite> &asteroid, uniform_int_distribution<int> &dist, mt19937 &gen,bool &firstMovement, int movement[]);

//test for collision asteroids
void asteroidcollision(vector<Sprite> &asteroid, int movement[]);

//test for collision asterds with missiles
void fullcollisioncheck(vector<Sprite> &missiles, vector<Sprite> &asteroid);

//had to set movement outside of main because multiple function call from these variables
int movement[] = {0,1,2,3,4,5,6,7,8,9};

//*********************************************  MAIN *******************************************************************//

int main()
{
    
    //window settings. Color, Size and window name - 800 and 600 is the window size i picked
    RenderWindow window(sf::VideoMode(800, 600), "Asteroid$!!!");
    window.clear(sf::Color::Black);
    
    //get background image - Space
    Texture imagebackground;
    imagebackground.loadFromFile("/Volumes/Transcend/COSC_051/COSC_051/Asteroid$!! assignment2(c)/space2.jpg");
    
    //random number generator for starting position - had to be set outside the function then call for them through the parameters
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen1(seed);
    uniform_int_distribution<int> dist1(1,800);
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen2(seed1);
    uniform_int_distribution<int> dist2(0,600);
    
    //setting random numbers for movement
    unsigned seed2 = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed2);
    uniform_int_distribution<int> dist(-2,3);
    
    //had to add this so the asteroids could keep the positions once they were set
    bool firstMovement = true;
    
    
    //initalizing sprites - missiles
    vector<Sprite> missiles(3,Sprite());
    
    //initalizing sprites - asteroids
    vector<Sprite> asteroid(5,Sprite());
   
    
    //setting the texture for asteroid and missiles - had to do this outside the function because it was not setting the texture inside the fucntion
    Texture textureast;
    textureast.loadFromFile("/Volumes/Transcend/COSC_051/COSC_051/Asteroid$!! assignment2(c)/en.lproj/Vesta_Rotation.gif");
    Texture texturemiss;
    texturemiss.loadFromFile("/Volumes/Transcend/COSC_051/COSC_051/Asteroid$!! assignment2(c)/en.lproj/missile3.png");
  
    
    //function calls to set the missiles and asteroids in their positions. Asteroids used random number generator
    setasteroids( asteroid, textureast, dist1, dist2, gen1, gen2);
    setmissiles( missiles, texturemiss);
    
    
    //set counter for the spacebar so whenever you hit spacebar a missile will lauch
    int count = 0;
    
    //need to create an event to fire the missile
    Event firemissiles;

    while (window.isOpen())
    {
         window.clear();
        
        
    // Creates a while loop for the keypressevent
        while (window.pollEvent(firemissiles))
        {
            if (firemissiles.type == Event::KeyPressed)
            {
                if (firemissiles.key.code == Keyboard::Space)
                {
                    count++;
                    //spacepress = true;
                }
            
            }
            
            //when user hits close button it will shut down program
            if (firemissiles.type == sf::Event::Closed)
                window.close();
        }
        
        
            //call to function for missile movement
            missilemovement( missiles, count);
    
    //call for asteroid movement
    asteroidmovement(asteroid, dist, gen, firstMovement, movement);
    
    //missilecollision
    fullcollisioncheck( missiles, asteroid);
        

        
        //Draw the sprites
        for (int i = 0; i < 3; i++)
        {
            window.draw(missiles[i]);
        }
        
        for (int i = 0; i < 5; i++)
        {
            window.draw(asteroid[i]);
        }
        
        //window display
        window.display();
        sf::sleep(sf::milliseconds(2));
        }

    return 0;
}

//*********************************************  Set Missiles Function *************************************************************//

void setmissiles(vector<Sprite> &missile, const Texture &T2)
{
    
    
    //set up missiles
    for (int i = 0; i < 3; i++)
    {
    missile[i].setTexture(T2);
    missile[i].scale(.7f, .7f);
    missile[i].setPosition(400, 650);
    missile[i].setColor(Color::Magenta);
    }
    
    return;
}

//*********************************************  Set Asteroid Functions ***********************************************************//

void setasteroids(vector<Sprite> &asteroid,const Texture &T, uniform_int_distribution<int> &dist1, uniform_int_distribution<int> &dist2, mt19937 &gen1, mt19937 &gen2)
{
    //setting random numbers for initial position, 800 is x window and 600 is y window
    
    int intposX [] = {0,1,2,3,4};
    int intposY [] = {0,1,2,3,4};
    
    intposX[0] = static_cast<int>(dist1(gen1));
    intposX[1] = static_cast<int>(dist1(gen1));
    intposX[2] = static_cast<int>(dist1(gen1));
    intposX[3] = static_cast<int>(dist1(gen1));
    intposX[4] = static_cast<int>(dist1(gen1));
    
    intposY[0] = static_cast<int>(dist2(gen2));
    intposY[1] = static_cast<int>(dist2(gen2));
    intposY[2] = static_cast<int>(dist2(gen2));
    intposY[3] = static_cast<int>(dist2(gen2));
    intposY[4] = static_cast<int>(dist2(gen2));

    for (int i = 0; i< 5; i++)
    {
        asteroid[i].setTexture(T);
        asteroid[i].scale(.125f, .125f);
        asteroid[i].setPosition(intposX[i], intposY[i]);
        asteroid[i].setColor(Color::Blue);
    }
    
    return;
}

//*********************************************  Missile Movement Function *******************************************************//

void missilemovement(vector<Sprite> &missiles, int count)
{
    //set counter for the spacebar so whenever you hit spacebar a missile will lauch
    if ( count == 1)
    {
        missiles[0].move(0,-3);
        //will move left and right to try to hit asteroid
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            // left key is pressed: move missile
            missiles[0].move(-2, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            // left key is pressed: move missile
            missiles[0].move(2, 0);
        }
    
    }
    if (count == 2)
    {
        missiles[1].move(0,-3);
        //will move left and right to try to hit asteroid
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            // left key is pressed: move missile
            missiles[1].move(-2, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            // left key is pressed: move missile
            missiles[1].move(2, 0);
        }
    
    }
    if (count == 3)
    {
        missiles[2].move(0,-3);
        //will move left and right to try to hit asteroid
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            // left key is pressed: move missile
            missiles[2].move(-2, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            // left key is pressed: move missile
            missiles[2].move(2, 0);
        }
    
    }
    return;
}

//***************************************** Asteroid Movement Function *******************************************************//

void asteroidmovement(vector<Sprite> &asteroid, uniform_int_distribution<int> &dist, mt19937 &gen, bool &firstMovement, int movement[])
    {
        //window size
        int Xend = 800;
        int Yend = 600;
        
        
        //asteroid one movement
        if (firstMovement)
        {
            movement[0] = static_cast<int>(dist(gen));
            movement[1] = static_cast<int>(dist(gen));
            
            //asteroid two movement
            movement[2] = static_cast<int>(dist(gen));
            movement[3] = static_cast<int>(dist(gen));
            
            //asteroid three movement
            movement[4] = static_cast<int>(dist(gen));
            movement[5] = static_cast<int>(dist(gen));
            
            //asteroid four movement
            movement[6] = static_cast<int>(dist(gen));
            movement[7] = static_cast<int>(dist(gen));
            
            //asteroid five movement
            movement[8] = static_cast<int>(dist(gen));
            movement[9] = static_cast<int>(dist(gen));
            
            firstMovement = false;
        }
        
        

        
        //float variables used when finding the global bounds
        FloatRect ast1, ast2, ast3, ast4, ast5;
        
        //asteroidcollision( asteroid, move);
        asteroidcollision(asteroid, movement);
        
    //moving the first asteroid: wrapping it around the screen using a series of if/else if statements
    sf::sleep(sf::milliseconds(2));
        
    //asteroidcollision( asteroid, move);
    //asteroidcollision(asteroid, move);
    asteroid[0].move(movement[0],movement[1]);
    ast1 = asteroid[0].getGlobalBounds();
    
    if (ast1.left >= Xend)
    {
        asteroid[0].setPosition(0, ast1.top);
    }
    
    else if (ast1.left <= 0)
    {
        asteroid[0].setPosition(800, ast1.top);
    }
    
    else if (ast1.top >= Yend)
    {
        asteroid[0].setPosition(ast1.left, 0);
    }
    
    else if (ast1.top <= 0)
    {
        asteroid[0].setPosition(ast1.left, 600);
    }
    
    //moving the second asteroid: wrapping it around the screen using a series of if/else if statements
    sf::sleep(sf::milliseconds(2));
    asteroid[1].move(movement[2],movement[3]);
    ast2 = asteroid[1].getGlobalBounds();
    
    if (ast2.left >= Xend)
    {
        asteroid[1].setPosition(0, ast2.top);
    }
    
    else if (ast2.left <= 0)
    {
        asteroid[1].setPosition(800, ast2.top);
    }
    
    else if (ast2.top >= Yend)
    {
        asteroid[1].setPosition(ast2.left, 0);
    }
    
    else if (ast2.top <= 0)
    {
        asteroid[1].setPosition(ast2.left, 600);
    }
    
    //moving the third asteroid: wrapping it around the screen using a series of if/else if statements
    sf::sleep(sf::milliseconds(3));
    asteroid[2].move(movement[4],movement[5]);
    ast3 = asteroid[2].getGlobalBounds();
    
    if (ast3.left >= Xend)
    {
        asteroid[2].setPosition((0), (ast3.top));
    }
    
    else if (ast3.left <= 0)
    {
        asteroid[2].setPosition((800), (ast3.top));
    }
    
    else if (ast3.top >= Yend)
    {
        asteroid[2].setPosition((ast3.left), (0));
    }
    
    else if (ast3.top <= 0)
    {
        asteroid[2].setPosition((ast3.left), (600));
    }
    
    //moving the fourth asteroid: wrapping it around the screen using a series of if/else if statements
    sf::sleep(sf::milliseconds(2));
    asteroid[3].move(movement[6], movement[7]);
    ast4 = asteroid[3].getGlobalBounds();
    
    if (ast4.left >= Xend)
    {
        asteroid[3].setPosition((0), (ast4.top));
    }
    
    else if (ast4.left <= 0)
    {
        asteroid[3].setPosition((800), (ast4.top));
    }
    
    else if (ast4.top >= Yend)
    {
        asteroid[3].setPosition((ast4.left), (0));
    }
    
    else if (ast4.top <= 0)
    {
        asteroid[3].setPosition((ast4.left), (600));
    }
    
    //moving the fifth asteroid: wrapping it around the screen using a series of if/else if statements
    sf::sleep(sf::milliseconds(1));
    asteroid[4].move(movement[8], movement[9]);
    ast5 = asteroid[4].getGlobalBounds();
    
    if (ast5.left >= Xend)
    {
        asteroid[4].setPosition((0), (ast5.top));
    }
    
    else if (ast5.left <= 0)
    {
        asteroid[4].setPosition((800), (ast5.top));
    }
    
    else if (ast5.top >= Yend)
    {
        asteroid[4].setPosition((ast5.left), (0));
    }
    
    else if (ast5.top <= 0)
    {
        asteroid[4].setPosition((ast5.left), (600));
    }
        
    return;
    }

//***************************************** Asteroid Collision Function ********************************************************//

//movement for the asteroids after the collision are set in a different function
void asteroidcollision(vector<Sprite> &asteroid, int movement[])
    {
        //collision checks
        //asteroid one check with other asteroids
        if (asteroid[0].getGlobalBounds().intersects(asteroid[1].getGlobalBounds()))
        {
            movement[0]*=-1;
            movement[1]*=-1;
        }
        if (asteroid[0].getGlobalBounds().intersects(asteroid[2].getGlobalBounds()))
        {
            movement[0]*=-1;
            movement[1]*=-1;
        }
        if (asteroid[0].getGlobalBounds().intersects(asteroid[3].getGlobalBounds()))
        {
            movement[0]*=-1;
            movement[1]*=-1;
        }
        if (asteroid[0].getGlobalBounds().intersects(asteroid[4].getGlobalBounds()))
        {
            movement[0]*=-1;
            movement[1]*=-1;
        }
        //asteroid two collision check with other asteroids
        if (asteroid[1].getGlobalBounds().intersects(asteroid[0].getGlobalBounds()))
        {
            movement[2]*=-1;
            movement[3]*=-1;
        }
        if (asteroid[1].getGlobalBounds().intersects(asteroid[2].getGlobalBounds()))
        {
            movement[2]*=-1;
            movement[3]*=-1;
        }
        if (asteroid[1].getGlobalBounds().intersects(asteroid[3].getGlobalBounds()))
        {
            movement[2]*=-1;
            movement[3]*=-1;
        }
        if (asteroid[1].getGlobalBounds().intersects(asteroid[4].getGlobalBounds()))
        {
            movement[2]*=-1;
            movement[3]*=-1;
        }
        
        //asteroid three collision check with other asteroids
        if (asteroid[2].getGlobalBounds().intersects(asteroid[0].getGlobalBounds()))
        {
            movement[4]*=-1;
            movement[5]*=-1;
        }
        if (asteroid[2].getGlobalBounds().intersects(asteroid[1].getGlobalBounds()))
        {
            movement[4]*=-1;
            movement[5]*=-1;
        }
        if (asteroid[2].getGlobalBounds().intersects(asteroid[3].getGlobalBounds()))
        {
            movement[4]*=-1;
            movement[5]*=-1;
        }
        if (asteroid[2].getGlobalBounds().intersects(asteroid[4].getGlobalBounds()))
        {
            movement[4]*=-1;
            movement[5]*=-1;
        }
        
        //asteroid four collision check with other asteroids
        if (asteroid[3].getGlobalBounds().intersects(asteroid[0].getGlobalBounds()))
        {
            movement[6]*=-1;
            movement[7]*=-1;
        }
        if (asteroid[3].getGlobalBounds().intersects(asteroid[1].getGlobalBounds()))
        {
            
            movement[6]*=-1;
            movement[7]*=-1;
        }
        if (asteroid[3].getGlobalBounds().intersects(asteroid[2].getGlobalBounds()))
        {
            movement[6]*=-1;
            movement[7]*=-1;
        }
        if (asteroid[3].getGlobalBounds().intersects(asteroid[4].getGlobalBounds()))
        {
            movement[6]*=-1;
            movement[7]*=-1;
        }
        //asteroid five collision check with other asteroids
        if (asteroid[4].getGlobalBounds().intersects(asteroid[0].getGlobalBounds()))
        {
            movement[8]*=-1;
            movement[9]*=-1;
        }
        if (asteroid[4].getGlobalBounds().intersects(asteroid[1].getGlobalBounds()))
        {
            movement[8]*=-1;
            movement[9]*=-1;
        }
        if (asteroid[4].getGlobalBounds().intersects(asteroid[2].getGlobalBounds()))
        {
            movement[8]*=-1;
            movement[9]*=-1;
        }
        if (asteroid[4].getGlobalBounds().intersects(asteroid[3].getGlobalBounds()))
        {
            movement[8]*=-1;
            movement[9]*=-1;
        }
        
        return;
    }

//**************************** Missile Collision check with Asteroids Function ******************************************************//

    //test for collision asterds with missiles
void fullcollisioncheck(vector<Sprite> &missiles, vector<Sprite> &asteroid)
    {
        //float variables used when finding the global bounds
        FloatRect miss1, miss2, miss3;
        
        //Gets position of missile 1 and draw explosion, scale the missile 1 and what ever asteroid that it hits to zero so that it disappears, set the position of the explosion to where the missile and asteroid explode
        miss1 = missiles[0].getGlobalBounds();
        if (miss1.intersects(asteroid[0].getGlobalBounds()))
        {
            missiles[0].scale(0,0);
            asteroid[0].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss1.intersects(asteroid[1].getGlobalBounds()))
        {
            missiles[0].scale(0,0);
            asteroid[1].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss1.intersects(asteroid[2].getGlobalBounds()))
        {
            missiles[0].scale(0,0);
            asteroid[2].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss1.intersects(asteroid[3].getGlobalBounds()))
        {
            missiles[0].scale(0,0);
            asteroid[3].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss1.intersects(asteroid[4].getGlobalBounds()))
        {
            missiles[0].scale(0,0);
            asteroid[4].scale(0,0);
            sleep(milliseconds(2));
        }
        
        //Gets position of missile 2 and draw explosion, scale the missile 1 and what ever asteroid that it hits to zero so that it disappears, set the position of the explosion to where the missile and asteroid explode
        miss2 = missiles[1].getGlobalBounds();
        if (miss2.intersects(asteroid[0].getGlobalBounds()))
        {
            missiles[1].scale(0,0);
            asteroid[0].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss2.intersects(asteroid[1].getGlobalBounds()))
        {
            missiles[1].scale(0,0);
            asteroid[1].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss2.intersects(asteroid[2].getGlobalBounds()))
        {
            missiles[1].scale(0,0);
            asteroid[2].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss2.intersects(asteroid[3].getGlobalBounds()))
        {
            missiles[1].scale(0,0);
            asteroid[3].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss2.intersects(asteroid[4].getGlobalBounds()))
        {
            missiles[1].scale(0,0);
            asteroid[4].scale(0,0);
            sleep(milliseconds(2));
        }
        
        //Gets position of missile 3 and draw explosion, scale the missile 1 and what ever asteroid that it hits to zero so that it disappears, set the position of the explosion to where the missile and asteroid explode
        miss3 = missiles[2].getGlobalBounds();
        if (miss3.intersects(asteroid[0].getGlobalBounds()))
        {
            missiles[2].scale(0,0);
            asteroid[0].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss3.intersects(asteroid[1].getGlobalBounds()))
        {
            missiles[2].scale(0,0);
            asteroid[1].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss3.intersects(asteroid[2].getGlobalBounds()))
        {
            missiles[2].scale(0,0);
            asteroid[2].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss3.intersects(asteroid[3].getGlobalBounds()))
        {
            missiles[2].scale(0,0);
            asteroid[3].scale(0,0);
            sleep(milliseconds(2));
        }
        
        if (miss3.intersects(asteroid[4].getGlobalBounds()))
        {
            missiles[2].scale(0,0);
            asteroid[4].scale(0,0);
            sleep(milliseconds(2));
        }
        
        return;
    }

