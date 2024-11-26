#include <VGAX.h>

byte frame_counter = 0;

#define vga_width    120
#define vga_height    60
#define vga_pixel_size 12

VGAX vga;

#define hautParleurPin 8
#define BleuHautPin 1
#define BleuBasPin 0
#define VertHautPin 4
#define VertBasPin 2
#define BlancPin 5
#define JauneHautPin 10
#define JauneBasPin 11
#define RougeHautPin 12
#define RougeBasPin 13

byte VertHautValue = 0;
byte VertBasValue = 0;
byte JauneHautValue = 0;
byte JauneBasValue = 0;
byte RougeHautValue = 0;
byte RougeBasValue = 0;
byte BleuHautValue = 0;
byte BleuBasValue = 0;
byte BlancValue = 0;

byte BlancPressed = 0;
byte VertHautPressed = 0;
byte VertBasPressed = 0;
byte JauneHautPressed = 0;
byte JauneBasPressed = 0;
byte RougeHautPressed = 0;
byte RougeBasPressed = 0;
byte BleuHautPressed = 0;
byte BleuBasPressed = 0;

#define SCREEN_WIDTH vga_width;
#define SCREEN_HEIGHT vga_height;

byte selectedButtonWidthIndex = 0;
byte selectedButtonHeightIndex = 0;
int selectButtonModifyIndex = 0;

byte bigButtonClickedIndex = 9;

byte pageState = 1;

#define gameColumn 8
#define gameLine 12

byte reverseMusicPosition = gameLine;
byte fileLine = 0;
byte max_score = 0;
byte score = 0;
byte new_line[gameColumn];
byte tabMusic[gameColumn*gameLine];
byte lineMusic[4*gameLine] = {
    00000000,
    00000000,
    00000000,
    00000000,
    00010000,
    00001000,
    00110000,
    00000100,
    01010000,
    00000010,
    10010000,
    00000001,
	00000000,
    00010000,
    00000000,
    00000000,
    00010000,
    00001000,
    00100000,
    00000100,
    01010000,
    00000010,
    10010000,
    00000001,
	00000000,
    00000000,
    00000000,
    00000000,
    00010000,
    00001000,
    00100000,
    00000100,
    01000000,
    00000010,
    10000000,
    00000001,
	00000000,
    00000000,
    00000000,
    00000000,
    00010000,
    00001000,
    00100000,
    00000100,
    01000000,
    00000010,
    10000000,
    00000001,
};

byte pageStateChanged = 0;


#define button_modify_size 9
#define button_modify_default_x 10
#define button_modify_default_y 50
int isPlaying = 0;
int barPosition = 0;
int indentTable = 0;
int tableMusicLine = 0;

void DrawNewRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color)
{
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            vga.putpixel(x+i,y+j,color);
        }
    }
}

void DrawEmptyRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color)
{
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            if(i==0 || i==width-1 || j==0 || j==height-1){
                vga.putpixel(x+i,y+j,color);
            }
        }
    }
}

