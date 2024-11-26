#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>

unsigned int frame_counter = 0;

#define vga_width    120
#define vga_height    60
#define vga_pixel_size 12

char Color_pixel[vga_width*vga_height];

const char WALL_TAB[] = {
		0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,		
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
};

const char WALL_LINE[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,
};

unsigned int SCREEN_WIDTH = vga_width * vga_pixel_size;
unsigned int SCREEN_HEIGHT = vga_height * vga_pixel_size;

int selectedButtonWidthIndex = 0; // Index of the width of the selected button
int selectedButtonHeightIndex = 0; // Index of the height of the selected button
int selectButtonModifyIndex = 0;

int bigButtonClickedIndex = -1; // Initialize to -1 to indicate no button is clicked yet

int pageState = 1;
int pageStateChanged = 0;

#define gameColumn 8
#define gameLine 12

int reverseMusicPosition = vga_height;
int fileLine = 0;
int max_score = 0;
int score = 0;
int new_line[gameColumn];
int tabMusic[gameColumn*4*gameLine];
int lineMusic[gameColumn*4*gameLine] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,0,0,1,0,0,0,
    0,0,1,1,0,0,0,0,
    0,0,0,0,0,1,0,0,
    0,1,0,1,0,0,0,0,
    0,0,0,0,0,0,1,0,
    1,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,0,0,1,0,0,0,
    0,0,1,0,0,0,0,0,
    0,0,0,0,0,1,0,0,
    0,1,0,1,0,0,0,0,
    0,0,0,0,0,0,1,0,
    1,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,0,0,1,0,0,0,
    0,0,1,0,0,0,0,0,
    0,0,0,0,0,1,0,0,
    0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,
    1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,0,0,1,0,0,0,
    0,0,1,0,0,0,0,0,
    0,0,0,0,0,1,0,0,
    0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,
    1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,1,
};


void putPixel(unsigned int x, unsigned int y, unsigned int color)
{
	// Check if x and y are within the bounds of the Color_pixel array
    if (x < vga_width && y < vga_height)
    {
        Color_pixel[x*60+y]=color;
        Color c;
        if(color == 0)
            c = BLACK;
        if(color == 1)
            c = GREEN;
        if(color == 2)
            c = PURPLE;
        if(color == 3)
            c = WHITE;
        DrawRectangle(x*vga_pixel_size, y*vga_pixel_size, vga_pixel_size, vga_pixel_size, c);
    }
    else
    {
        printf("Error: x or y is out of bounds\n");
    }
}

void DrawNewRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color)
{
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            putPixel(x+i,y+j,color);
        }
    }
}

void DrawEmptyRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color)
{
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            if(i==0 || i==width-1 || j==0 || j==height-1){
                putPixel(x+i,y+j,color);
            }
        }
    }
}

void drawMusicNote(unsigned int center_x, unsigned int center_y,unsigned int color){
	int bottom = -2;
	int top = 2;
	for(int i = bottom; i<top+1;i++){
		for(int j = bottom; j<top+1;j++){
			if(((i==bottom || i==top) && j!=bottom && j!=top) 
			|| ((j==bottom || j==top) && i!=bottom && i!=top)){
				putPixel(center_x+j,center_y+i,color);
			}
		}
	}
}

