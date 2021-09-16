#include "GamePlay.hpp"

#include <SFML/Window/Event.hpp>

#include <stdlib.h>
#include <time.h>

#define RESOURCE_DIR (string)"C:\\Users\\1z3r0\\Desktop\\game\\2D_Roguelike\\Resources\\"

enum types
{
    COIN = 1,
    POWERUP,
};
bool update = false;
Player player1(24, 32);
int counter;
int counter2;
static int request_id = 1;

sf::View view(sf::FloatRect(200, 200, 300, 200));

std::vector <Player> enemies;
vector<Player>::const_iterator playerIter;
bool enemyUpdate = false;
Player enem(24, 32);
Enemy enemy(0, 0, 48, 48);

vector<Projectile>::const_iterator projectileIter;
vector<Projectile> projectileArr;
Projectile projectile;

vector<Enemy>::const_iterator enemyIter;
vector<Enemy> enemyArr;

vector<Item>::const_iterator itemIter;
vector<Item> itemArr;
Item item(10, 10, 200, 150, COIN);

vector<Wall>::const_iterator wallIter;
vector<Wall> wallArr;
Wall wall(0, 32, 18, 18);

vector<IngameText>::const_iterator ingameTextIter;
vector<IngameText> ingameTextArr;

IngameText ingameText;

sf::Font maumFont;
sf::Texture playerTexture;
sf::Texture enemyTexture;
sf::Texture coinTexture;
sf::Texture powerUpTexture;
sf::Texture wallTexture;
sf::Sprite energyBallSprite;
AnimatedGIF gif((RESOURCE_DIR + "energyball.gif").c_str());

sf::Text hpText("Money: ", maumFont, 50);
sf::Text scoreText("Money: ", maumFont, 50);
sf::Text powerUpText("Power Level: ", maumFont, 50);


sf::Clock projectileClock;
sf::Clock playerCollisionClock;
sf::Clock aggroClock;

static net::AsyncTCPClient client;

GamePlay::GamePlay(std::shared_ptr<Context>& context)
    : m_context(context)
{
    srand(time(nullptr));
}

GamePlay::~GamePlay()
{
}

