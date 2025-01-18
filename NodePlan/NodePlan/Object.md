游戏内所有要显式每帧更新的类

# protected
## texture
### 定义
```c++
std::vector<SDL_Texture *> texture;
```
### 说明
使用了容器`vector`，存放所有的材质（或许可以使用精灵表？但是我懒了😁）

---
# public
## Object()
## ~Object()
## paint()
### 定义
```c++
void paint();
```
### 说明
调用之后就会按照当前的方式绘画出来。