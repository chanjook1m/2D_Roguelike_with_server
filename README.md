# 2D_Roguelike_with_server

## Description

2D Roguelike game written in C++ with login, chat, game server

![in_game](https://user-images.githubusercontent.com/27846824/142897089-9652300b-84ad-4e4a-a5e9-8bffcae751eb.gif)


## Dependencies
- boost, boost_chrono, boost_date_time, boost_serialization, boost_thread
- cereal
- MYSQL Connector c++

## Description of files
| filename | description |
| --- | --- |
| 2D_Roguelike | Game client |
| 2D_Roguelike/client/Inputbox.hpp,.cpp | Login input box |
| 2D_Roguelike/client/Chatbox.hpp,.cpp | Chat content, input box |
| 2D_Roguelike/client/MainMenu.hpp,.cpp | Menu scene where login input box is set |
| 2D_Roguelike/client/StateManager.hpp,.cpp, State.hpp,.cpp | Manage scene as state for changing |
| 2D_Roguelike/client/Game.hpp,.cpp | Game scene where game logic is performed|
| 2D_Roguelike/client/GamePlay.hpp,.cpp | Game logic |
| 2D_Roguelike/client/entity.hpp,.cpp | Parent class for every object |
| 2D_Roguelike/client/projectile.hpp,.cpp | Projectile class |
| 2D_Roguelike/client/player.hpp,.cpp | Player class |
| 2D_Roguelike/client/enemy.hpp,.cpp | Enemy class |
| 2D_Roguelike/client/wall.hpp,.cpp | Wall class |
| 2D_Roguelike/client/item.hpp,.cpp | Item class |
| 2D_Roguelike/client/ingameText.hpp,.cpp | IngameText used in Game scene |
| 2D_Roguelike/client/animatedGIF.hpp,.cpp | Change GIF to sprite |
| 2D_Roguelike/client/random.hpp,.cpp | Perform randomized calculation |
| 2D_Roguelike/client/client_interface.hpp | Interface for communication with server |
| 2D_Roguelike/client/net.hpp | Include Classes and functions used in client_interface.hpp |
| Server_1 | Chat server |
| Server_2 | Login server |
| Server_3 | Game server |

## Getting Started
### Installation
1. Clone the repo
```code
https://github.com/chanjook1m/2D_Roguelike_with_server.git
```
2. Install boost dependencies (see above) with NuGet
3. Install MYSQL Connector c++ and set MYSQL_ROOT as environment variable with its exe path
4. Change resource path in GamePlay.cpp
