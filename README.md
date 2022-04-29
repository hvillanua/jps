# Jump Point Search (JPS)

## Motivation
I started this work as an exercise to remember C++ and learn about modern C++.
It's been some time that I wanted to implement JPS myself, so I decided to use it as the base project.
Also using this as a base to learn some new skills building a backend and frontend for the application.

## Download, install requirements, compile and run
Clone the repository and run the server, you need a C++17 compatible compiler:
```console
user@user-pc:~$ git clone https://github.com/hvillanua/jps.git
user@user-pc:~$ sudo apt-get install libboost-all-dev
user@user-pc:~$ cd jps
user@user-pc:~$ make
user@user-pc:~$ ./jps
```

## Use
If you want to create your own map and locations the following code can be used as a reference:
```
unordered_set<Location> walls {/*Define locations that constitute impassable terrain*/};
Grid map {10, 10, walls};

Location start {/*Define starting location */};
Location goal {/*Define goal location */};

auto came_from = jps(map, start, goal, Tool::euclidean);
auto path = Tool::reconstruct_path(start, goal, came_from);
Tool::draw_grid(map, {}, {}, path, came_from, start, goal);
```