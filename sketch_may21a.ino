const int matxWidth = 13;
const int matxHeight = 13;

const int restoreDelay = 1;

// 表明矩阵可用格子的真值表（剔除了四个角）
const bool gridMap[matxWidth][matxHeight] = {
false,false,false,false,true, true, true, true, true, false,false,false,false,
false,false,false,true, true, true, true, true, true, true, false,false,false,
false,false,true, true, true, true, true, true, true, true, true, false,false,
false,true, true, true, true, true, true, true, true, true, true, true, false,
true, true, true, true, true, true, true, true, true, true, true, true, true, 
true, true, true, true, true, true, true, true, true, true, true, true, true, 
true, true, true, true, true, true, true, true, true, true, true, true, true, 
true, true, true, true, true, true, true, true, true, true, true, true, true, 
true, true, true, true, true, true, true, true, true, true, true, true, true, 
false,true, true, true, true, true, true, true, true, true, true, true, false,
false,false,true, true, true, true, true, true, true, true, true, false,false,
false,false,false,true, true, true, true, true, true, true, false,false,false,
false,false,false,false,true, true, true, true, true, false,false,false,false
};

// 矩阵格子（气缸）的动作状态表（等于0为不激活，大于0为激活）
int valMap[matxWidth][matxHeight];

// if we want continous animation
//    begin another before one is fully stopped,
//    use a queue(array) instead of sigle variables.
int last_centreX, last_centreY;
int diffusionStep;
bool isDiffusionRunning;

void action_setActive(int i, int j)
{
    if (i < 0 || i >= matxWidth) return;
    if (j < 0 || j >= matxWidth) return;
    if (!gridMap[i][j] ) return;

    valMap[i][j] = restoreDelay;

    // !TODO: 在这里写你的驱动某个（即第i,j个）气缸 上升 的代码
}
void action_setDeactive(int i, int j)
{
    if (i < 0 || i >= matxWidth) return;
    if (j < 0 || j >= matxWidth) return;
    if (!gridMap[i][j] ) return;

    // !TODO: 在这里写你的驱动某个（即第i,j个）气缸 下降 的代码
}

void start_diffusion(int x, int y) 
{
    last_centreX = x;
    last_centreY = y;
    diffusionStep = 0;
    isDiffusionRunning = true;
}

void diffusion_routine(int step)
{
    if (!isDiffusionRunning) return;

    int p1x = last_centreX + step * (-1);
    int p1y = last_centreY;
    int p2x = last_centreX;
    int p2y = last_centreY + step * (-1);
    int p3x = last_centreX + step * (+1);
    int p3y = last_centreY;
    int p4x = last_centreX;
    int p4y = last_centreY + step * (+1);

    if (p1x <= 0 && p3x >= matxWidth && p2y <= 0 && p4y >= matxHeight)
    {
        // then we should stop the diffusion 
        isDiffusionRunning = false;
    }

    for (int dj = p1y - step + 1; dj <= p1y + step - 1; dj++)
    {
        action_setActive(p1x, dj);
        action_setActive(p3x, dj);

    }
    for (int di = p2x - step ; di <= p2x + step; di++)
    {
        action_setActive(di, p2y);
        action_setActive(di, p4y);
    }

}

#define _abstract

// !TODO: 这是第i,j个按钮按下的时候需要调用的函数，
//          请你在合适的地方（按按钮的时候）调用.
_abstract void on_button_pressed(int i, int j){
  start_diffusion(i, j);
}

// !TODO: 这是每个动画帧刷新的时候需要调用的函数.
//          请你在你自己实现的定时器里面调用。
_abstract void on_every_timer_tick() {
  
    diffusion_routine(diffusionStep++);
    for (int i = 0; i < matxWidth; i++)
    {
        for (int j = 0; j < matxHeight; j++)
        {
            if (valMap[i][j] > 0)
            {
                valMap[i][j]--;
            }
            else
            {
                valMap[i][j] = 0;
                action_setDeactive(i, j);
            }
        }
    }
}

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:

  // !TODO: 我建议你写一个定长时间的定时器，让它每次都调用on_every_timer_tick()，
  //        不然，像下面这样，你的动画效果可能不会太完美。
  //
  //        关于Arduino定时器：http://playground.arduino.cc/Code/Timer
  //        例如，用timer1.every(20, on_every_timer_tick);
  
  on_every_timer_tick();
  delay(20);

}

