/*****************************************************************************
* gmaze.cpp
*
* Maze generated and solved using a recursive backtracker algorithm
* Modeled after xmaze
* The Logo is a homage to the original x windows version
*
* Originally written in 1999 using the GRX graphics library for DOS
* Modified in 2016 to use WINBGIM graphics library and runs under Windows
*
*****************************************************************************/
#include <time.h>
#include <graphics.h>

/*****************************************************************************
* a cell is a 5 pixel x 5 pixel square.
* the top 4 x left 4 is the cell with the bottom row and right side being walls
*****************************************************************************/
#define PER_CELL 5
#define X_MIN    9
#define X_MAX    629
#define X_CELLS ((X_MAX-X_MIN)/PER_CELL)
#define Y_MIN    9
#define Y_MAX    469
#define Y_CELLS ((Y_MAX-Y_MIN)/PER_CELL)

// Function to draw a filled bar using the fillpoly function in BGI
void box(int x, int y, int xx, int yy, int c);

int main()
{
    int    direction, found;
    int    color, preferred=0;
    int    step;
    int    x_stack[10000];
    int    y_stack[10000];

    // Variables needed for the BGI library initialization
    int gd=DETECT;
    int gm;

    // Picture for the Logo
    char    pic[]=
       {"00000011100000011010000000001"
        "01000011100000011010100000001"
        "00000010000000000000100000100"
        "00000011110111111111111111001"
        "00010001000100000000000001000"
        "00010001000100000000000001000"
        "00011001000101110000000101001"
        "11000001000100111000001001010"
        "11100001000100011100010001000"
        "00000011100100001110100001001"
        "00000000100100000111000001010"
        "00000000100100001011100001001"
        "00000000100100010001110001000"
        "00010000000100100000111001000"
        "00010001000101000000011101001"
        "11010001010100000000000001010"
        "00110001000100000000000001000"
        "11000000000111111111111111000"
        "00000000000000010000000000000"
        "00000111111100011101100000011"
        "00000010100100001000100000011"
        "00000000000100000000100010000"
        "01110000001101111100001110000"
        "11010001100101000011101010000"
        "00011001010110011000001111001"
        "00010001100100001000100000000"
        "00000001000100000000100000010"
        "00000001000110000000100000001"
        "01100001000000010000000000000"};

    // Initialize graphics screen and random number generator
    initgraph(&gd, &gm, "C:\\TC\\BGI");
    srand(time(NULL));

    // Loop for new maze
    while(TRUE){

        // Draw Maze Box
        // Clear the screen
        box(0,0,639,479,BLACK);
        // Fill usable area with neutral color
        box(X_MIN,Y_MIN,X_MAX,Y_MAX,DARKGRAY);
        // Draw a border
        setcolor(WHITE);
        rectangle(X_MIN,Y_MIN,X_MAX,Y_MAX);
        rectangle(X_MIN-1,Y_MIN-1,X_MAX+1,Y_MAX+1);

        // Randomly select a location and draw box for the logo
        // And store that location to the bottom of the stack
        x_stack[9999]=((rand() % (X_CELLS-18))+3)*PER_CELL; // width is 60 pixels / 5 = 12 so 18 - 12 =6 /2 = 3
        y_stack[9999]=((rand() % (Y_CELLS-18))+3)*PER_CELL;

        box(x_stack[9999], y_stack[9999], x_stack[9999]+59, y_stack[9999]+59, CYAN);
        setcolor(WHITE);
        rectangle(x_stack[9999]-1,y_stack[9999]-1,x_stack[9999]+59,y_stack[9999]+59);

        // Randomly select an ending location at the top of the screen, we will build maze from that point
        x_stack[0]=(rand() % (X_CELLS-10)+2)*PER_CELL;
        y_stack[0] = Y_MIN+1;
        box(x_stack[0], 0, x_stack[0]+3, Y_MIN, MAGENTA);
        // Draw bottom and right side wall, they might be erased later
        setcolor(WHITE);
        line(x_stack[0]+4,y_stack[0],  x_stack[0]+4,y_stack[0]+4);
        line(x_stack[0],  y_stack[0]+4,x_stack[0]+4,y_stack[0]+4);
        // Fill in the cell with the visited color
        box( x_stack[0],  y_stack[0],  x_stack[0]+3,y_stack[0]+3, CYAN);

        step = 0;

        // while the current cell has an empty cell around it
        while((getpixel(x_stack[step]+5,y_stack[step]  )==DARKGRAY)
            ||(getpixel(x_stack[step]-5,y_stack[step]  )==DARKGRAY)
            ||(getpixel(x_stack[step],  y_stack[step]+5)==DARKGRAY)
            ||(getpixel(x_stack[step],  y_stack[step]-5)==DARKGRAY))
        {
            found=0;
            while(found==0)
            {
                // Randomly set a direction (0 - 3) if greater than 4 use previous direction
                // else save the new direction
                // This makes the previous direction 3x more likely to be chosen
                direction=rand() % 6;
                if(direction>=4)
                    direction=preferred;
                else
                    preferred=direction;
                setcolor(WHITE);
                switch(direction){
                    // If available, move up, do not draw the bottom as we just moved across that border
                    case 0:
                    if(getpixel(x_stack[step],y_stack[step]-5)==DARKGRAY)
                    {
                        step++;
                        x_stack[step]=x_stack[step-1];
                        y_stack[step]=y_stack[step-1]-5;
                        line(x_stack[step]+4,y_stack[step],x_stack[step]+4,y_stack[step]+4);
                        box( x_stack[step],  y_stack[step],x_stack[step]+3,y_stack[step]+4, CYAN);
                        found=1;
                    }
                    break;
                    // If available, move left, do not draw right wall as we just moved across that border
                    case 1:
                    if(getpixel(x_stack[step]-5,y_stack[step])==DARKGRAY)
                    {
                        step++;
                        x_stack[step]=x_stack[step-1]-5;
                        y_stack[step]=y_stack[step-1];
                        line(x_stack[step],y_stack[step]+4,x_stack[step]+4,y_stack[step]+4);
                        box( x_stack[step],y_stack[step],  x_stack[step]+4,y_stack[step]+3, CYAN);
                        found=1;
                    }
                    break;
                    // if available, move down
                    case 2:
                    if(getpixel(x_stack[step],y_stack[step]+5)==DARKGRAY)
                    {
                        step++;
                        x_stack[step]=x_stack[step-1];
                        y_stack[step]=y_stack[step-1]+5;
                        line(x_stack[step]+4,y_stack[step],  x_stack[step]+4,y_stack[step]+4);
                        line(x_stack[step],  y_stack[step]+4,x_stack[step]+4,y_stack[step]+4);
                        box( x_stack[step],  y_stack[step]-1,x_stack[step]+3,y_stack[step]+3, CYAN);
                        found=1;
                    }
                    break;
                    // If available, move right
                    case 3:
                    if(getpixel(x_stack[step]+5,y_stack[step])==DARKGRAY)
                    {
                        step++;
                        x_stack[step]=x_stack[step-1]+5;
                        y_stack[step]=y_stack[step-1];
                        line(x_stack[step]+4,y_stack[step],  x_stack[step]+4,y_stack[step]+4);
                        line(x_stack[step],  y_stack[step]+4,x_stack[step]+4,y_stack[step]+4);
                        box( x_stack[step]-1,y_stack[step],  x_stack[step]+3,y_stack[step]+3, CYAN);
                        found=1;
                    }
                    break;
                }
            }

            // If there is no more places to move, backtrack using the stack until we find one
            // If we backed up to step 0 and there is still not open places we are finished creating the maze
            while((getpixel(x_stack[step]+5,y_stack[step]  )!=DARKGRAY)
                &&(getpixel(x_stack[step]-5,y_stack[step]  )!=DARKGRAY)
                &&(getpixel(x_stack[step],  y_stack[step]+5)!=DARKGRAY)
                &&(getpixel(x_stack[step],  y_stack[step]-5)!=DARKGRAY)
                &&(step!=0))
                    --step;
        }

        // Draw Logo Box Graphics
        for(y_stack[0]=0; y_stack[0]<29; y_stack[0]++)
        {
            for(x_stack[0]=0; x_stack[0]<29; x_stack[0]++)
            {
                if(pic[y_stack[0]*29+x_stack[0]]=='1'){
                    putpixel(2*x_stack[0]+x_stack[9999],  2*y_stack[0]+y_stack[9999],  RED);
                    putpixel(2*x_stack[0]+x_stack[9999]+1,2*y_stack[0]+y_stack[9999],  RED);
                    putpixel(2*x_stack[0]+x_stack[9999],  2*y_stack[0]+y_stack[9999]+1,RED);
                    putpixel(2*x_stack[0]+x_stack[9999]+1,2*y_stack[0]+y_stack[9999]+1,RED);
                }
            }
        }

        // Randomly select a starting location at the bottom of the screen
        x_stack[0]=(rand() % (X_CELLS-10)+2)*PER_CELL;
        y_stack[0] = Y_MAX - 4;
        box(x_stack[0], 479, x_stack[0]+3, Y_MAX-4, RED);
        step = 0;

        // Solve the maze
        // Use the direction in order, Up, Left, Down, Right
        // Check to see if there is a wall and if the cell is not visited
        while(((getpixel(x_stack[step],  y_stack[step]-1)==CYAN) && (getpixel(x_stack[step],  y_stack[step]-5)== CYAN))
            ||((getpixel(x_stack[step]-1,y_stack[step]  )==CYAN) && (getpixel(x_stack[step]-5,y_stack[step]  )== CYAN))
            ||((getpixel(x_stack[step]  ,y_stack[step]+4)==CYAN) && (getpixel(x_stack[step],  y_stack[step]+5)== CYAN))
            ||((getpixel(x_stack[step]+4,y_stack[step]  )==CYAN) && (getpixel(x_stack[step]+5,y_stack[step]  )== CYAN)))
        {
            if((getpixel(x_stack[step],  y_stack[step]-1)==CYAN) && (getpixel(x_stack[step],  y_stack[step]-5)== CYAN)){
                x_stack[step+1]=x_stack[step];
                y_stack[step+1]=y_stack[step]-5;
                box(x_stack[step+1],y_stack[step+1],x_stack[step+1]+3,y_stack[step+1]+4, RED);}
            else{
                if((getpixel(x_stack[step]-1,y_stack[step]  )==CYAN) && (getpixel(x_stack[step]-5,y_stack[step]  )== CYAN)){
                    x_stack[step+1]=x_stack[step]-5;
                    y_stack[step+1]=y_stack[step];
                    box(x_stack[step+1],y_stack[step+1],x_stack[step+1]+4,y_stack[step+1]+3, RED);}
                else{
                    if((getpixel(x_stack[step]  ,y_stack[step]+4)==CYAN) && (getpixel(x_stack[step],  y_stack[step]+5)== CYAN)){
                        x_stack[step+1]=x_stack[step];
                        y_stack[step+1]=y_stack[step]+5;
                        box(x_stack[step+1],y_stack[step+1]-1,x_stack[step+1]+3,y_stack[step+1]+3, RED);}
                    else{
                        if((getpixel(x_stack[step]+4,y_stack[step]  )==CYAN) && (getpixel(x_stack[step]+5,y_stack[step]  )== CYAN)){
                            x_stack[step+1]=x_stack[step]+5;
                            y_stack[step+1]=y_stack[step];
                            box(x_stack[step+1]-1,y_stack[step+1],x_stack[step+1]+3,y_stack[step+1]+3, RED);}
                    }
                }
            }

            step++;

            // If the cell above is the ending location, we are finished
            if(getpixel(x_stack[step],  y_stack[step]-5)== MAGENTA)
            break;

            // If there is no more places to move, backtrack using the stack until we find one
            // Everytime we move away from a location, mark it as the wrong direction
            while(((getpixel(x_stack[step],  y_stack[step]-1)!=CYAN) || (getpixel(x_stack[step],  y_stack[step]-5)!= CYAN))
                &&((getpixel(x_stack[step]-1,y_stack[step]  )!=CYAN) || (getpixel(x_stack[step]-5,y_stack[step]  )!= CYAN))
                &&((getpixel(x_stack[step]  ,y_stack[step]+4)!=CYAN) || (getpixel(x_stack[step],  y_stack[step]+5)!= CYAN))
                &&((getpixel(x_stack[step]+4,y_stack[step]  )!=CYAN) || (getpixel(x_stack[step]+5,y_stack[step]  )!= CYAN)))
            {
                // Since we do not know which way we are leaving, change color of all pixels in the cell
                // that mean correct direction
                for(y_stack[9998]=y_stack[step]-1; y_stack[9998]<y_stack[step]+4; y_stack[9998]++)
                    for(x_stack[9998]=x_stack[step]-1; x_stack[9998]<x_stack[step]+4; x_stack[9998]++)
                        if((color=getpixel(x_stack[9998],y_stack[9998]) == RED) && color != CYAN)
                            putpixel(x_stack[9998],y_stack[9998], BLUE);
                --step;
            }
        }

        // We have solved the maze when we get here
        // As a reward, flash the Logo different colors
        for(y_stack[9998]=0; y_stack[9998]<=200; y_stack[9998]++)
        {
            color=rand() % 16;

            for(y_stack[0]=0; y_stack[0]<29; y_stack[0]++)
            {
                for(x_stack[0]=0; x_stack[0]<29; x_stack[0]++)
                {
                    if(pic[y_stack[0]*29+x_stack[0]]=='1')
                    {
                        putpixel(2*x_stack[0]+x_stack[9999],  2*y_stack[0]+y_stack[9999],  color);
                        putpixel(2*x_stack[0]+x_stack[9999]+1,2*y_stack[0]+y_stack[9999],  color);
                        putpixel(2*x_stack[0]+x_stack[9999],  2*y_stack[0]+y_stack[9999]+1,color);
                        putpixel(2*x_stack[0]+x_stack[9999]+1,2*y_stack[0]+y_stack[9999]+1,color);
                    }
                }
            }
        }

    }

    return(0);
}

// box subroutine
// Initializes the poly array holding the x and y coordinates of the rectangle corners
// and calls the fillpoly BGI routine to dray and fill the polynomial
void box(int x, int y, int xx, int yy, int c)
{
    int poly[10];
    setcolor(c);
    setfillstyle(SOLID_FILL, c);
    poly[0]=poly[8]=poly[6]=x;
    poly[1]=poly[9]=poly[3]=y;
    poly[2]=poly[4]=xx;
    poly[5]=poly[7]=yy;
    fillpoly(5, poly);
}
