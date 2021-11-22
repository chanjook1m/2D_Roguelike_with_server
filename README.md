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