void DrawTitle()
{
	// Draw the title
	int margin = 32;
	// R letter
	putPixel(10 + margin, 5, 1);
	putPixel(10 + margin, 5, 1);
	putPixel(11 + margin, 5, 1);
	putPixel(12 + margin, 5, 1);
	putPixel(13 + margin, 5, 1);
	putPixel(14 + margin, 6, 1);
	putPixel(14 + margin, 7, 1);
	putPixel(14 + margin, 8, 1);
	putPixel(13 + margin, 9, 1);
	putPixel(12 + margin, 9, 1);
	putPixel(11 + margin, 9, 1);
	putPixel(10 + margin, 6, 1);
	putPixel(10 + margin, 7, 1);
	putPixel(10 + margin, 8, 1);
	putPixel(10 + margin, 9, 1);
	putPixel(10 + margin, 10, 1);
	putPixel(10 + margin, 11, 1);
	putPixel(10 + margin, 12, 1);
	putPixel(10 + margin, 13, 1);
	putPixel(11 + margin, 10, 1);
	putPixel(12 + margin, 11, 1);
	putPixel(13 + margin, 12, 1);
	putPixel(14 + margin, 13, 1);

	// Y letter
	putPixel(17 + margin, 5, 1);
	putPixel(17 + margin, 6, 1);
	putPixel(17 + margin, 7, 1);
	putPixel(18 + margin, 8, 1);
	putPixel(18 + margin, 9, 1);
	putPixel(20 + margin, 9, 1);
	putPixel(20 + margin, 8, 1);
	putPixel(21 + margin, 7, 1);
	putPixel(21 + margin, 6, 1);
	putPixel(21 + margin, 5, 1);
	putPixel(19 + margin, 10, 1);
	putPixel(19 + margin, 11, 1);
	putPixel(19 + margin, 12, 1);
	putPixel(19 + margin, 13, 1);

	// T letter
	putPixel(24 + margin, 5, 1);
	putPixel(25 + margin, 5, 1);
	putPixel(26 + margin, 5, 1);
	putPixel(27 + margin, 5, 1);
	putPixel(28 + margin, 5, 1);
	putPixel(26 + margin, 6, 1);
	putPixel(26 + margin, 7, 1);
	putPixel(26 + margin, 8, 1);
	putPixel(26 + margin, 9, 1);
	putPixel(26 + margin, 10, 1);
	putPixel(26 + margin, 11, 1);
	putPixel(26 + margin, 12, 1);
	putPixel(26 + margin, 13, 1);

	putPixel(31 + margin, 5, 1);
	putPixel(31 + margin, 6, 1);
	putPixel(31 + margin, 7, 1);
	putPixel(31 + margin, 8, 1);
	putPixel(31 + margin, 9, 1);
	putPixel(31 + margin, 10, 1);
	putPixel(31 + margin, 11, 1);
	putPixel(31 + margin, 12, 1);
	putPixel(31 + margin, 13, 1);
	putPixel(32 + margin, 9, 1);
	putPixel(33 + margin, 9, 1);
	putPixel(34 + margin, 9, 1);
	putPixel(35 + margin, 5, 1);
	putPixel(35 + margin, 6, 1);
	putPixel(35 + margin, 7, 1);
	putPixel(35 + margin, 8, 1);
	putPixel(35 + margin, 9, 1);
	putPixel(35 + margin, 10, 1);
	putPixel(35 + margin, 11, 1);
	putPixel(35 + margin, 12, 1);
	putPixel(35 + margin, 13, 1);

	// M letter
	putPixel(38 + margin, 5, 1);
	putPixel(38 + margin, 6, 1);
	putPixel(38 + margin, 7, 1);
	putPixel(38 + margin, 8, 1);
	putPixel(38 + margin, 9, 1);
	putPixel(38 + margin, 10, 1);
	putPixel(38 + margin, 11, 1);
	putPixel(38 + margin, 12, 1);
	putPixel(38 + margin, 13, 1);
	putPixel(39 + margin, 5, 1);
	putPixel(40 + margin, 6, 1);
	putPixel(40 + margin, 7, 1);
	putPixel(41 + margin, 8, 1);
	putPixel(41 + margin, 9, 1);
	putPixel(42 + margin, 7, 1);
	putPixel(42 + margin, 6, 1);
	putPixel(43 + margin, 5, 1);
	putPixel(44 + margin, 5, 1);
	putPixel(44 + margin, 6, 1);
	putPixel(44 + margin, 7, 1);
	putPixel(44 + margin, 8, 1);
	putPixel(44 + margin, 9, 1);
	putPixel(44 + margin, 10, 1);
	putPixel(44 + margin, 11, 1);
	putPixel(44 + margin, 12, 1);
	putPixel(44 + margin, 13, 1);

	// O letter
	putPixel(47 + margin, 5, 1);
	putPixel(47 + margin, 6, 1);
	putPixel(47 + margin, 7, 1);
	putPixel(47 + margin, 8, 1);
	putPixel(47 + margin, 9, 1);
	putPixel(47 + margin, 10, 1);
	putPixel(47 + margin, 11, 1);
	putPixel(47 + margin, 12, 1);
	putPixel(47 + margin, 13, 1);
	putPixel(48 + margin, 5, 1);
	putPixel(49 + margin, 5, 1);
	putPixel(50 + margin, 5, 1);
	putPixel(51 + margin, 5, 1);
	putPixel(51 + margin, 6, 1);
	putPixel(51 + margin, 7, 1);
	putPixel(51 + margin, 8, 1);
	putPixel(51 + margin, 9, 1);
	putPixel(51 + margin, 10, 1);
	putPixel(51 + margin, 11, 1);
	putPixel(51 + margin, 12, 1);
	putPixel(51 + margin, 13, 1);
	putPixel(48 + margin, 13, 1);
	putPixel(49 + margin, 13, 1);
	putPixel(50 + margin, 13, 1);
}

