using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace test_animation
{
    public partial class Form1 : Form
    {

        const int matxWidth = 13;
        const int matxHeight = 13;

        PictureBox[,] imgArray = new PictureBox[matxWidth, matxHeight];
        bool[,] gridMap = new bool[matxWidth, matxHeight];
        int[,] valMap = new int[matxWidth, matxHeight];

        public void initGridMap()
        {
            for (int i = 0; i < matxWidth; i++)
            {
                for (int j = 0; j < matxHeight; j++)
                {
                    gridMap[i, j] = true;
                }
            }
            gridMap[0, 0] = false;
            gridMap[0, 1] = false;
            gridMap[0, 2] = false;
            gridMap[0, 3] = false;
            gridMap[1, 2] = false;
            gridMap[2, 1] = false;
            gridMap[3, 0] = false;
            gridMap[2, 0] = false;
            gridMap[1, 0] = false;
            gridMap[1, 1] = false;
            gridMap[12, 0] = false;
            gridMap[11, 0] = false;
            gridMap[10, 0] = false;
            gridMap[9, 0] = false;
            gridMap[10, 1] = false;
            gridMap[11, 1] = false;
            gridMap[12, 1] = false;
            gridMap[11, 2] = false;
            gridMap[12, 2] = false;
            gridMap[12, 3] = false;
            gridMap[9, 12] = false;
            gridMap[10, 12] = false;
            gridMap[11, 12] = false;
            gridMap[12, 12] = false;
            gridMap[12, 11] = false;
            gridMap[12, 10] = false;
            gridMap[12, 9] = false;
            gridMap[11, 10] = false;
            gridMap[11, 11] = false;
            gridMap[10, 11] = false;
            gridMap[3, 12] = false;
            gridMap[2, 12] = false;
            gridMap[1, 12] = false;
            gridMap[0, 12] = false;
            gridMap[0, 11] = false;
            gridMap[0, 10] = false;
            gridMap[0, 9] = false;
            gridMap[1, 10] = false;
            gridMap[2, 11] = false;
            gridMap[1, 11] = false;

        }

        public Form1()
        {
            InitializeComponent();
            initGridMap();
            printGridMap();
            for (int i = 0; i < matxWidth; i++)
            {
                for (int j = 0; j < matxHeight; j++)
                {
                    PictureBox pb = new PictureBox();
                    pb.ClientSize = new Size(20, 20);
                    pb.Location = new Point(i * 22, j * 22);
                    pb.BackColor = Color.White;
                    pb.SizeMode = PictureBoxSizeMode.StretchImage;
                    if (gridMap[i, j])
                    {
                        pb.MouseClick += pb_MouseClick;
                    }
                    else
                    {
                        pb.BackColor = Color.Empty;
                    }
                    this.Controls.Add(pb);
                    imgArray[i,j] = pb;
                }
            }
        }

        Point findPBIndex(PictureBox pb)
        {
            for (int i = 0; i < matxWidth; i++)
            {
                for (int j = 0; j < matxHeight; j++)
                {
                    if (pb == imgArray[i, j])
                        return new Point(i, j);
                }
            }
            return new Point(-1,-1);

        }
        
        private int last_centreX, last_centreY;
        private int diffusionStep;
        private bool isDiffusionRunning;

        void start_diffusion(int x, int y) 
        {
            last_centreX = x;
            last_centreY = y;
            diffusionStep = 0;
            isDiffusionRunning = true;

        }

        void action_setActive(int i, int j)
        {
            if (i < 0 || i >= matxWidth) return;
            if (j < 0 || j >= matxWidth) return;
            if (!gridMap[i, j]) return;

            valMap[i, j] = restoreDelay;

            imgArray[i, j].BackColor = Color.Black;
        }
        void action_setDeactive(int i, int j)
        {
            if (i < 0 || i >= matxWidth) return;
            if (j < 0 || j >= matxWidth) return;
            if (!gridMap[i, j]) return;

            imgArray[i, j].BackColor = Color.White;
        }

        int restoreDelay = 1;
        void diffusion_routine(int step)
        {
            if (!isDiffusionRunning) return;

            Point p1 = new Point(last_centreX + step * (-1), last_centreY);
            Point p2 = new Point(last_centreX, last_centreY + step * (-1));
            Point p3 = new Point(last_centreX + step * (+1), last_centreY);
            Point p4 = new Point(last_centreX, last_centreY + step * (+1));

            if (p1.X <= 0 && p3.X >= matxWidth && p2.Y <= 0 && p4.Y >= matxHeight)
            {
                // then we should stop the diffusion 
                isDiffusionRunning = false;
            }

            for (int dj = p1.Y - step + 1; dj <= p1.Y + step - 1; dj++)
            {
                action_setActive(p1.X, dj);
                action_setActive(p3.X, dj);

            }
            for (int di = p2.X - step ; di <= p2.X + step; di++)
            {
                action_setActive(di, p2.Y);
                action_setActive(di, p4.Y);
            }

        }

        void pb_MouseClick(object sender, MouseEventArgs e)
        {
            PictureBox pb = (PictureBox)sender;
            //pb.BackColor = Color.Gray;
            Point foundCoor = findPBIndex(pb);
            System.Console.Out.WriteLine("clicked at ["+foundCoor.X+","+ foundCoor.Y+"]");
            start_diffusion(foundCoor.X, foundCoor.Y);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }

        void printGridMap() {
            System.Console.Out.WriteLine("{");
            for (int i = 0; i < matxWidth; i++)
            {
                for (int j = 0; j < matxHeight; j++)
                {
                    System.Console.Out.Write(gridMap[i, j] + ",");
                }
                System.Console.Out.WriteLine("");
            }
            System.Console.Out.WriteLine("};");
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            diffusion_routine(diffusionStep++);
            for (int i = 0; i < matxWidth; i++)
            {
                for (int j = 0; j < matxHeight; j++)
                {
                    if (valMap[i, j] > 0)
                    {
                        valMap[i, j]--;
                    }
                    else
                    {
                        valMap[i, j] = 0;
                        action_setDeactive(i, j);
                    }
                }
            }
        }

    }
}
