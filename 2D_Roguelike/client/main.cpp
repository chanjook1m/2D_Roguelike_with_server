#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <SFML\Network.hpp>

#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "random.h"
#include "ingameText.h"
#include "item.h"
#include "wall.h"
#include "animatedGIF.h"
#define RESOURCE_DIR (string)"C:\\Users\\1z3r0\\Desktop\\game\\2D_Roguelike\\Resources\\"
using namespace std;

enum types
{
    COIN = 1,
    POWERUP,
};

sf::Packet& operator <<(sf::Packet& packet, const Player& m)
{
    return packet << m.id << m.velocity << m.attackDamage << m.direction << m.x << m.y << m.hp << m.powerUpLevel << m.canMoveUp << m.canMoveDown << m.canMoveLeft << m.canMoveRight << m.isAlive << m.collisionRect_x << m.collisionRect_y << m.projectile_x << m.projectile_y << m.shooted;
}

sf::Packet& operator >>(sf::Packet& packet, Player& m)
{
    return packet >> m.id >> m.velocity >> m.attackDamage >> m.direction >> m.x >> m.y >> m.hp >> m.powerUpLevel >> m.canMoveUp >> m.canMoveDown >> m.canMoveLeft >> m.canMoveRight >> m.isAlive >> m.collisionRect_x >> m.collisionRect_y >> m.projectile_x >> m.projectile_y >> m.shooted;
}

