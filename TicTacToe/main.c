#include <stdio.h>
 
char table[3][3];
 
void InitializeTable()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            table[i][j] = ' ';
        }
    }
    int count = 1;
    printf("\n\n\t  ");
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            printf("%d", count++);
            if (j < 2)
            {
                printf("  |  ");
            }
        }
        if (i < 2)
        printf("\n\t----------------\n\t  ");
    }
    printf("\n\n\n");
}
 
void ShowTable(int x, int y)
{
    printf("\n\n\t  ");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%c", table[i][j]);
            if (j < 2)
            {
                printf("  |  ");
            }
        }
        if (i < 2)
        printf("\n\t----------------\n\t  ");
    }
    printf("\n\n\n");
}

int UpdateTable(int cell, char playerSign)
{
    int row = (cell - 1) / 3;
    int column = (cell - 1) % 3;
    int isValid = 1;
 
    if (table[row][column] != ' ')
    {
        printf("\nCell is already Filled!\n");
        isValid = 0;
    }
 
    else
    {
        table[row][column] = playerSign;
    }
    ShowTable(row, column);
 
    return isValid;
}
 
int PlayerWon(char sg)
{
    if (table[0][0] == sg && table[0][1] == sg && table[0][2] == sg ||
      table[1][0] == sg && table[1][1] == sg && table[1][2] == sg ||
      table[2][0] == sg && table[2][1] == sg && table[2][2] == sg)
    {
        return 1;
      }
    else if (table[0][0] == sg && table[1][0] == sg && table[2][0] == sg ||
           table[0][1] == sg && table[1][1] == sg && table[2][1] == sg ||
           table[0][2] == sg && table[1][2] == sg && table[2][2] == sg)
    {
        return 1;
    }
    else if (table[0][0] == sg && table[1][1] == sg && table[2][2] == sg ||
           table[0][2] == sg && table[1][1] == sg && table[2][0] == sg)
    {
        return 1;
    }
    return 0;
}
 
void Play()
{
    int gameResult = 0;
    int cell = 0;
    int playCount = 0;
    int updationResult = 1;
 
    char playerSign = ' ';
 
    while (!gameResult && playCount < 9)
    {
        if (playCount % 2 == 0)
        {
            printf("\nPlayer 1 [ X ] : ");
            playerSign = 'X';
        }
        else
        {
            printf("\nPlayer 2 [ O ] : ");
            playerSign = 'O';
        }
        scanf("%d", &cell);
        if (cell > 0 && cell < 10)
        {
            updationResult = UpdateTable(cell, playerSign);
            if (updationResult)
            {
                gameResult = PlayerWon(playerSign);
                if (gameResult)
                {
                    printf("\t *** Player %d Won!! ***\n", playerSign == 'X' ? 1 : 2);
                }
                playCount++;
            }
        }
        else if (cell == -1)
        {
            printf("\n\tGame Ended\n");
            return;
        }
        else
        {
            printf("\nPlease Enter a valid cell value\n");
        }
    }
 
    if (!gameResult && playCount == 9)
    {
        printf("\n\t *** Draw...  ***\n");
    }
    printf("\n\t --- Game Over --- \n");
}
 
int main()
{
    printf("\n\n* Cell Numbers are the positions on Board\n");
    InitializeTable();
 
    char start = ' ';
    printf("\nPress Enter to start!");
 
    scanf("%c", &start);
 
    if (start)
    {
        int choice = 1;
        while (choice)
        {
            Play();
            printf("\nType 1 to Restart");
            printf("\nType 0 for Exit");
            printf("\n\nChoice: ");
            scanf("%d", &choice);
            if (choice)
            {
                InitializeTable();
            }
            printf("\n");
        }
    }
    return 0;
}