void DrawPlayIcon(int centerX,int centerY,int color){
	// Draw the play icon
	putPixel(centerX - 1, centerY - 2, color);
	putPixel(centerX - 1, centerY - 1, color);
	putPixel(centerX - 1, centerY, color);
	putPixel(centerX - 1, centerY + 1, color);
	putPixel(centerX - 1, centerY + 2, color);
	putPixel(centerX, centerY - 1, color);
	putPixel(centerX + 1, centerY, color);
	putPixel(centerX, centerY + 1, color);
	putPixel(centerX, centerY, color);
}

void DrawStopIcon(int centerX,int centerY,int color){
	// Draw the play icon
	for(int i=0;i<2;i++){
		DrawNewRectangle(centerX-1+2*i,centerY-1,1,3,color);
	}
}

void DrawEditButton(int centerX,int centerY,int color){
	// Edit button
	for (int k = -2 ; k < 3 ; k+=2)
	{
		putPixel(centerX - 2, centerY + k, color);
		putPixel(centerX, centerY + k, color);
		putPixel(centerX + 1, centerY + k, color);
		putPixel(centerX + 2, centerY + k, color);
	}
}

void DrawCrossSize(int centerX,int centerY,int color){
	// Cross button
	// Define the size of the cross
	int crossSize = 5 / 2;  // Adjust this value to change the size of the cross
	// Draw the cross
	for (int i = -crossSize / 2 - 1; i <= crossSize / 2 + 1; i++)
	{
		putPixel(centerX + i, centerY + i, color);
		putPixel(centerX + i, centerY - i, color);
	}
}

void DrawHouse(int centerX,int centerY,int color){
	putPixel(centerX, centerY - 2, color);
	putPixel(centerX - 1, centerY - 1, color);
	putPixel(centerX, centerY - 1, color);
	putPixel(centerX + 1, centerY - 1, color);
	putPixel(centerX - 2, centerY, color);
	putPixel(centerX - 1, centerY, color);
	putPixel(centerX, centerY, color);
	putPixel(centerX + 1, centerY, color);
	putPixel(centerX + 2, centerY, color);
	putPixel(centerX - 1, centerY + 1, color);
	putPixel(centerX + 1, centerY + 1, color);
	putPixel(centerX - 1, centerY + 2, color);
	putPixel(centerX, centerY + 2, color);
	putPixel(centerX + 1, centerY + 2, color);
}

void drawMenu(int center_x, int center_y)
{
	// Draw the title
	DrawTitle();

    // Define button dimensions
    int buttonWidth = 20;
    int buttonHeight = 5;
 	int smallButtonHeight = buttonHeight;
	int smallButtonWidth = smallButtonHeight;
	int color = 3;
	
	// Calculate total height and width of all buttons and spaces
    int totalHeight = 3 * buttonHeight + 2 * 5;  // 3 buttons and 2 spaces
	int totalWidth = buttonWidth + 2 * (smallButtonWidth + 5);  // 1 button and 2 small buttons

    // Draw buttons and labels
	for (int i = 0; i < 3; i++) 
	{
		// Calculate button position
		int buttonX = center_x - totalWidth / 2;
		int buttonY = center_y - totalHeight / 2 + i * (buttonHeight + 5);

		// Draw button border
		if (i == selectedButtonHeightIndex && selectedButtonWidthIndex == 0)
		{
			color = 1;
		}
		else
		{
			color = 3;
		}
		DrawEmptyRectangle(buttonX - 2, buttonY - 2, buttonWidth + 4, buttonHeight + 4, color);
		// Draw small buttons if the big button has been clicked
		if (i == bigButtonClickedIndex || bigButtonClickedIndex == -1)
		{
			for (int j = 0; j < 3; j++) 
			{
				int smallButtonX = (buttonX + buttonWidth + 5 + j * (smallButtonWidth + 5));
				int smallButtonY = buttonY;

				// Draw small button border
				if (bigButtonClickedIndex != -1) // Only draw the border if a big button has been clicked
				{
					if (j + 1 == selectedButtonWidthIndex && i == selectedButtonHeightIndex)
					{
						color = 1;
					}
					else{
						color = 3;
					}
					// Draw borders of small buttons
					DrawEmptyRectangle(smallButtonX - 2, smallButtonY - 2, smallButtonWidth + 4, smallButtonHeight + 4, color);

					// Draw small button icons
					
					// Calculate the center of the button
					int centerX = smallButtonX + smallButtonWidth / 2;
					int centerY = smallButtonY + smallButtonHeight / 2;
						
					if (j == 0)
					{
						// Draw the play icon
						DrawPlayIcon(centerX,centerY,color);
					}
					else if (j == 1)
					{
						// Edit button
						DrawEditButton(centerX,centerY,color);
					}
					else if (j == 2)
					{
						// Cross button
						DrawCrossSize(centerX,centerY,color);
					}
				}
				else
				{
					DrawNewRectangle(smallButtonX - 2, smallButtonY - 2, smallButtonWidth + 4, smallButtonHeight + 4, 0); // Color in black
				}
			}
		}
	}
}