int main()
{
    int counter;
    int counter2;

    //
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    std::string sIp = ip.toString();
    sf::TcpSocket socket;
    //socket.setBlocking(false);
    bool done = false;
    std::string id = "111";

    //std::cout << "Enter your id: ";
    //std::getline(std::cin, id);
    ////std::cin >> id;
    socket.connect(ip, 5555);
    std::vector<sf::Text> chat;
    sf::Packet packet;

    packet << id;
    socket.send(packet);
    
    std::vector <Player> enemies;
    Player player1(24, 32);
    player1.isMainPlayer = true;
    
    if (socket.receive(packet) == sf::Socket::Done)
    {
        if (packet.getDataSize() > 0)
        {
            if (packet >> player1)
            {
                std::cout << "Player ID: " << player1.id << std::endl;
                player1.collisionRect.setPosition(player1.collisionRect_x, player1.collisionRect_y);
            }
        }
    }
    socket.setBlocking(false);

    //
    sf::RenderWindow window(sf::VideoMode(1000, 800), "My RPG");
    window.setFramerateLimit(60);

    sf::View view(sf::FloatRect(200, 200, 300, 200));
    view.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    view.setCenter(sf::Vector2f(view.getSize().x/2, view.getSize().y/2));
    window.setView(view);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile(RESOURCE_DIR + "rpg_sprite_walk.png")) 
    {
        return EXIT_FAILURE;
    }

    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile(RESOURCE_DIR + "goblin.png")) 
    {
        return EXIT_FAILURE;
    }

    sf::Texture coinTexture;
    if (!coinTexture.loadFromFile(RESOURCE_DIR + "coin.png"))
    {
        return EXIT_FAILURE;
    }

    sf::Texture powerUpTexture;
    if (!powerUpTexture.loadFromFile(RESOURCE_DIR + "powerup.png"))
    {
        return EXIT_FAILURE;
    }

    sf::Texture wallTexture;
    if (!wallTexture.loadFromFile(RESOURCE_DIR + "wall.png"))
    {
        return EXIT_FAILURE;
    }

    AnimatedGIF gif((RESOURCE_DIR + "energyball.gif").c_str());
    sf::Sprite energyBallSprite;
    energyBallSprite.setScale(sf::Vector2f(0.1, 0.1));

    sf::Font maumFont;
    if (!maumFont.loadFromFile(RESOURCE_DIR + "godoMaum.ttf")) 
    {
        return EXIT_FAILURE;
    }

    sf::SoundBuffer shotBuffer;
    if (!shotBuffer.loadFromFile(RESOURCE_DIR + "shot.ogg"))
    {
        return EXIT_FAILURE;
    }
    sf::Sound shotSound;
    shotSound.setBuffer(shotBuffer);
    
    sf::SoundBuffer collisionBuffer;
    if (!collisionBuffer.loadFromFile(RESOURCE_DIR + "collision.ogg"))
    {
        return EXIT_FAILURE;
    }
    sf::Sound collisionSound;
    collisionSound.setBuffer(collisionBuffer);

    sf::SoundBuffer wallCollisionBuffer;
    if (!wallCollisionBuffer.loadFromFile(RESOURCE_DIR + "wall_collision.ogg"))
    {
        return EXIT_FAILURE;
    }
    sf::Sound wallCollisionSound;
    wallCollisionSound.setBuffer(wallCollisionBuffer);

    sf::SoundBuffer coinSoundBuffer;
    if (!coinSoundBuffer.loadFromFile(RESOURCE_DIR + "coin.ogg"))
    {
        return EXIT_FAILURE;
    }
    sf::Sound coinSound;
    coinSound.setBuffer(coinSoundBuffer);

    sf::SoundBuffer hitBuffer;
    if (!hitBuffer.loadFromFile(RESOURCE_DIR + "hit.ogg"))
    {
        return EXIT_FAILURE;
    }
    sf::Sound hitSound;
    hitSound.setBuffer(hitBuffer);

    sf::SoundBuffer powerUpSoundBuffer;
    if (!powerUpSoundBuffer.loadFromFile(RESOURCE_DIR + "powerup.ogg"))
    {
        return EXIT_FAILURE;
    }
    sf::Sound powerUpSound;
    powerUpSound.setBuffer(powerUpSoundBuffer);

    sf::SoundBuffer backgroundBuffer;
    if (!backgroundBuffer.loadFromFile(RESOURCE_DIR + "background.ogg"))
    {
        return EXIT_FAILURE;
    }
    sf::Sound backgroundSound;
    backgroundSound.setBuffer(backgroundBuffer);

    backgroundSound.setLoop(true);
    backgroundSound.setVolume(20.f);
    backgroundSound.play();

    //

    player1.sprite.setTexture(playerTexture);
    

    vector<Projectile>::const_iterator projectileIter;
    vector<Projectile> projectileArr;
    Projectile projectile;
    projectile.sprite = energyBallSprite;

    vector<Enemy>::const_iterator enemyIter;
    vector<Enemy> enemyArr;

    Enemy enemy(0,0, 48, 48);
    enemy.sprite.setTexture(enemyTexture);
    enemy.text.setFont(maumFont);
    enemy.text.setFillColor(sf::Color::Red);
    //enemyArr.push_back(enemy);

    

    vector<Item>::const_iterator itemIter;
    vector<Item> itemArr;

    Item item(10,10, 200, 150, COIN);
    item.sprite.setTexture(coinTexture);
    item.sprite.setScale(sf::Vector2f(0.2, 0.2));
    //item.collisionRect.setPosition(500, 500);
    //itemArr.push_back(item);

    vector<Wall>::const_iterator wallIter;
    vector<Wall> wallArr;
    Wall wall(0,32, 18,18);

    wall.sprite.setTextureRect(sf::IntRect(0, 0, 18, 18));
    wall.sprite.setTexture(wallTexture);
    wall.sprite.setScale(3.0, 3.0);

    // create room
    int roomSize = 10;
    int bossRoomSize = 15;
    int verticalDoorAt = 2;
    int horizontalDoorAt = 2;
    int initialRoomX = 200;
    int initialRoomY = 300;
    
    counter = 0;
    while (counter < roomSize)
    {
        // first room
        wall.collisionRect.setPosition(50 * counter + initialRoomX, initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(50 * counter + initialRoomX, 50 * roomSize + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(initialRoomX, 50 * counter + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(50 * roomSize + initialRoomX, 50 * counter + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);

        // second room
        wall.collisionRect.setPosition(50 * counter + initialRoomX + (roomSize * 50) + 50, initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(50 * counter + initialRoomX + (roomSize * 50) + 50, 50 * roomSize + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(initialRoomX + (roomSize * 50) + 50, 50 * counter + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(50 * roomSize + initialRoomX + (roomSize * 50) + 50, 50 * counter + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);

        // third room
        wall.collisionRect.setPosition(50 * counter + initialRoomX + (roomSize * 50 * 2) + 50, initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(50 * counter + initialRoomX + (roomSize * 50 * 2) + 50, 50 * roomSize + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(initialRoomX + (roomSize * 50 * 2) + 50, 50 * counter + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(50 * roomSize + initialRoomX + (roomSize * 50 * 2) + 50, 50 * counter + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        
        counter++;
    }

    counter = 0;
    while (counter < bossRoomSize)
    {
        wall.collisionRect.setPosition(50 * counter + initialRoomX + (bossRoomSize * 50 * 2) + 50, initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(50 * counter + initialRoomX + (bossRoomSize * 50 * 2) + 50, 50 * bossRoomSize + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(initialRoomX + (bossRoomSize * 50 * 2) + 50, 50 * counter + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(50 * bossRoomSize + initialRoomX + (bossRoomSize * 50 * 2) + 50, 50 * counter + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
        wallArr.push_back(wall);
        counter++;
    }

    // generate shop item
    item = Item(0, 0, 100, 100, POWERUP);
    item.inShop = true;
    item.cost = 10;
    item.text.setFont(maumFont);
    item.text.setFillColor(sf::Color::Green);
    item.text.setString("Cost: " + to_string(item.cost));
    item.sprite.setTexture(powerUpTexture);
    item.collisionRect.setPosition((50 * roomSize / 2 + initialRoomX + (roomSize * 50 * 2) + 50), 50 * roomSize / 2 + initialRoomY);
    itemArr.push_back(item);
    item.inShop = false;
    

    // create wall and enemy in room
    counter = 1;
    while (counter < roomSize - 2)
    {
        counter2 = 1;
        while (counter2 < roomSize - 2)
        {
            if (counter != counter2)
            {
                int tempRandom = generateRandom(7);

                if (tempRandom == 1)
                {
                    // first room
                    wall.destructible = true;
                    wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
                    wall.collisionRect.setPosition((counter * 50) + 100 + initialRoomX, (counter2 * 50) + 100 + initialRoomY);
                    wallArr.push_back(wall);

                    // second room
                    wall.destructible = true;
                    wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
                    wall.collisionRect.setPosition(50 * counter + initialRoomX + (roomSize * 50) + 150, (counter2 * 50) + 100 + initialRoomY);
                    wallArr.push_back(wall);
                }
                else if (tempRandom == 2)
                {
                    // first room
                    //enemy.collisionRect.setPosition((counter * 50) + 100 + initialRoomX, (counter2 * 50) + 100 + initialRoomY);
                    //enemyArr.push_back(enemy);

                    //// second room
                    //enemy.collisionRect.setPosition(50 * counter + initialRoomX + (roomSize * 50) + 150, (counter2 * 50) + 100 + initialRoomY);
                    //enemyArr.push_back(enemy);
                }
            }
            counter2++;
        }
        counter++;
    }

    // create enemy and wall in boss room

    counter = 0;
    while (counter < bossRoomSize - 1)
    {
        counter2 = 0;
        while (counter2 < bossRoomSize - 1)
        {
            int tempRandom = generateRandom(40);

            if (tempRandom == 1)
            {
                wall.destructible = true;
                wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
                wall.collisionRect.setPosition(50 * counter + initialRoomX + (bossRoomSize * 50 * 2) + 100, (counter2 * 50) + 50 + initialRoomY);
                wallArr.push_back(wall);
            }
            else if (tempRandom == 2)
            {
                //enemy.collisionRect.setPosition(50 * counter + initialRoomX + (bossRoomSize * 50 * 2) + 100, (counter2 * 50) + 50 + initialRoomY);
                //enemyArr.push_back(enemy);
            }
            counter2++;
        }
        counter++;
    }

    Enemy boss(48 * 6, 0, 48, 48);
    boss.sprite.setTexture(enemyTexture);
    boss.text.setFont(maumFont);
    boss.maxHp = 100;
    boss.hp = 100;
    boss.attackDamage = 10;
    boss.collisionRect.setPosition(50 * bossRoomSize/2 + initialRoomX + (bossRoomSize * 50 * 2) + 100, (bossRoomSize/2 * 50) + 50 + initialRoomY);
    boss.text.setFillColor(sf::Color::Red);
    //enemyArr.push_back(boss);

    vector<IngameText>::const_iterator ingameTextIter;
    vector<IngameText> ingameTextArr;

    IngameText ingameText;
    ingameText.text.setPosition(200, 200);
    ingameText.text.setFont(maumFont);
    ingameTextArr.push_back(ingameText);

    sf::Text hpText("Money: ", maumFont, 50);
    hpText.setPosition(50, 50);

    sf::Text scoreText("Money: ", maumFont, 50);
    scoreText.setPosition(50, 50);

    sf::Text powerUpText("Power Level: ", maumFont, 50);
    powerUpText.setPosition(50, 100);

    sf::Clock projectileClock;
    sf::Clock playerCollisionClock;
    sf::Clock aggroClock;

    
    
    
    // run the program as long as the window is open
    bool enemyUpdate = false;
    bool update = false;
    while (window.isOpen())
    {
        // receive update game packet
        sf::Packet updatePacket;
        sf::Socket::Status status = socket.receive(updatePacket);
        if (status == sf::Socket::Done)

        {
            //std::cout << " Done" << std::endl;
            if (updatePacket.getDataSize() > 0)
            {
                Player player(24, 32);
                Player enem(24, 32);
                updatePacket >> player;
                
                std::cout << player.id << " and " << player1.id << std::endl;
                //std::cout << player.collisionRect_x << " and " << player.collisionRect_y << std::endl;
                if (player.id == player1.id)
                {
                    std::cout << "player update" << std::endl;
                    player1.hp = player.hp;
                    player1.x = player.x;
                    player1.y = player.y;
                    player1.powerUpLevel = player.powerUpLevel;
                    player1.direction = player.direction;
                    player1.score = player.score;
                    player1.canMoveUp = player.canMoveUp;
                    player1.canMoveDown = player.canMoveDown;
                    player1.canMoveLeft = player.canMoveLeft;
                    player1.canMoveRight = player.canMoveRight;
                    player1.projectile_x = player.projectile_x;
                    player1.projectile_y = player.projectile_y;
                    player1.collisionRect.setPosition(player.collisionRect_x, player.collisionRect_y);
                    player1.sprite.setPosition(player1.collisionRect.getPosition());

                    if (player.shooted)
                    {
                        std::cout << "shooted1" << std::endl;
                        counter = 0;
                        projectile.sprite = energyBallSprite;
                        projectile.id = player.id;
                        while (counter < player.powerUpLevel)
                        {
                            projectile.collisionRect.setPosition(
                                player.collisionRect.getPosition().x + counter * generateRandom(10),
                                player.collisionRect.getPosition().y + counter * generateRandom(10));
                            projectile.direction = player.direction;
                            projectileArr.push_back(projectile);
                            counter++;
                        }
                    }
                }
                else
                {
                    for (size_t i = 0; i < enemies.size(); i++)
                    {
                        if (enemies[i].id == player.id)
                        {
                            std::cout << "enemy update" << std::endl;
                            enemies[i].hp = player.hp;
                            enemies[i].x = player.x;
                            enemies[i].y = player.y;
                            enemies[i].powerUpLevel = player.powerUpLevel;
                            enemies[i].direction = player.direction;
                            enemies[i].score = player.score;
                            enemies[i].canMoveUp = player.canMoveUp;
                            enemies[i].canMoveDown = player.canMoveDown;
                            enemies[i].canMoveLeft = player.canMoveLeft;
                            enemies[i].canMoveRight = player.canMoveRight;
                            enemies[i].projectile_x = player.projectile_x;
                            enemies[i].projectile_y = player.projectile_y;
                            enemies[i].collisionRect.setPosition(player.collisionRect_x, player.collisionRect_y);
                            enemyUpdate = true;

                            if (player.shooted)
                            {
                                counter = 0;
                                projectile.sprite = energyBallSprite;
                                projectile.id = player.id;
                                while (counter < player.powerUpLevel)
                                {
                                    projectile.collisionRect.setPosition(
                                        player.collisionRect.getPosition().x + counter * generateRandom(10),
                                        player.collisionRect.getPosition().y + counter * generateRandom(10));
                                    projectile.direction = player.direction;
                                    projectileArr.push_back(projectile);
                                    counter++;
                                }
                            }
                        }
                    }

                    if (enemyUpdate == false)
                    {
                        //std::cout << "enemyUpdate " << player.id << std::endl;
                        enem = player;
                        enem.sprite.setTexture(playerTexture);
                        enemies.push_back(enem);

                    }
                    enemyUpdate = false;
                }   

            }
        }
        //else if (status == sf::Socket::NotReady)
        //    // ok, data received
        //    std::cout << "Not Ready" << std::endl;
        //else if (status == sf::Socket::Error)
        //    // error
        //    std::cout << "Error" << std::endl;
        //else if (status == sf::Socket::Disconnected)
        //    // disconnected
        //    std::cout << "Disconnected" << std::endl;
        
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::GainedFocus)
                update = true;
            else if (event.type == sf::Event::LostFocus)
                update = false;
        }

        window.clear();
        //
        sf::Time projectileClockElapsed = projectileClock.getElapsedTime();
        sf::Time playerCollisionClockElapsed = playerCollisionClock.getElapsedTime();
        sf::Time aggroClockElapsed = aggroClock.getElapsedTime();

        
        // projectile-enemy collision
        counter = 0;
        for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
        {
            counter2 = 0;
            for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
            {
                if (projectileArr[counter].collisionRect.getGlobalBounds().intersects(enemyArr[counter2].collisionRect.getGlobalBounds()))
                { 
                    collisionSound.play();
                    projectileArr[counter].isAlive = false;

                    ingameText.text.setFillColor(sf::Color::Red);
                    ingameText.text.setString(to_string((int)projectileArr[counter].attackDamage));
                    ingameText.text.setPosition(enemyArr[counter2].collisionRect.getPosition().x + enemyArr[counter2].collisionRect.getSize().x/2
                        , enemyArr[counter2].collisionRect.getPosition().y - enemyArr[counter2].collisionRect.getSize().y / 2);
                    ingameTextArr.push_back(ingameText);
                        
                    enemyArr[counter2].hp -= projectileArr[counter].attackDamage;
                    if (enemyArr[counter2].hp <= 0)
                    {
                        enemyArr[counter2].isAlive = false;
                        enemyArr[counter2].aggroedBy = 0;
                    }
                    
                }
                counter2++;
            }
            counter++;
        }

        // player-enemy collision && enemy aggro
        if (playerCollisionClockElapsed.asSeconds() >= 0.5)
        {
            playerCollisionClock.restart();
            counter = 0;
            for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
            {
                if (abs(player1.collisionRect.getPosition().y - enemyArr[counter].collisionRect.getPosition().y) <= 5 ||
                    abs(player1.collisionRect.getPosition().x - enemyArr[counter].collisionRect.getPosition().x) <= 5)
                {
                    enemyArr[counter].aggroedBy = 1;
                }
                else
                {
                    enemyArr[counter].aggroedBy = 0;
                }
                if (player1.collisionRect.getGlobalBounds().intersects(enemyArr[counter].collisionRect.getGlobalBounds()))
                {
                    hitSound.play();
                    player1.hp -= enemyArr[counter].attackDamage;

                    if (player1.powerUpLevel > 1)
                    {
                        player1.powerUpLevel--;
                    }

                    ingameText.text.setFillColor(sf::Color::Yellow);
                    ingameText.text.setString(to_string((int)enemyArr[counter].attackDamage));
                    ingameText.text.setPosition(player1.collisionRect.getPosition().x + player1.collisionRect.getSize().x / 2,
                        player1.collisionRect.getPosition().y - player1.collisionRect.getSize().y / 2);
                    ingameTextArr.push_back(ingameText);
                }
                counter++;
            }
        }

        // player-item collision
        counter = 0;
        for (itemIter = itemArr.begin(); itemIter != itemArr.end(); itemIter++)
        {
            if (player1.collisionRect.getGlobalBounds().intersects(itemArr[counter].collisionRect.getGlobalBounds()))
            {
                if (itemArr[counter].inShop == false)
                {
                    if (itemArr[counter].type == COIN)
                    {
                        coinSound.play();
                        player1.score += 20;
                    }
                    else if (itemArr[counter].type == POWERUP)
                    {
                        powerUpSound.play();
                        if (player1.powerUpLevel < player1.maxPowerUpLevel)
                        {
                            player1.powerUpLevel++;
                        }
                    }
                    itemArr[counter].isAlive = false;
                }
                else
                {
                    if (player1.score >= itemArr[counter].cost && player1.powerUpLevel < 5)
                    {
                        powerUpSound.play();
                        int num = player1.score / itemArr[counter].cost;
                        
                        if (player1.powerUpLevel + num >= 5)
                        {
                            num = 5 - player1.powerUpLevel;
                            player1.powerUpLevel = 5;
                        } 
                        else
                        {
                            player1.powerUpLevel += num;
                        }
                        player1.score -= itemArr[counter].cost * num;
                        
                    }
                }
            }
            counter++;
        }

        // player-wall collision
        counter = 0;
        for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
        {
            if (player1.collisionRect.getGlobalBounds().intersects(wallArr[counter].collisionRect.getGlobalBounds()))
            {
                if (player1.direction == 1)
                {
                    player1.canMoveUp = false;
                    player1.collisionRect.move(0, player1.velocity);
                }
                else if (player1.direction == 2)
                {
                    player1.canMoveDown = false;
                    player1.collisionRect.move(0, -player1.velocity);
                }
                else if (player1.direction == 3)
                {
                    player1.canMoveLeft = false;
                    player1.collisionRect.move(player1.velocity, 0);
                }
                else if (player1.direction == 4)
                {
                    player1.canMoveRight = false;
                    player1.collisionRect.move(-player1.velocity, 0);
                }
            }
            counter++;
        }

        // enemy-wall collision
        counter = 0;
        for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
        {
            counter2 = 0;
            for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
            {
                if (enemyArr[counter].collisionRect.getGlobalBounds().intersects(wallArr[counter2].collisionRect.getGlobalBounds()))
                {
                    if (enemyArr[counter].direction == 1)
                    {
                        enemyArr[counter].canMoveUp = false;
                        enemyArr[counter].collisionRect.move(0, enemyArr[counter].velocity);
                    }
                    else if (enemyArr[counter].direction == 2)
                    {
                        enemyArr[counter].canMoveDown = false;
                        enemyArr[counter].collisionRect.move(0, -enemyArr[counter].velocity);
                    }
                    else if (enemyArr[counter].direction == 3)
                    {
                        enemyArr[counter].canMoveLeft = false;
                        enemyArr[counter].collisionRect.move(enemyArr[counter].velocity, 0);
                    }
                    else if (enemyArr[counter].direction == 4)
                    {
                        enemyArr[counter].canMoveRight = false;
                        enemyArr[counter].collisionRect.move(-enemyArr[counter].velocity, 0);
                    }
                }

                counter2++;
            }
            counter++;
        }

        // projectile-wall collision
        counter = 0;
        for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
        {
            counter2 = 0;
            for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
            {
                if (projectileArr[counter].collisionRect.getGlobalBounds().intersects(wallArr[counter2].collisionRect.getGlobalBounds()))
                {
                    
                    if (wallArr[counter2].destructible == true)
                    {
                        wallCollisionSound.play(); 
                        wallArr[counter2].hp -= projectileArr[counter].attackDamage;

                        if (wallArr[counter2].hp <= 0)
                        {
                            wallArr[counter2].isAlive = false;
                        }
                    }

                    projectileArr[counter].isAlive = false;
                }

                counter2++;
            }
            counter++;
        }


        // delete not alive enemy
        counter = 0;
        for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
        {
            if (enemyArr[counter].isAlive == false)
            {
                // generate item           
                if (generateRandom(4) == 1)
                {
                    item = Item(10, 10, 200, 150, COIN);
                    item.sprite.setTexture(coinTexture);
                    item.collisionRect.setPosition(enemyArr[counter].collisionRect.getPosition());
                    itemArr.push_back(item);
                } 
                else if (generateRandom(4) == 2)
                {
                    
                    item = Item(0, 0, 100, 100, POWERUP);
                    item.sprite.setTexture(powerUpTexture);
                    item.collisionRect.setPosition(enemyArr[counter].collisionRect.getPosition());
                    itemArr.push_back(item);
                }

                enemyArr.erase(enemyIter);
                break;
            }
            counter++;
        }

        // delete not alive projectile
        counter = 0;
        for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
        {
            if (projectileArr[counter].isAlive == false)
            {
                projectileArr.erase(projectileIter);
                break;
            }
            counter++;
        }

        // delete not alive ingameText
        counter = 0;
        for (ingameTextIter = ingameTextArr.begin(); ingameTextIter != ingameTextArr.end(); ingameTextIter++)
        {
            if (ingameTextArr[counter].isAlive == false)
            {
                ingameTextArr.erase(ingameTextIter);
                break;
            }
            counter++;
        }

        // delete not alive item
        counter = 0;
        for (itemIter = itemArr.begin(); itemIter != itemArr.end(); itemIter++)
        {
            if (itemArr[counter].isAlive == false)
            {
                itemArr.erase(itemIter);
                break;
            }
            counter++;
        }

        // delete not alive wall
        counter = 0;
        for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
        {
            if (wallArr[counter].isAlive == false)
            {
                // generate item
                if (generateRandom(4) == 1)
                {
                    item = Item(10, 10, 200, 150, COIN);
                    item.sprite.setTexture(coinTexture);
                    item.collisionRect.setPosition(wallArr[counter].collisionRect.getPosition());
                    itemArr.push_back(item);
                }
                else if (generateRandom(4) == 2)
                {
                    
                    item = Item(0, 0, 100, 100, POWERUP);
                    item.sprite.setTexture(powerUpTexture);
                    item.collisionRect.setPosition(wallArr[counter].collisionRect.getPosition());
                    itemArr.push_back(item);
                }

                wallArr.erase(wallIter);
                break;
            }
            counter++;
        }

        // create enemy (c-Key)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            enemy.collisionRect.setPosition(generateRandom(window.getSize().x), generateRandom(window.getSize().y));
            enemyArr.push_back(enemy);
        }

        // create projectile (space-Key)
        if (update)
        {
            counter = 0;
            if (projectileClockElapsed.asSeconds() >= 0.2 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                shotSound.play();
                projectileClock.restart();
                sf::Packet shootPacket;
                player1.shooted = true;
                shootPacket << player1;
                //std::cout << player1.id << " : " << player1.collisionRect_x << " and " << player1.collisionRect_y << std::endl;
                socket.send(shootPacket);
                player1.updated = false;
                /*projectile.sprite = energyBallSprite;

                while (counter < player1.powerUpLevel)
                {
                    projectile.collisionRect.setPosition(
                        player1.collisionRect.getPosition().x + counter * generateRandom(10),
                        player1.collisionRect.getPosition().y + counter * generateRandom(10));
                    projectile.direction = player1.direction;
                    projectileArr.push_back(projectile);
                    counter++;
                }*/
            }
        }

        // enemy AI
        counter = 0;
        for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
        {
            if (enemyArr[counter].aggroedBy == 1)
            {
                if (aggroClockElapsed.asSeconds() > 1)
                {
                    aggroClock.restart();
                }

                int actionNumber = generateRandom(3);

                switch (actionNumber) 
                {
                case 1:
                    if (player1.collisionRect.getPosition().x < enemyArr[counter].collisionRect.getPosition().x &&
                        player1.collisionRect.getPosition().y - enemyArr[counter].collisionRect.getPosition().y <= 40)
                    {
                        enemyArr[counter].direction = 3;
                    }
                    if (player1.collisionRect.getPosition().x > enemyArr[counter].collisionRect.getPosition().x &&
                        player1.collisionRect.getPosition().y - enemyArr[counter].collisionRect.getPosition().y <= 40)
                    {
                        enemyArr[counter].direction = 4;
                    }
                    if (player1.collisionRect.getPosition().y < enemyArr[counter].collisionRect.getPosition().y &&
                        player1.collisionRect.getPosition().x - enemyArr[counter].collisionRect.getPosition().x <= 40)
                    {
                        enemyArr[counter].direction = 1;
                    }
                    if (player1.collisionRect.getPosition().y > enemyArr[counter].collisionRect.getPosition().y &&
                        player1.collisionRect.getPosition().x - enemyArr[counter].collisionRect.getPosition().x <= 40)
                    {
                        enemyArr[counter].direction = 2;
                    }
                    break;
                case 2:
                    break;
                default:
                    break;
                }
            }
            counter++;
        }

        // draw projectile
        counter = 0;
        for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
        {
            for (size_t i = 0; i < enemies.size(); i++)
            {
                if (enemies[i].id == projectileArr[counter].id)
                {
                    projectileArr[counter].update();
                    projectileArr[counter].collisionRect.setPosition(enemies[i].projectile_x, enemies[i].projectile_y);
                    // draw eneryball
                    gif.update(projectileArr[counter].sprite);
                    window.draw(projectileArr[counter].sprite);
                }
            }

            if (player1.id == projectileArr[counter].id)
            {
                //std::cout << "shooted2: " << player1.projectile_x << " : " << player1.projectile_y << std::endl;
                projectileArr[counter].collisionRect.setPosition(player1.projectile_x, player1.projectile_y);
                projectileArr[counter].sprite.setPosition(projectileArr[counter].collisionRect.getPosition());
                //projectileArr[counter].update();
                gif.update(projectileArr[counter].sprite);
                window.draw(projectileArr[counter].sprite);
            }
            counter++;
        }

        // draw enemy
        counter = 0;
        for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
        {
            enemyArr[counter].text.setString("HP " + to_string(enemyArr[counter].hp) + "/" + to_string(enemyArr[counter].maxHp));
            window.draw(enemyArr[counter].text);
            window.draw(enemyArr[counter].sprite);
            enemyArr[counter].update();
            counter++;
        }

        // draw wall
        counter = 0;
        for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
        {
            wallArr[counter].update();
            window.draw(wallArr[counter].sprite);
            counter++;
        }

        // view player
        window.setView(view);
        view.setCenter(player1.collisionRect.getPosition());

        // draw player , scoreText
        window.draw(player1.sprite);

        for (size_t i = 0; i < enemies.size(); i++)
        {
            if (!update)
                enemies[i].update();
            window.draw(enemies[i].sprite);
        }
        if (update)
        {
            player1.update();
            if (player1.updated == true)
            {
                    sf::Packet movePacket;
                    movePacket << player1;
                    std::cout << player1.id << " : " << player1.collisionRect_x << " and " << player1.collisionRect_y << std::endl;
                    socket.send(movePacket);
                    player1.updated = false;
            }
        }
        

        scoreText.setString("Money: " + to_string(player1.score));
        window.draw(scoreText);
        scoreText.setPosition(player1.collisionRect.getPosition().x - window.getSize().x/2, 
            player1.collisionRect.getPosition().y - window.getSize().y/2);
        
        powerUpText.setString("Power Level: " + to_string(player1.powerUpLevel));
        window.draw(powerUpText);
        powerUpText.setPosition(player1.collisionRect.getPosition().x - window.getSize().x / 2,
            player1.collisionRect.getPosition().y - window.getSize().y / 2 + 50);

        hpText.setString("HP: " + to_string(player1.hp) + "/" + to_string(player1.maxHp));
        window.draw(hpText);
        hpText.setPosition(player1.collisionRect.getPosition().x - window.getSize().x / 2,
            player1.collisionRect.getPosition().y - window.getSize().y / 2 + 100);
        
        
        // draw ingameText
        counter = 0;
        for (ingameTextIter = ingameTextArr.begin(); ingameTextIter != ingameTextArr.end(); ingameTextIter++)
        {
            ingameTextArr[counter].update();
            window.draw(ingameTextArr[counter].text);
            counter++;
        }

        // draw item
        counter = 0;
        for (itemIter = itemArr.begin(); itemIter != itemArr.end(); itemIter++)
        {
            if (itemArr[counter].inShop == true)
            {
                window.draw(itemArr[counter].text);
            }
            itemArr[counter].update();
            window.draw(itemArr[counter].sprite);
            counter++;
        }

        
        //
        window.display();
    }
    return 0;
}