void GamePlay::Init()
{
    
    bool done = false;


    

    
    maumFont.loadFromFile(RESOURCE_DIR + "godoMaum.ttf");
    

    
    playerTexture.loadFromFile(RESOURCE_DIR + "rpg_sprite_walk.png");
    

    
    player1.text.setFont(maumFont);
    player1.text.setPosition(295, 370);
    player1.isMainPlayer = true;
    player1.sprite.setTexture(playerTexture);

    // client
    static int request_id = 1;


    

    //client.WriteOperation(5, "127.0.0.1", 5555, net::handler, player1.id, "", 0);

    //std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "[INFO] connection status: " << (net::connected ? "connected" : "not connected") << std::endl;
    if (net::connected)
    {
        player1.id = net::id;
    }


    std::this_thread::sleep_for(std::chrono::milliseconds(5));



    //client.WriteOperation(5, "127.0.0.1", 5555, net::handler, player1.id, 1);
    //


    

    
    view.setSize(sf::Vector2f(m_context->m_window->getSize().x, m_context->m_window->getSize().y));
    view.setCenter(sf::Vector2f(view.getSize().x / 2, view.getSize().y / 2));
    m_context->m_window->setView(view);

    if (STRESS_TEST == true)
        m_context->m_window->setVisible(false);




    
    enemyTexture.loadFromFile(RESOURCE_DIR + "goblin.png");
    
    
    coinTexture.loadFromFile(RESOURCE_DIR + "coin.png");
    

    
    powerUpTexture.loadFromFile(RESOURCE_DIR + "powerup.png");
    

    
    wallTexture.loadFromFile(RESOURCE_DIR + "wall.png");
    

    
    sf::Sprite energyBallSprite;
    energyBallSprite.setScale(sf::Vector2f(0.1, 0.1));



    sf::SoundBuffer shotBuffer;
    shotBuffer.loadFromFile(RESOURCE_DIR + "shot.ogg");
    
    sf::Sound shotSound;
    shotSound.setBuffer(shotBuffer);

    sf::SoundBuffer collisionBuffer;
    collisionBuffer.loadFromFile(RESOURCE_DIR + "collision.ogg");
    
    sf::Sound collisionSound;
    collisionSound.setBuffer(collisionBuffer);

    sf::SoundBuffer wallCollisionBuffer;
    wallCollisionBuffer.loadFromFile(RESOURCE_DIR + "wall_collision.ogg");
    
    sf::Sound wallCollisionSound;
    wallCollisionSound.setBuffer(wallCollisionBuffer);

    sf::SoundBuffer coinSoundBuffer;
    coinSoundBuffer.loadFromFile(RESOURCE_DIR + "coin.ogg");
    
    sf::Sound coinSound;
    coinSound.setBuffer(coinSoundBuffer);

    sf::SoundBuffer hitBuffer;
    hitBuffer.loadFromFile(RESOURCE_DIR + "hit.ogg");
    
    sf::Sound hitSound;
    hitSound.setBuffer(hitBuffer);

    sf::SoundBuffer powerUpSoundBuffer;
    powerUpSoundBuffer.loadFromFile(RESOURCE_DIR + "powerup.ogg");
    
    sf::Sound powerUpSound;
    powerUpSound.setBuffer(powerUpSoundBuffer);

    sf::SoundBuffer backgroundBuffer;
    backgroundBuffer.loadFromFile(RESOURCE_DIR + "background.ogg");
 
    sf::Sound backgroundSound;
    backgroundSound.setBuffer(backgroundBuffer);

    backgroundSound.setLoop(true);
    backgroundSound.setVolume(20.f);
    backgroundSound.play();

    // objects and objects array

    

    projectile.sprite = energyBallSprite;

    

    
    enemy.sprite.setTexture(enemyTexture);
    enemy.text.setFont(maumFont);
    enemy.text.setFillColor(sf::Color::Red);

    

    
    item.sprite.setTexture(coinTexture);
    item.sprite.setScale(sf::Vector2f(0.2, 0.2));
    
    

    wall.sprite.setTextureRect(sf::IntRect(0, 0, 18, 18));
    wall.sprite.setTexture(wallTexture);
    wall.sprite.setScale(3.0, 3.0);

    //// create room
    //int roomSize = 10;
    //int bossRoomSize = 15;
    //int verticalDoorAt = 2;
    //int horizontalDoorAt = 2;
    //int initialRoomX = 200;
    //int initialRoomY = 300;

    //// generate shop item
    //item = Item(0, 0, 100, 100, POWERUP);

    Enemy boss(48 * 6, 0, 48, 48);
    boss.sprite.setTexture(enemyTexture);
    boss.text.setFont(maumFont);
    boss.text.setFillColor(sf::Color::Red);
    /*boss.maxHp = 100;
    boss.hp = 100;
    boss.attackDamage = 10;
    boss.collisionRect.setPosition(50 * bossRoomSize/2 + initialRoomX + (bossRoomSize * 50 * 2) + 100, (bossRoomSize/2 * 50) + 50 + initialRoomY);*/

    //enemyArr.push_back(boss);

    
    ingameText.text.setPosition(200, 200);
    ingameText.text.setFont(maumFont);
    ingameTextArr.push_back(ingameText);

    
    hpText.setPosition(50, 50);
    
    scoreText.setPosition(50, 50);

    
    powerUpText.setPosition(50, 100);

    

    sf::Time projectileClockElapsed = projectileClock.getElapsedTime();
    sf::Time playerCollisionClockElapsed = playerCollisionClock.getElapsedTime();
    sf::Time aggroClockElapsed = aggroClock.getElapsedTime();

    
    
    
    enem.text.setFont(maumFont);
}

void GamePlay::ProcessInput()
{
    sf::Event event;
    while (m_context->m_window->pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            m_context->m_window->close();
        else if (event.type == sf::Event::GainedFocus)
            update = true;
        else if (event.type == sf::Event::LostFocus)
            update = false;
    }
}