void drawGameBackground(){
    for(int i=0; i<gameColumn;i++){
        for(int j=0;j<gameLine;j++){
            for(int k=0;k<vga_height/gameLine;k++){
                if((k+j*vga_height/gameLine)%2==0){
                    DrawNewRectangle(0+i*vga_width/gameColumn, k+j*vga_height/gameLine, 1, 1, i%2+1);
                }else{
                    DrawNewRectangle(vga_width/gameColumn-1+i*vga_width/gameColumn, k+j*vga_height/gameLine, 1, 1, i%2+1);
                }
            }
        }
        DrawEmptyRectangle(0+i*vga_width/gameColumn, vga_height-vga_height/gameLine, vga_width/gameColumn, vga_height/gameLine, 3);
        DrawNewRectangle(0+i*vga_width/gameColumn+1, vga_height-vga_height/gameLine+1, vga_width/gameColumn-2, vga_height/gameLine-2, 0);
    }
}

void affiche_tab(){
    printf("tab\n");
    for(int i = 0;i<sizeof(lineMusic)/4;i++){
        printf("%d ",lineMusic[i]);
        if(i/gameColumn==vga_height-1-reverseMusicPosition){
            printf(" %d ",i);
        }
        if(i%gameColumn==7){
            printf("\n");
        }
    }
}

int* getColumnValue(){
    if(fileLine>=sizeof(lineMusic)/4/gameColumn){
        for(int i = 0;i<gameColumn;i++){
            new_line[i]=0;
        }
        fileLine++;
        return new_line;
    }
    for(int i = 0;i<sizeof(lineMusic)/4;i++){
		if(i/gameColumn==fileLine){
        	new_line[i%gameColumn]=lineMusic[i];
		}
    }
    fileLine++;
    return new_line;
}

void drawGameNote(int x,int y,int value){
    if(value==1){
		int new_height = 5;
		if(y==0 || y==vga_height-1){
			new_height = 1;
			y=y+2;
		}
		else if(y==1 || y==vga_height-2){
			new_height = 2;
			if(y==1){
				y=y+1;
			}else{
				y=y+2;
			}
		}
		else if(y==2 || y==vga_height-3){
			new_height = 3;
			if(y==vga_height-3){
				y=y+2;
			}
		}
		else if(y==3 || y==vga_height-4){
			new_height = 4;
			if(y==vga_height-4){
				y=y+2;
			}
		}
		y=y-2;
        DrawNewRectangle(x+1,y,vga_width/gameColumn-2,new_height,(x/(vga_width/gameColumn)+1)%2+1);
		if((y>0 && new_height==5) || y>5){
			int pos = y-5;
			if(pos<0){
				new_height = new_height+pos;
				pos=0;
			}
			DrawNewRectangle(x+1,pos,vga_width/gameColumn-2,new_height,0);
		}
    }
}

void drawGameMusic(int position){
    int length = vga_height*gameColumn;
	int* line = NULL;
	if(position%((vga_height/gameLine)*2)==0){
			line = getColumnValue();
	}
    for(int i = 0;i<sizeof(tabMusic)/4;i++){
		if(i/gameColumn==position && line){
            tabMusic[i]=line[i%gameColumn];
        }
		drawGameNote(i%gameColumn*vga_width/gameColumn,(length+i/gameColumn-position)%vga_height,tabMusic[i]);
    }
}

