const int matxWidth = 13;
const int matxHeight = 13;

const int restoreDelay = 1;

// indicating which grid is valid.
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

    // !TODO: The actual code the make the actuator active
    //        YOUR CODE setting [i][j] to active
}
void action_setDeactive(int i, int j)
{
    if (i < 0 || i >= matxWidth) return;
    if (j < 0 || j >= matxWidth) return;
    if (!gridMap[i][j] ) return;

    // !TODO: The actual code the make the actuator active
    //        YOUR CODE setting [i][j] to deactive!
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

// !TODO: CHANGE this to fit your code.
_abstract void on_button_pressed(int i, int j){
  start_diffusion(i, j);
}

// !TODO: CHANGE this to fit your code.
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

  // !TODO: your should write a more stable timer
  on_every_timer_tick();
  delay(20);

}