void GamePlay::Update(sf::Time deltaTime)
{
    client.ReadOperation(10, "127.0.0.1", 5555, net::handler, request_id++);

    client.WriteOperation(5, "127.0.0.1", 5555, net::handler, player1.id, "", 1);

    std::this_thread::sleep_for(std::chrono::milliseconds(33));

    std::cout << "[INFO] PLAYER ID : " << player1.id << std::endl;

    // update players
    for (int i = 0; i < net::players.size(); i++)
    {
        //std::cout << "ID2 : " << net::players[i].id << std::endl;
        if (net::players[i].id == player1.id)
        {
            player1.isAlive = net::players[i].isAlive;
            player1.hp = net::players[i].hp;
            player1.x = net::players[i].x;
            player1.y = net::players[i].y;
            player1.powerUpLevel = net::players[i].powerUpLevel;
            player1.direction = net::players[i].direction;
            player1.score = net::players[i].score;
            player1.canMoveUp = net::players[i].canMoveUp;
            player1.canMoveDown = net::players[i].canMoveDown;
            player1.canMoveLeft = net::players[i].canMoveLeft;
            player1.canMoveRight = net::players[i].canMoveRight;


            player1.collisionRect.setPosition(net::players[i].collisionRect_x, net::players[i].collisionRect_y);
            player1.sprite.setPosition(player1.collisionRect.getPosition());
        }
        else
        {
            for (size_t j = 0; j < enemies.size(); j++)
            {
                //std::cout << "enem player id : " << enemies[j].id << std::endl;
                //std::cout << "[INFO] enem size : " << enemies.size() << std::endl;
                if (enemies[j].id == net::players[i].id)
                {
                    //std::cout << "enemy update - " << net::players[i].direction << std::endl;
                    enemies[j].isAlive = net::players[i].isAlive;
                    enemies[j].id = net::players[i].id;
                    enemies[j].hp = net::players[i].hp;
                    enemies[j].x = net::players[i].x;
                    enemies[j].y = net::players[i].y;
                    enemies[j].powerUpLevel = net::players[i].powerUpLevel;
                    enemies[j].direction = net::players[i].direction;
                    enemies[j].score = net::players[i].score;
                    enemies[j].canMoveUp = net::players[i].canMoveUp;
                    enemies[j].canMoveDown = net::players[i].canMoveDown;
                    enemies[j].canMoveLeft = net::players[i].canMoveLeft;
                    enemies[j].canMoveRight = net::players[i].canMoveRight;


                    enemies[j].collisionRect.setPosition(net::players[i].collisionRect_x, net::players[i].collisionRect_y);
                    enemyUpdate = true;
                }
            }


            if (enemyUpdate == false)
            {
                //std::cout << "enemyUpdate " << player.id << std::endl;
                enem.isAlive = net::players[i].isAlive;
                enem.id = net::players[i].id;
                enem.hp = net::players[i].hp;
                enem.x = net::players[i].x;
                enem.y = net::players[i].y;
                enem.powerUpLevel = net::players[i].powerUpLevel;
                enem.direction = net::players[i].direction;
                enem.score = net::players[i].score;
                enem.canMoveUp = net::players[i].canMoveUp;
                enem.canMoveDown = net::players[i].canMoveDown;
                enem.canMoveLeft = net::players[i].canMoveLeft;
                enem.canMoveRight = net::players[i].canMoveRight;


                enem.collisionRect.setPosition(net::players[i].collisionRect_x, net::players[i].collisionRect_y);
                enem.sprite.setTexture(playerTexture);
                enemies.push_back(enem);

            }
            enemyUpdate = false;
        }
    }
    //

    // update projectiles

    int netProjectileSize = net::projectiles.size();
    int clientProjectileSize = projectileArr.size();
    int diff = netProjectileSize - clientProjectileSize;
    std::cout << "[INFO] projectile update: " << netProjectileSize << " : " << clientProjectileSize << std::endl;


    if (clientProjectileSize < netProjectileSize)
    {
        counter2 = netProjectileSize - diff;
        while (counter2 < netProjectileSize)
        {
            netProjectileSize = net::projectiles.size();
            if (counter2 >= netProjectileSize)
                break;
            projectile.id = net::projectiles[counter2].id;
            projectile.isAlive = net::projectiles[counter2].isAlive;
            projectile.collisionRect.setPosition(net::projectiles[counter2].collisionRect_x,
                net::projectiles[counter2].collisionRect_y);
            projectile.collisionRect.setSize(sf::Vector2f(net::projectiles[counter2].width, net::projectiles[counter2].height));
            projectile.direction = net::projectiles[counter2].direction;
            projectile.isCollide = net::projectiles[counter2].isCollide;
            projectileArr.push_back(projectile);
            counter2++;
        }
    }
    else
    {

        for (size_t i = 0; i < net::projectiles.size(); i++)
        {
            if (net::projectiles.size() > 0)
            {
                try {
                    projectileArr.at(i);
                    if (net::projectiles.size() > projectileArr.size())
                        break;
                    if (projectileArr[i].id == net::projectiles[i].id)
                    {
                        projectileArr[i].isAlive = net::projectiles[i].isAlive;
                        projectileArr[i].isCollide = net::projectiles[i].isCollide;
                        projectileArr[i].collisionRect.setPosition(net::projectiles[i].collisionRect_x,
                            net::projectiles[i].collisionRect_y);
                    }
                }
                catch (int e)
                {
                    std::cout << "[ERROR] projectile index error" << std::endl;
                }
            }
        }
    }


    //

    // update enemies
    int netEnemySize = net::enemies.size();
    int clientEnemySize = enemyArr.size();
    diff = netEnemySize - clientEnemySize;
    std::cout << "[INFO] enemy update: " << netEnemySize << " : " << clientEnemySize << std::endl;
    if (clientEnemySize < netEnemySize)
    {
        counter2 = netEnemySize - diff;
        while (counter2 < netEnemySize)
        {

            enemy.id = net::enemies[counter2].id;
            enemy.isAlive = net::enemies[counter2].isAlive;
            enemy.collisionRect.setPosition(net::enemies[counter2].collisionRect_x,
                net::enemies[counter2].collisionRect_y);
            enemy.direction = net::enemies[counter2].direction;
            enemy.hp = net::enemies[counter2].hp;
            enemy.maxHp = net::enemies[counter2].maxHp;
            enemy.isCollide = net::enemies[counter2].isCollide;

            net::enemies[counter2].isBoss ? enemy.sprite.setTextureRect(sf::IntRect(48 * 6, 0, 48, 48))
                : enemy.sprite.setTextureRect(sf::IntRect(0, 0, 48, 48));
            enemyArr.push_back(enemy);
            counter2++;
        }
    }
    else //if (clientEnemySize == netEnemySize)
    {

        for (size_t i = 0; i < net::enemies.size(); i++)
        {
            if (net::enemies.size() > enemyArr.size())
                break;
            if (enemyArr[i].id == net::enemies[i].id)
            {
                enemyArr[i].hp = net::enemies[i].hp;
                enemyArr[i].isAlive = net::enemies[i].isAlive;
                enemyArr[i].isCollide = net::enemies[i].isCollide;
                enemyArr[i].collisionRect.setPosition(net::enemies[i].collisionRect_x,
                    net::enemies[i].collisionRect_y);
            }

        }

    }
    //
    // update items
    int netItemSize = net::items.size();
    int clientItemSize = itemArr.size();
    diff = netItemSize - clientItemSize;
    std::cout << "[INFO] item update: " << netItemSize << " : " << clientItemSize << std::endl;
    if (clientItemSize < netItemSize)
    {
        counter2 = netItemSize - diff;
        while (counter2 < netItemSize)
        {

            item.id = net::items[counter2].id;
            item.isAlive = net::items[counter2].isAlive;
            item.collisionRect.setPosition(net::items[counter2].collisionRect_x,
                net::items[counter2].collisionRect_y);
            item.scaleValue = net::items[counter2].scaleValue;
            item.inShop = net::items[counter2].inShop;
            item.cost = net::items[counter2].cost;
            item.type = net::items[counter2].type;
            item.isCollide = net::items[counter2].isCollide;
            item.animateSpriteNumber = net::items[counter2].animateSpriteNumber;

            item.spriteWidth = item.type == COIN ? 200 : 100;
            item.spriteHeight = item.type == COIN ? 150 : 100;
            item.y = item.type == COIN ? 10 : 0;
            item.sprite.setScale(sf::Vector2f(item.scaleValue, item.scaleValue));
            item.sprite.setTextureRect(item.type == COIN ?
                sf::IntRect(10, 10, 200, 150) : sf::IntRect(0, 0, 100, 100));

            item.sprite.setTexture(item.type == COIN ? coinTexture : powerUpTexture);
            if (item.inShop == true)
            {
                item.text.setFont(maumFont);
                item.text.setFillColor(sf::Color::Green);
                item.text.setString("Cost: " + to_string(item.cost));
            }
            itemArr.push_back(item);
            counter2++;
        }
    }
    else //if (clientItemSize == netItemSize)
    {
        for (size_t i = 0; i < net::items.size(); i++)
        {
            if (net::items.size() > itemArr.size())
                break;
            if (itemArr[i].id == net::items[i].id)
            {
                itemArr[i].isAlive = net::items[i].isAlive;
                itemArr[i].isCollide = net::items[i].isCollide;
                itemArr[i].collisionRect.setPosition(net::items[i].collisionRect_x,
                    net::items[i].collisionRect_y);
            }

        }
    }
    // update walls
    int netWallSize = net::walls.size();
    int clientWallSize = wallArr.size();
    diff = netWallSize - clientWallSize;
    std::cout << "[INFO] wall update: " << netWallSize << " : " << clientWallSize << std::endl;
    if (clientWallSize < netWallSize)
    {
        counter2 = netWallSize - diff;
        while (counter2 < netWallSize)
        {

            //enemy.id = net::projectiles[counter2].id;
            wall.id = net::walls[counter2].id;
            wall.isAlive = net::walls[counter2].isAlive;
            wall.hp = net::walls[counter2].hp;
            wall.collisionRect.setPosition(net::walls[counter2].collisionRect_x,
                net::walls[counter2].collisionRect_y);
            wall.destructible = net::walls[counter2].destructible;
            wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
            wall.isCollide = net::walls[counter2].isCollide;
            wallArr.push_back(wall);
            counter2++;
        }
    }
    else //if (clientWallSize == netWallSize)
    {
        for (size_t i = 0; i < net::walls.size(); i++)
        {
            if (net::walls.size() > wallArr.size())
                break;
            if (wallArr[i].id == net::walls[i].id)
            {
                wallArr[i].hp = net::walls[i].hp;
                wallArr[i].isAlive = net::walls[i].isAlive;
                wallArr[i].isCollide = net::walls[i].isCollide;
                wallArr[i].collisionRect.setPosition(net::walls[i].collisionRect_x,
                    net::walls[i].collisionRect_y);
            }

        }
    }

    if (STRESS_TEST == true)
    {
        player1.update();
        player1.direction = generateRandom(5);

        player1.virtualKeyPressed = player1.direction;
        client.WriteOperation(player1.direction, "127.0.0.1", 5555, net::handler, player1.id, "", 3);
    }

    //
    sf::Time projectileClockElapsed = projectileClock.getElapsedTime();
    sf::Time playerCollisionClockElapsed = playerCollisionClock.getElapsedTime();
    sf::Time aggroClockElapsed = aggroClock.getElapsedTime();



    // delete not alive enemy
    counter = 0;
    for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
    {
        bool found = false;
        for (auto i : net::enemies)
        {
            if (enemyArr[counter].id == i.id)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            enemyArr.erase(enemyIter);
            break;
        }
        counter++;
    }

    // delete not alive projectile
    counter = 0;
    for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
    {
        bool found = false;
        for (auto i : net::projectiles)
        {
            if (projectileArr[counter].id == i.id)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            //std::cout << ">>>>>>>>>>> deleted" << std::endl;
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
        bool found = false;
        for (auto i : net::items)
        {
            if (itemArr[counter].id == i.id)
            {
                found = true;
                break;
            }
        }
        if (found == false)
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
        bool found = false;
        for (auto i : net::walls)
        {
            if (wallArr[counter].id == i.id)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            wallArr.erase(wallIter);
            break;
        }
        counter++;
    }

    // delete not alive players
    counter = 0;
    for (playerIter = enemies.begin(); playerIter != enemies.end(); playerIter++)
    {
        bool found = false;
        for (auto i : net::players)
        {
            if (enemies[counter].id == i.id)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            enemies.erase(playerIter);
            break;
        }
        counter++;
    }


    // create projectile (space-Key)
    if (update)
    {
        counter = 0;
        if (projectileClockElapsed.asSeconds() >= 1.0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            //shotSound.play();
            projectileClock.restart();
            //std::cout << "shooooooooooooooooooooooooooooooooooooooooooooooo" <<std::endl;
            client.WriteOperation(5, "127.0.0.1", 5555, net::handler, player1.id, "", 3);
        }
    }

    // view player
    m_context->m_window->setView(view);
    view.setCenter(player1.collisionRect.getPosition());

}

void GamePlay::Draw()
{
    m_context->m_window->clear();
    // draw item
    counter = 0;
    for (itemIter = itemArr.begin(); itemIter != itemArr.end(); itemIter++)
    {
        if (itemArr[counter].inShop == true)
        {
            m_context->m_window->draw(itemArr[counter].text);
        }
        itemArr[counter].update();
        m_context->m_window->draw(itemArr[counter].sprite);
        counter++;
    }

    // draw projectile
    counter = 0;
    for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
    {
        projectileArr[counter].update();
        // draw eneryball
        gif.update(projectileArr[counter].sprite);
        m_context->m_window->draw(projectileArr[counter].sprite);
        //window.draw(projectileArr[counter].collisionRect);
        counter++;
    }

    // draw enemy Monster
    counter = 0;
    for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
    {
        enemyArr[counter].text.setString("HP " + to_string(enemyArr[counter].hp) + "/" + to_string(enemyArr[counter].maxHp));
        m_context->m_window->draw(enemyArr[counter].text);
        m_context->m_window->draw(enemyArr[counter].sprite);
        enemyArr[counter].update();
        counter++;
    }

    // draw wall
    counter = 0;
    for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
    {
        if (wallArr[counter].isAlive == true)
        {
            wallArr[counter].update();
            m_context->m_window->draw(wallArr[counter].sprite);
            //window.draw(wallArr[counter].collisionRect);
        }
        counter++;
    }

    // draw player , scoreText
    player1.text.setString("id : " + to_string(player1.id));
    m_context->m_window->draw(player1.text);
    m_context->m_window->draw(player1.sprite);

    for (size_t i = 0; i < enemies.size(); i++)
    {
        enemies[i].text.setString("id : " + to_string(enemies[i].id));
        enemies[i].update();
        m_context->m_window->draw(enemies[i].text);
        m_context->m_window->draw(enemies[i].sprite);
    }
    if (update)
    {
        player1.update();
        if (player1.updated == true)
        {
            client.WriteOperation(player1.direction, "127.0.0.1", 5555, net::handler, player1.id, "", 3);
            std::cout << "0000000000 updated " << std::endl;

        }
    }


    scoreText.setString("Money: " + to_string(player1.score));
    m_context->m_window->draw(scoreText);
    scoreText.setPosition(player1.collisionRect.getPosition().x - m_context->m_window->getSize().x / 2,
        player1.collisionRect.getPosition().y - m_context->m_window->getSize().y / 2);

    powerUpText.setString("Power Level: " + to_string(player1.powerUpLevel));
    m_context->m_window->draw(powerUpText);
    powerUpText.setPosition(player1.collisionRect.getPosition().x - m_context->m_window->getSize().x / 2,
        player1.collisionRect.getPosition().y - m_context->m_window->getSize().y / 2 + 50);

    hpText.setString("HP: " + to_string(player1.hp) + "/" + to_string(player1.maxHp));
    m_context->m_window->draw(hpText);
    hpText.setPosition(player1.collisionRect.getPosition().x - m_context->m_window->getSize().x / 2,
        player1.collisionRect.getPosition().y - m_context->m_window->getSize().y / 2 + 100);


    // draw ingameText
    counter = 0;
    for (ingameTextIter = ingameTextArr.begin(); ingameTextIter != ingameTextArr.end(); ingameTextIter++)
    {
        ingameTextArr[counter].update();
        m_context->m_window->draw(ingameTextArr[counter].text);
        counter++;
    }
    //
    m_context->m_window->display();
}