void selectGameColumn(int input){
	int y = vga_height-1-reverseMusicPosition;
    if(tabMusic[(y-2)%vga_height*gameColumn+input] \
	|| tabMusic[(y-1)%vga_height*gameColumn+input] \
	|| tabMusic[(y)%vga_height*gameColumn+input] \
	|| tabMusic[(y+1)%vga_height*gameColumn+input] \
	|| tabMusic[(y+2)%vga_height*gameColumn+input]){
        score++;
		printf("%d\n",score);
    }
    DrawEmptyRectangle(0+input*vga_width/gameColumn, vga_height-vga_height/gameLine, vga_width/gameColumn, vga_height/gameLine, input%2+1);
}

void drawScore(){
	//printf("%d\n",score*100/max_score);
}
void playGame(int input){
    drawGameBackground();
    if(input){
        input=input-1;
        selectGameColumn(input);
    }
	drawGameMusic(vga_height-1-reverseMusicPosition);
	reverseMusicPosition = (reverseMusicPosition+1)%vga_height;
	drawScore();
}

int getInput()
{
	if(IsKeyDown(KEY_Q)){
        return 1;
    }
    if(IsKeyDown(KEY_W)){
        return 2;
    }
    if(IsKeyDown(KEY_E)){
        return 3;
    }
    if(IsKeyDown(KEY_R)){
        return 4;
    }
    if(IsKeyDown(KEY_T)){
        return 5;
    }
    if(IsKeyDown(KEY_Y)){
        return 6;
    }
    if(IsKeyDown(KEY_U)){
        return 7;
    }
    if(IsKeyDown(KEY_I)){
        return 8;
    }
    if(IsKeyDown(KEY_O)){
        return 9;
    }
	return 0;
}

void getInputKey()
{
	if (IsKeyPressed(KEY_UP))
	{
		if (bigButtonClickedIndex == -1)
		{
			selectedButtonHeightIndex--;
			selectedButtonHeightIndex = (selectedButtonHeightIndex > 2) ? 0 : (selectedButtonHeightIndex < 0) ? 2 : selectedButtonHeightIndex;
		}
	}
	if (IsKeyPressed(KEY_DOWN))
	{
		if (bigButtonClickedIndex == -1)
		{
			selectedButtonHeightIndex++;
			selectedButtonHeightIndex = (selectedButtonHeightIndex > 2) ? 0 : (selectedButtonHeightIndex < 0) ? 2 : selectedButtonHeightIndex;
		}
	}
	if (IsKeyPressed(KEY_LEFT))
	{
		if (bigButtonClickedIndex != -1)
		{
			selectedButtonWidthIndex--;
			selectedButtonWidthIndex = (selectedButtonWidthIndex > 3) ? 0 : (selectedButtonWidthIndex < 0) ? 3 : selectedButtonWidthIndex;
		}
	}
	if (IsKeyPressed(KEY_RIGHT))
	{
		if (bigButtonClickedIndex != -1)
		{
			selectedButtonWidthIndex++;
			selectedButtonWidthIndex = (selectedButtonWidthIndex > 3) ? 0 : (selectedButtonWidthIndex < 0) ? 3 : selectedButtonWidthIndex;
		}
	}
	if (IsKeyPressed(KEY_ENTER))
	{
		if (bigButtonClickedIndex == selectedButtonHeightIndex && selectedButtonWidthIndex == 0)
        {
            bigButtonClickedIndex = -1;  // Deselect the button if it was already selected
        }
        else
        {
            bigButtonClickedIndex = selectedButtonHeightIndex;

			if (selectedButtonWidthIndex == 1) // Play button
			{
				pageStateChanged = 1;
				pageState = 2;
			}
			if (selectedButtonWidthIndex == 2) // Edit button
			{
				pageStateChanged = 1;
				pageState = 3;
			}
        }
	}
}

int isPlaying = 0;
void getInputTable()
{
	if (IsKeyPressed(KEY_LEFT))
	{
		if(!isPlaying){
			if(selectButtonModifyIndex!=0){
				selectButtonModifyIndex--;
			}
		}
	}
	if (IsKeyPressed(KEY_RIGHT))
	{
		if(!isPlaying){
			if(selectButtonModifyIndex!=2){
				selectButtonModifyIndex++;
			}
		}
	}
	if (IsKeyPressed(KEY_ENTER))
	{
		if(selectButtonModifyIndex==0){
			pageStateChanged = 1;
			pageState = 1;
		}
		if(selectButtonModifyIndex==1){
			if(isPlaying){
				isPlaying=0;
				pageStateChanged = 1;
			}
			else{
				for(int i = 0;i<sizeof(lineMusic)/4;i++){
					lineMusic[i]=0;
				}
				isPlaying=1;
				pageStateChanged = 1;
			}
		}
		if(selectButtonModifyIndex==2){
			for(int i = 0;i<sizeof(lineMusic)/4;i++){
				lineMusic[i]=0;
			}
			pageStateChanged = 1;
		}
	}
}