void drawMenu(int center_x, int center_y)
{
		// Draw the title
	DrawTitle();

    // Define button dimensions
    byte buttonWidth = 20;
    byte buttonHeight = 5;
 	byte smallButtonHeight = buttonHeight;
	byte smallButtonWidth = smallButtonHeight;
	byte color = 3;
	
	// Calculate total height and width of all buttons and spaces
    byte totalHeight = 3 * buttonHeight + 2 * 5;  // 3 buttons and 2 spaces
	byte totalWidth = buttonWidth + 2 * (smallButtonWidth + 5);  // 1 button and 2 small buttons

    // Draw buttons and labels
	for (int i = 0; i < 3; i++) 
	{
		// Calculate button position
		byte buttonX = center_x - totalWidth / 2;
		byte buttonY = center_y - totalHeight / 2 + i * (buttonHeight + 5);

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
				byte smallButtonX = (buttonX + buttonWidth + 5 + j * (smallButtonWidth + 5));
				byte smallButtonY = buttonY;

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
					byte centerX = smallButtonX + smallButtonWidth / 2;
					byte centerY = smallButtonY + smallButtonHeight / 2;
						
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

int get_mult(int j){
	int mult = 1;
	for(int k = 0;k<=j;k++){
		mult = mult*2;
	}
	return mult;
}

byte* getColumnValue(){
    if(fileLine>=sizeof(lineMusic)){
        for(int i = 0;i<gameColumn;i++){
            new_line[i]=0;
        }
        fileLine++;
        return new_line;
    }
    for(int i = 0;i<sizeof(lineMusic);i++){
		for(int j = 0;j<gameColumn;j++){
			if(i==fileLine){
        		new_line[i%gameColumn]=lineMusic[i]/get_mult(j)%2;
			}
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


void drawMusicNote(unsigned int center_x, unsigned int center_y,unsigned int color){
	int bottom = -2;
	int top = 2;
	for(int i = bottom; i<top+1;i++){
		for(int j = bottom; j<top+1;j++){
			if(((i==bottom || i==top) && j!=bottom && j!=top) 
			|| ((j==bottom || j==top) && i!=bottom && i!=top)){
				vga.putpixel(center_x+j,center_y+i,color);
			}
		}
	}
}

void drawGameMusic(int position){
    int length = vga_height*gameColumn;
	byte* line = NULL;
	if(position%((vga_height/gameLine)*2)==0){
			line = getColumnValue();
	}
    for(int i = 0;i<sizeof(tabMusic);i++){
		int new_value = 0;
		for(int j = 0;j<gameColumn;j++){
			if(i==position && line){
            	new_value=line[j]*get_mult(j);
        	}
		}
		tabMusic[i] = new_value;
		for(int j = 0;j<gameColumn;j++){
			drawGameNote(j%gameColumn*vga_width/gameColumn,(length+i-position)%vga_height,tabMusic[i]/get_mult(j)%2);
		}
    }
}

void selectGameColumn(int input){
	int y = vga_height-1-reverseMusicPosition;
    if(tabMusic[(y-2)%vga_height]/get_mult(input)%2 \
	|| tabMusic[(y-1)%vga_height]/get_mult(input)%2 \
	|| tabMusic[(y)%vga_height]/get_mult(input)%2 \
	|| tabMusic[(y+1)%vga_height]/get_mult(input)%2 \
	|| tabMusic[(y+2)%vga_height]/get_mult(input)%2){
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
	BlancPressed = 0;
	VertHautPressed = 0;
	VertBasPressed = 0;
	JauneHautPressed = 0;
	JauneBasPressed = 0;
	RougeHautPressed = 0;
	RougeBasPressed = 0;
	BleuHautPressed = 0;
	BleuBasPressed = 0;

	BleuBasValue = digitalRead(BleuBasPin);
	if (BleuBasValue == LOW and !BleuBasPressed) 
	{
		BleuBasPressed = 1;
		return 1;
	}
	BleuHautValue = digitalRead(BleuHautPin);
	if (BleuHautValue == LOW and !BleuHautPressed) 
	{
		BleuHautPressed = 1;
		return 2;
	}
	VertBasValue = digitalRead(VertBasPin);
	if (VertBasValue == LOW and !VertBasPressed) 
	{
		VertBasPressed = 1;
		return 3;
	}
	VertHautValue = digitalRead(VertHautPin);
	if (VertHautValue == LOW and !VertHautPressed) 
	{
		VertHautPressed = 1;
		return 4;
	}
	JauneBasValue = digitalRead(JauneBasPin);
	if (JauneBasValue == LOW and !JauneBasPressed) 
	{
		JauneBasPressed = 1;
		return 5;
	}
	JauneHautValue = digitalRead(JauneHautPin);
	if (JauneHautValue == LOW and !JauneHautPressed) 
	{
		JauneHautPressed = 1;
		return 6;
	}
	RougeBasValue = digitalRead(RougeBasPin);
	if (RougeBasValue == LOW and !RougeBasPressed) 
	{
		RougeBasPressed = 1;
		return 7;
	}
	RougeHautValue = digitalRead(RougeHautPin);
	if (RougeHautValue == LOW and !RougeHautPressed) 
	{
		RougeHautPressed = 1;
		return 8;
	}
	BlancValue = digitalRead(BlancPin);
	if (BlancValue == LOW and !BlancPressed) 
	{
		BlancPressed = 1;
		return 9;
	}
	return 0;
}

void getInputKey()
{
	BlancPressed = 0;
	VertHautPressed = 0;
	VertBasPressed = 0;
	JauneHautPressed = 0;
	JauneBasPressed = 0;
	RougeHautPressed = 0;
	RougeBasPressed = 0;
	BleuHautPressed = 0;
	BleuBasPressed = 0;

	JauneHautValue = digitalRead(JauneHautPin);
	if (JauneHautValue == LOW and !JauneHautPressed) // haut
	{
		if (bigButtonClickedIndex == 9)
		{
			selectedButtonHeightIndex--;
			selectedButtonHeightIndex = (selectedButtonHeightIndex > 2) ? 0 : (selectedButtonHeightIndex < 0) ? 2 : selectedButtonHeightIndex;
		}
		JauneHautPressed = 1;
	}
	JauneBasValue = digitalRead(JauneBasPin);
	if (JauneBasValue == LOW and !JauneBasPressed) // gauche
	{
		if (bigButtonClickedIndex != 9)
		{
			selectedButtonWidthIndex--;
			selectedButtonWidthIndex = (selectedButtonWidthIndex > 3) ? 0 : (selectedButtonWidthIndex < 0) ? 3 : selectedButtonWidthIndex;
		}
		JauneBasPressed = 1;
	}
	VertHautValue = digitalRead(VertHautPin);
	if (VertHautValue == LOW and !VertHautPressed) // bas
	{
		if (bigButtonClickedIndex == 9)
		{
			selectedButtonHeightIndex++;
			selectedButtonHeightIndex = (selectedButtonHeightIndex > 2) ? 0 : (selectedButtonHeightIndex < 0) ? 2 : selectedButtonHeightIndex;
		}
		VertHautPressed = 1;
	}
  	VertBasValue = digitalRead(VertBasPin);
	if (VertBasValue == LOW and !VertBasPressed) // droite
	{
		if (bigButtonClickedIndex != 9)
		{
			selectedButtonWidthIndex++;
			selectedButtonWidthIndex = (selectedButtonWidthIndex > 3) ? 0 : (selectedButtonWidthIndex < 0) ? 3 : selectedButtonWidthIndex;
		}
		VertBasPressed = 1;
	}
	BlancValue = digitalRead(BlancPin);
	if (BlancValue == LOW and !BlancPressed) // enter
	{
	  if (bigButtonClickedIndex == selectedButtonHeightIndex && selectedButtonWidthIndex == 0)
    {
        bigButtonClickedIndex = 9;  // Deselect the button if it was already selected
    }
    else
    {
      bigButtonClickedIndex = selectedButtonHeightIndex;

      if (selectedButtonWidthIndex == 1) // Play button
      {
        pageState = 2;
        pageStateChanged = 1;
      }
      if (selectedButtonWidthIndex == 2) // Edit button
      {
        pageState = 3;
      }
    }
	BlancPressed = 1;
	}
}

void DrawSelectedRectangle(int x, int y, int width, int height, int color)
{
	for(int i=0;i<width;i++){
		for(int j=0;j<height;j++){
			if(i==0 || i==width-1 || j==0 || j==height-1){
				vga.putpixel(x+i,y+j,color);
			}
		}
	}
}


void DrawPlayIcon(int centerX,int centerY,int color){
	// Draw the play icon
	vga.putpixel(centerX - 1, centerY - 2, color);
	vga.putpixel(centerX - 1, centerY - 1, color);
	vga.putpixel(centerX - 1, centerY, color);
	vga.putpixel(centerX - 1, centerY + 1, color);
	vga.putpixel(centerX - 1, centerY + 2, color);
	vga.putpixel(centerX, centerY - 1, color);
	vga.putpixel(centerX + 1, centerY, color);
	vga.putpixel(centerX, centerY + 1, color);
	vga.putpixel(centerX, centerY, color);
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
		vga.putpixel(centerX - 2, centerY + k, color);
		vga.putpixel(centerX, centerY + k, color);
		vga.putpixel(centerX + 1, centerY + k, color);
		vga.putpixel(centerX + 2, centerY + k, color);
	}
}

void DrawCrossSize(int centerX,int centerY,int color){
	// Cross button
	// Define the size of the cross
	int crossSize = 5 / 2;  // Adjust this value to change the size of the cross
	// Draw the cross
	for (int i = -crossSize / 2 - 1; i <= crossSize / 2 + 1; i++)
	{
		vga.putpixel(centerX + i, centerY + i, color);
		vga.putpixel(centerX + i, centerY - i, color);
	}
}

void DrawHouse(int centerX,int centerY,int color){
	vga.putpixel(centerX, centerY - 2, color);
	vga.putpixel(centerX - 1, centerY - 1, color);
	vga.putpixel(centerX, centerY - 1, color);
	vga.putpixel(centerX + 1, centerY - 1, color);
	vga.putpixel(centerX - 2, centerY, color);
	vga.putpixel(centerX - 1, centerY, color);
	vga.putpixel(centerX, centerY, color);
	vga.putpixel(centerX + 1, centerY, color);
	vga.putpixel(centerX + 2, centerY, color);
	vga.putpixel(centerX - 1, centerY + 1, color);
	vga.putpixel(centerX + 1, centerY + 1, color);
	vga.putpixel(centerX - 1, centerY + 2, color);
	vga.putpixel(centerX, centerY + 2, color);
	vga.putpixel(centerX + 1, centerY + 2, color);
}

void DrawTitle()
{
	// Draw the title
	byte margin = 32;
	// R letter
	vga.putpixel(10 + margin, 5, 1);
	vga.putpixel(10 + margin, 5, 1);
	vga.putpixel(11 + margin, 5, 1);
	vga.putpixel(12 + margin, 5, 1);
	vga.putpixel(13 + margin, 5, 1);
	vga.putpixel(14 + margin, 6, 1);
	vga.putpixel(14 + margin, 7, 1);
	vga.putpixel(14 + margin, 8, 1);
	vga.putpixel(13 + margin, 9, 1);
	vga.putpixel(12 + margin, 9, 1);
	vga.putpixel(11 + margin, 9, 1);
	vga.putpixel(10 + margin, 6, 1);
	vga.putpixel(10 + margin, 7, 1);
	vga.putpixel(10 + margin, 8, 1);
	vga.putpixel(10 + margin, 9, 1);
	vga.putpixel(10 + margin, 10, 1);
	vga.putpixel(10 + margin, 11, 1);
	vga.putpixel(10 + margin, 12, 1);
	vga.putpixel(10 + margin, 13, 1);
	vga.putpixel(11 + margin, 10, 1);
	vga.putpixel(12 + margin, 11, 1);
	vga.putpixel(13 + margin, 12, 1);
	vga.putpixel(14 + margin, 13, 1);

	// Y letter
	vga.putpixel(17 + margin, 5, 1);
	vga.putpixel(17 + margin, 6, 1);
	vga.putpixel(17 + margin, 7, 1);
	vga.putpixel(18 + margin, 8, 1);
	vga.putpixel(18 + margin, 9, 1);
	vga.putpixel(20 + margin, 9, 1);
	vga.putpixel(20 + margin, 8, 1);
	vga.putpixel(21 + margin, 7, 1);
	vga.putpixel(21 + margin, 6, 1);
	vga.putpixel(21 + margin, 5, 1);
	vga.putpixel(19 + margin, 10, 1);
	vga.putpixel(19 + margin, 11, 1);
	vga.putpixel(19 + margin, 12, 1);
	vga.putpixel(19 + margin, 13, 1);

	// T letter
	vga.putpixel(24 + margin, 5, 1);
	vga.putpixel(25 + margin, 5, 1);
	vga.putpixel(26 + margin, 5, 1);
	vga.putpixel(27 + margin, 5, 1);
	vga.putpixel(28 + margin, 5, 1);
	vga.putpixel(26 + margin, 6, 1);
	vga.putpixel(26 + margin, 7, 1);
	vga.putpixel(26 + margin, 8, 1);
	vga.putpixel(26 + margin, 9, 1);
	vga.putpixel(26 + margin, 10, 1);
	vga.putpixel(26 + margin, 11, 1);
	vga.putpixel(26 + margin, 12, 1);
	vga.putpixel(26 + margin, 13, 1);

	vga.putpixel(31 + margin, 5, 1);
	vga.putpixel(31 + margin, 6, 1);
	vga.putpixel(31 + margin, 7, 1);
	vga.putpixel(31 + margin, 8, 1);
	vga.putpixel(31 + margin, 9, 1);
	vga.putpixel(31 + margin, 10, 1);
	vga.putpixel(31 + margin, 11, 1);
	vga.putpixel(31 + margin, 12, 1);
	vga.putpixel(31 + margin, 13, 1);
	vga.putpixel(32 + margin, 9, 1);
	vga.putpixel(33 + margin, 9, 1);
	vga.putpixel(34 + margin, 9, 1);
	vga.putpixel(35 + margin, 5, 1);
	vga.putpixel(35 + margin, 6, 1);
	vga.putpixel(35 + margin, 7, 1);
	vga.putpixel(35 + margin, 8, 1);
	vga.putpixel(35 + margin, 9, 1);
	vga.putpixel(35 + margin, 10, 1);
	vga.putpixel(35 + margin, 11, 1);
	vga.putpixel(35 + margin, 12, 1);
	vga.putpixel(35 + margin, 13, 1);

	// M letter
	vga.putpixel(38 + margin, 5, 1);
	vga.putpixel(38 + margin, 6, 1);
	vga.putpixel(38 + margin, 7, 1);
	vga.putpixel(38 + margin, 8, 1);
	vga.putpixel(38 + margin, 9, 1);
	vga.putpixel(38 + margin, 10, 1);
	vga.putpixel(38 + margin, 11, 1);
	vga.putpixel(38 + margin, 12, 1);
	vga.putpixel(38 + margin, 13, 1);
	vga.putpixel(39 + margin, 5, 1);
	vga.putpixel(40 + margin, 6, 1);
	vga.putpixel(40 + margin, 7, 1);
	vga.putpixel(41 + margin, 8, 1);
	vga.putpixel(41 + margin, 9, 1);
	vga.putpixel(42 + margin, 7, 1);
	vga.putpixel(42 + margin, 6, 1);
	vga.putpixel(43 + margin, 5, 1);
	vga.putpixel(44 + margin, 5, 1);
	vga.putpixel(44 + margin, 6, 1);
	vga.putpixel(44 + margin, 7, 1);
	vga.putpixel(44 + margin, 8, 1);
	vga.putpixel(44 + margin, 9, 1);
	vga.putpixel(44 + margin, 10, 1);
	vga.putpixel(44 + margin, 11, 1);
	vga.putpixel(44 + margin, 12, 1);
	vga.putpixel(44 + margin, 13, 1);

	// O letter
	vga.putpixel(47 + margin, 5, 1);
	vga.putpixel(47 + margin, 6, 1);
	vga.putpixel(47 + margin, 7, 1);
	vga.putpixel(47 + margin, 8, 1);
	vga.putpixel(47 + margin, 9, 1);
	vga.putpixel(47 + margin, 10, 1);
	vga.putpixel(47 + margin, 11, 1);
	vga.putpixel(47 + margin, 12, 1);
	vga.putpixel(47 + margin, 13, 1);
	vga.putpixel(48 + margin, 5, 1);
	vga.putpixel(49 + margin, 5, 1);
	vga.putpixel(50 + margin, 5, 1);
	vga.putpixel(51 + margin, 5, 1);
	vga.putpixel(51 + margin, 6, 1);
	vga.putpixel(51 + margin, 7, 1);
	vga.putpixel(51 + margin, 8, 1);
	vga.putpixel(51 + margin, 9, 1);
	vga.putpixel(51 + margin, 10, 1);
	vga.putpixel(51 + margin, 11, 1);
	vga.putpixel(51 + margin, 12, 1);
	vga.putpixel(51 + margin, 13, 1);
	vga.putpixel(48 + margin, 13, 1);
	vga.putpixel(49 + margin, 13, 1);
	vga.putpixel(50 + margin, 13, 1);
}

void drawGame(){
    if(fileLine==0){
		max_score = get_max_score();
    }
    if(fileLine==sizeof(lineMusic)+gameLine){
		fileLine=0;
		pageStateChanged = 1;
        pageState=1;
    }
    int input = getInput();
    playGame(input);
}

void clear_page(){
    DrawNewRectangle(0,0,vga_width,vga_height,0);
}

void setup() {
  vga.begin();
  vga.clear(00);

  pinMode(BleuHautPin, INPUT_PULLUP);
  pinMode(BleuBasPin, INPUT_PULLUP);
  pinMode(VertHautPin, INPUT_PULLUP);
  pinMode(VertBasPin, INPUT_PULLUP);
  pinMode(BlancPin, INPUT_PULLUP);
  pinMode(JauneHautPin, INPUT_PULLUP);
  pinMode(JauneBasPin, INPUT_PULLUP);
  pinMode(RougeHautPin, INPUT_PULLUP);
  pinMode(RougeBasPin, INPUT_PULLUP);
}

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
void drawTableMusic(){
	drawTableBar();
	int notePosition = 5;
	for(int i = 0;i<sizeof(lineMusic);i++){
		for(int j = 0; j<gameColumn;j++){
			if(j%gameColumn==0){
				notePosition = notePosition + 5;
			}
			if(lineMusic[i]/get_mult(j)%2==1 && notePosition<vga_width*(indentTable+1) && i*gameColumn+j>=indentTable*vga_width/5*gameColumn && i*gameColumn+j<(indentTable+1)*vga_width/5*gameColumn){
				drawTableNote(notePosition%vga_width,i%gameColumn);
			}
		}
	}
}

void getInputTable()
{
    JauneBasValue = digitalRead(JauneBasPin);
    if (JauneBasValue == LOW and !JauneBasPressed) // gauche
    {
        if(!isPlaying){
            if(selectButtonModifyIndex!=0){
                selectButtonModifyIndex--;
            }
        }
        JauneBasPressed = 1;
    }
    VertBasValue = digitalRead(VertBasPin);
    if (VertBasValue == LOW and !VertBasPressed) // droite 
    {
        if(!isPlaying){
            if(selectButtonModifyIndex!=2){
                selectButtonModifyIndex++;
            }
        }
        VertBasPressed = 1;
    }
    BlancValue = digitalRead(BlancPin);
    if (BlancValue == LOW and !BlancPressed) // enter
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
                for(int i = 0;i<sizeof(lineMusic);i++){
                    lineMusic[i]=0;
                }
                isPlaying=1;
                pageStateChanged = 1;
            }
        }
        if(selectButtonModifyIndex==2){
            for(int i = 0;i<sizeof(lineMusic);i++){
                lineMusic[i]=0;
            }
            pageStateChanged = 1;
        }
        BlancPressed = 1;
    }
}

int get_max_score(){
    int new_score = 0;
    for(int i=0;i<sizeof(lineMusic);i++){
		for(int j = 0;j<gameColumn;j++){
        	if(lineMusic[i]/get_mult(j)%2==1){
            	new_score++;
        	}
		}
    }
    return new_score;
}

void modifyTableMusic(int indent){
	int value = getInput();
	int new_value = 0;
	for(int i = 0;i<gameColumn;i++){
		if(value ==i+1){
			new_value = new_value+get_mult(i);
		}
	}
	lineMusic[indent] = new_value;
}

void drawTable(){
	getInputTable();
	if(isPlaying && tableMusicLine!=sizeof(lineMusic)){
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

void loop() {
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