int get_max_score(){
    int new_score = 0;
    for(int i=0;i<sizeof(lineMusic)/4;i++){
        if(lineMusic[i]==1){
            new_score++;
        }
    }
    return new_score;
}

void drawGame(){
    if(fileLine==0){
		max_score = get_max_score();
    }
    if(fileLine==sizeof(lineMusic)/4/gameColumn+gameLine){
		fileLine=0;
		pageStateChanged = 1;
        pageState=1;
    }
    int input = getInput();
    playGame(input);
}

#define button_modify_size 9
#define button_modify_default_x 10
#define button_modify_default_y 50
void drawTableBackground(){
	for(int i = 1;i<6;i++){
		DrawNewRectangle(0,i*6,vga_width,1,3);
	}
	for(int j = 0;j<3;j++){
		int color = 3;
		if(selectButtonModifyIndex==j){
			color = 1;
		}
		if(j==0){
			DrawHouse(button_modify_default_x+50*j,button_modify_default_y,color);
		}
		if(j==1){
			if(!isPlaying){
				DrawPlayIcon(button_modify_default_x+50*j,button_modify_default_y,color);
			}else{
				DrawStopIcon(button_modify_default_x+50*j,button_modify_default_y,color);				
			}
		}
		if(j==2){
			DrawCrossSize(button_modify_default_x+50*j,button_modify_default_y,color);
		}
		DrawEmptyRectangle(button_modify_default_x+50*j-4,button_modify_default_y-4,button_modify_size,button_modify_size,color);

	}
}

void drawTableNote(int x,int value){
    drawMusicNote(x,27-value*(vga_height/gameLine-2),2);
}

int barPosition = 0;
void drawTableBar(){
	if(!isPlaying){
		barPosition=0;
		DrawNewRectangle(0,0,1,vga_height/gameLine*6+1,1);
	}
	else{
		DrawNewRectangle((barPosition-1+vga_width)%vga_width,0,1,vga_height/gameLine*6+1,0);
		DrawNewRectangle(barPosition,0,1,vga_height/gameLine*6+1,1);
		barPosition=(barPosition+1)%vga_width;
	}
}
int indentTable = 0;
void drawTableMusic(){
	drawTableBar();
	int notePosition = 5;
	for(int i = 0;i<sizeof(lineMusic)/4;i++){
		if(i%gameColumn==0){
			notePosition = notePosition + 5;
		}
		if(lineMusic[i]==1 && notePosition<vga_width*(indentTable+1) && i>=indentTable*vga_width/5*gameColumn && i<(indentTable+1)*vga_width/5*gameColumn){
			drawTableNote(notePosition%vga_width,i%gameColumn);
		}
	}
}

void modifyTableMusic(int indent){
	int value = getInput();
	for(int i = 0;i<gameColumn;i++){
		if(value ==i+1){
			lineMusic[indent*gameColumn+i] = 1;
		}
		else{
			lineMusic[indent*gameColumn+i] = 0;
		}
	}
}
int tableMusicLine = 0;
void drawTable(){
	getInputTable();
	if(isPlaying && tableMusicLine!=sizeof(lineMusic)/4/gameColumn){
		if(barPosition==vga_width-1){
			indentTable = indentTable + 1;
			pageStateChanged = 1;
		}
		if(barPosition%5==0){
			modifyTableMusic(tableMusicLine);
			tableMusicLine++;
		}
	}
	else{
		indentTable = 0;
		isPlaying = 0;
		tableMusicLine = 0;
		pageStateChanged = 1;
	}
	drawTableBackground();
	drawTableMusic();
}

void clear_page(){
    DrawNewRectangle(0,0,vga_width,vga_height,0);
}

int main(int argc, char* argv[])
{	
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rythmo");
	SetTargetFPS(60);

	BeginDrawing();
	{
		
	}
	EndDrawing();

	while(!WindowShouldClose())
	{
		BeginDrawing();
		{
			if (pageStateChanged == 1)
			{
				pageStateChanged = 0;
				clear_page();
			}
			if(pageState==1){
				getInputKey();
				drawMenu(vga_width / 2, vga_height / 2);
				}
			else if (pageState==2)
			{
				drawGame();
			}
			else if (pageState == 3)
			{
            	drawTable();
			}
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
