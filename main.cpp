#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define LENGTH 1000
#define DEPTH 100000
#define CHAR_BITS 8
#define WINDOW_HEIGHT 22
#define WINDOW_WIDTH 77

typedef enum
{
    ENCRYPT,
    DECRYPT

}DIRECTION;

typedef struct
{
    double x_point;
    double y_point;
}POINT_TYPE;

typedef struct
{
    POINT_TYPE top_left;
    POINT_TYPE top_right;
    POINT_TYPE bottom_left;
    POINT_TYPE bottom_right;
}FRACTAL_TYPE;

FRACTAL_TYPE set_fractal()
{
    FRACTAL_TYPE fractal;

    printf("top left x y: ");
    scanf("%lf %lf",&fractal.top_left.x_point, &fractal.top_left.y_point);
    while(getchar() != '\n');
    printf("top right x y: ");
    scanf("%lf %lf",&fractal.top_right.x_point, &fractal.top_right.y_point);
    while(getchar() != '\n');
    printf("bottom left x y: ");
    scanf("%lf %lf",&fractal.bottom_left.x_point, &fractal.bottom_left.y_point);
    while(getchar() != '\n');
    printf("bottom right x y: ");
    scanf("%lf %lf",&fractal.bottom_right.x_point, &fractal.bottom_right.y_point);
    while(getchar() != '\n');

    return fractal;
}

unsigned char rotate_left( unsigned char data, int rotate_bits)
{
    rotate_bits = rotate_bits % CHAR_BITS;
	return ((data << rotate_bits) | (data >> (CHAR_BITS-rotate_bits)));
}

unsigned char rotate_right( unsigned char data, int rotate_bits)
{
    rotate_bits = rotate_bits % CHAR_BITS;
	return ((data >> rotate_bits) | (data << (CHAR_BITS-rotate_bits)));
}

unsigned int calculate_point(double x0, double y0, int display)
{
    double x = 0;
    double y = 0;
    double x_temp;
    unsigned int iteration = 0;

    while (((x*x + y*y) < 4) &&  (iteration < DEPTH))
    {
        x_temp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = x_temp;

        iteration++;
    }

    x_temp = (x*x + y*y);

    if(display)
    {
        return iteration;
    }
    else
    {
        return (x_temp*2003)+iteration;
    }
}

double get_y(int ii, int jj, FRACTAL_TYPE fractal, int height, int width)
{
    double left_y_spaceing, right_y_spaceing, y_slope;

    left_y_spaceing = (fractal.top_left.y_point - fractal.bottom_left.y_point)/height;
    right_y_spaceing = (fractal.top_right.y_point - fractal.bottom_right.y_point)/height;
    y_slope = ((fractal.top_right.y_point-(right_y_spaceing * jj)) - (fractal.top_left.y_point-(left_y_spaceing * jj)))/width;
    return (fractal.top_left.y_point - (left_y_spaceing * jj) + (y_slope * ii));
}

double get_x(int ii, int jj, FRACTAL_TYPE fractal, int height, int width)
{
    double left_x_slope, right_x_slope, x_spacing;

    left_x_slope = (fractal.top_left.x_point - fractal.bottom_left.x_point)/height;
    right_x_slope = (fractal.top_right.x_point - fractal.bottom_right.x_point)/height;
    x_spacing = ((fractal.top_right.x_point-(right_x_slope * jj)) - (fractal.top_left.x_point-(left_x_slope * jj)))/width;
    return (fractal.top_left.x_point + (x_spacing * ii) - (left_x_slope * jj));
}

void crunch(unsigned char *data, FRACTAL_TYPE fractal, int root, DIRECTION direction)
{
    int ii,jj;
    unsigned int index = 0;
    double x_point, bx_point, y_point;

    for(jj = 0; jj < root; jj++)
    {
        for(ii = 0; ii < root*2; ii+=2)
        {
            y_point = get_y(ii, jj, fractal, root, root*2);
            x_point = get_x(ii, jj, fractal, root, root*2);
            bx_point = get_x(ii+1, jj, fractal, root, root*2);

            if(direction  == ENCRYPT)
            {
                data[index] = rotate_left(data[index],  calculate_point(x_point,y_point,0)) ^ calculate_point(bx_point,y_point,0);
            }
            else if(direction  == DECRYPT)
            {
                data[index] = rotate_right((data[index] ^  calculate_point(bx_point,y_point,0)), calculate_point(x_point,y_point,0));
            }

            index++;
            if((int)index > (root*root)-1)
            {
                break;
            }
        }
    }
}

void print_fractal(FRACTAL_TYPE fractal)
{
    unsigned int temp;
    int ii,jj,kk;
    double x_point, y_point;

    kk = 'a'+WINDOW_HEIGHT-1;

    for(jj = 0; jj < WINDOW_HEIGHT; jj++)
    {
		printf("%c", kk);
        kk--;
        for(ii = 0; ii < WINDOW_WIDTH; ii++)
		{
            y_point = get_y(ii, jj, fractal,WINDOW_HEIGHT,WINDOW_WIDTH);
            x_point = get_x(ii, jj, fractal,WINDOW_HEIGHT,WINDOW_WIDTH);

            temp = calculate_point(x_point,y_point,1);
            if(temp % 2)
            {
                printf(" ");
            }
            else
            {
                printf("*");
            }
        }
		printf("\n");
    }

    printf(" ");
    for(kk = 0; kk < 10; kk++)
    {
        printf("%d",kk);
    }

    for(kk = 10; kk <= WINDOW_WIDTH; kk++)
    {
        if((kk+5) % 10 == 0)
        {
            printf(" ");
        }
        else
        {
            printf("%d",kk / 10);
        }
    }
    printf("\n");
}

FRACTAL_TYPE search_fractal()
{
	int temp_x;
    char temp_y, garbage;
    FRACTAL_TYPE fractal, temp_fractal;

    fractal.top_left.y_point = 1.5;
    fractal.top_left.x_point = -2;
    fractal.top_right.y_point = 1.5;
    fractal.top_right.x_point = 1;
    fractal.bottom_left.y_point = -1.5;
    fractal.bottom_left.x_point = -2;
    fractal.bottom_right.y_point = -1.5;
    fractal.bottom_right.x_point = 1;

    temp_fractal = fractal;

	while(1)
	{
		print_fractal(fractal);
		printf("finished? y/n ");
        scanf("%c",&garbage);
       while(getchar() != '\n');
        if(garbage == 'y')
		{
			return fractal;
		}
        printf("top left y x: ");
        scanf("%c %d",&temp_y, &temp_x);
        while(getchar() != '\n');
        temp_fractal.top_left.y_point = get_y(temp_x, (97+WINDOW_HEIGHT-1 - temp_y), fractal,WINDOW_HEIGHT,WINDOW_WIDTH);
        temp_fractal.top_left.x_point = get_x(temp_x, (97+WINDOW_HEIGHT-1 - temp_y), fractal,WINDOW_HEIGHT,WINDOW_WIDTH);
        printf("top right y x: ");
        scanf("%c %d",&temp_y, &temp_x);
        while(getchar() != '\n');
        temp_fractal.top_right.y_point = get_y(temp_x, (97+WINDOW_HEIGHT-1 - temp_y), fractal,WINDOW_HEIGHT,WINDOW_WIDTH);
        temp_fractal.top_right.x_point = get_x(temp_x, (97+WINDOW_HEIGHT-1 - temp_y), fractal,WINDOW_HEIGHT,WINDOW_WIDTH);
        printf("bottom left y x: ");
        scanf("%c %d",&temp_y, &temp_x);
        while(getchar() != '\n');
        temp_fractal.bottom_left.y_point = get_y(temp_x, (97+WINDOW_HEIGHT-1 - temp_y), fractal,WINDOW_HEIGHT,WINDOW_WIDTH);
        temp_fractal.bottom_left.x_point = get_x(temp_x, (97+WINDOW_HEIGHT-1 - temp_y), fractal,WINDOW_HEIGHT,WINDOW_WIDTH);
        printf("bottom right y x: ");
        scanf("%c %d",&temp_y, &temp_x);
        while(getchar() != '\n');
        temp_fractal.bottom_right.y_point = get_y(temp_x, (97+WINDOW_HEIGHT-1 - temp_y), fractal,WINDOW_HEIGHT,WINDOW_WIDTH);
        temp_fractal.bottom_right.x_point = get_x(temp_x, (97+WINDOW_HEIGHT-1 - temp_y), fractal,WINDOW_HEIGHT,WINDOW_WIDTH);

        fractal = temp_fractal;
	}
}

unsigned int get_root(unsigned int index)
{
	unsigned int temp;

	temp = (unsigned int)sqrt((double)index);

	if((temp * temp) == index)
	{
		return temp;
	}
	else
	{
		return (unsigned int)(sqrt((double)(index))+1);
	}
}

unsigned int get_message( unsigned char message[LENGTH])
{
    int ii = 0;
    char ch;

    printf("type message\n");

    while(((ch = getchar()) != '\n') && ii < LENGTH)
    {
        message[ii] = ch;
        ii++;
    }
    return ii;
}

unsigned int get_cipher_text( unsigned char cipher[LENGTH])
{
	int ii = 0;
	char hex_byte[2];

    printf("\ntype cipher text\n");

     while(((hex_byte[ii%2] = getchar()) != '\n') && ii < LENGTH)
	 {
         if(ii%2)
         {
            sscanf(hex_byte, "%2X", &cipher[ii/2]);
         }
		 ii++;
	 }
     return ii/2;
}

void print_cipher_text( unsigned char cipher[LENGTH], unsigned int size)
{
    int ii;

    printf("\n############## Cipher Text ##############\n\n");

    for(ii = 0; ii < (int)size; ii++)
    {
        printf("%02X", (unsigned char)cipher[ii]);
    }
	printf("\n");
}

void print_message( unsigned char message[LENGTH], unsigned int size)
{
    int ii;

    printf("\n############## Plain Text ##############\n\n");

    for(ii = 0; ii < size; ii++)
    {
        printf("%c", (char)message[ii]);
    }
	printf("\n");
}

void crunch_text(FRACTAL_TYPE fractal, DIRECTION direction)
{
    unsigned int root, size;
    unsigned char data[LENGTH];

    if(direction == ENCRYPT)
    {
        size = get_message(data);
        root = get_root(size);

        crunch(data,fractal,root,direction);
        print_cipher_text(data,size);
    }
    else
    {
        size = get_cipher_text(data);
        root = get_root(size);

        crunch(data,fractal,root,direction);
        print_message(data, size);
    }
}

void crunch_file(FRACTAL_TYPE fractal, DIRECTION direction)
{
    FILE *file;
    char file_name[256];
    unsigned char *data;
    unsigned int root;
    long int size;

    printf("\nfile name: ");
    scanf("%s", &file_name);
    while(getchar() != '\n');
    file = fopen64( file_name,"rb+");

    if(file)
    {
        fseek(file,0, SEEK_END);
        size = ftell(file);
        fseek(file,0, SEEK_SET);

        root = get_root(size);
        data = (unsigned char*)malloc(root*root);

        fread(data,sizeof(char),size,file);
        fseek(file,0, SEEK_SET);

        crunch(data, fractal, root, direction);

        fwrite(data,sizeof(char),size,file);

        fclose(file);
        free(data);

        if(direction == ENCRYPT)
        {
            printf("\n############ File Encrypted ############\n\n");
        }
        else
        {
            printf("\n############ File Decrypted ############\n\n");
        }
    }
}

void save_fractal(FRACTAL_TYPE fractal, FRACTAL_TYPE master_fractal)
{
    FILE *file;
    char file_name[256];
    unsigned char data[LENGTH];
    unsigned int root = 8;
    int index = 0;
    unsigned int size;
    char temp_char;

    printf("\nfile name: ");
    scanf("%s", &file_name);
    while(getchar() != '\n');
    file = fopen(file_name, "rb");

    if(file)
    {
        fseek(file,0, SEEK_END);
        size = ftell(file);
        fseek(file,0, SEEK_SET);

        if(size != 0)
        {
            printf("Overwrite Key? y/n ");
            scanf("%c", &temp_char);
            while(getchar() != '\n');
            if(temp_char != 'y')
            {

                return;
            }
        }
        fclose(file);
        file = fopen(file_name, "wb");

        memcpy(&data[index],&fractal.top_left.x_point, sizeof(fractal.top_left.x_point));
        index += sizeof(fractal.top_left.x_point);
        memcpy(&data[index],&fractal.top_left.y_point, sizeof(fractal.top_left.y_point));
        index += sizeof(fractal.top_left.y_point);
        memcpy(&data[index],&fractal.top_right.x_point, sizeof(fractal.top_right.x_point));
        index += sizeof(fractal.top_right.x_point);
        memcpy(&data[index],&fractal.top_right.y_point, sizeof(fractal.top_right.y_point));
        index += sizeof(fractal.top_right.y_point);
        memcpy(&data[index],&fractal.bottom_left.x_point, sizeof(fractal.bottom_left.x_point));
        index += sizeof(fractal.bottom_left.x_point);
        memcpy(&data[index],&fractal.bottom_left.y_point, sizeof(fractal.bottom_left.y_point));
        index += sizeof(fractal.bottom_left.y_point);
        memcpy(&data[index],&fractal.bottom_right.x_point, sizeof(fractal.bottom_right.x_point));
        index += sizeof(fractal.bottom_right.x_point);
        memcpy(&data[index],&fractal.bottom_right.y_point, sizeof(fractal.bottom_right.y_point));

        crunch(data, master_fractal, root, ENCRYPT);

        fwrite(&data, sizeof(char),256,file);
        fclose(file);

        printf("Key Saved\n");
    }
}

FRACTAL_TYPE load_fractal(FRACTAL_TYPE master_fractal)
{
	FRACTAL_TYPE fractal;
	FILE *file;
	char file_name[256];
	unsigned char data[LENGTH];
    unsigned int root = 8;
    int index = 0;

	printf("\nfile name: ");
	scanf("%s", &file_name);
    while(getchar() != '\n');
	file = fopen(file_name, "rb");

	if(file)
	{
		fread(&data, sizeof(char),256,file);

        crunch(data, master_fractal, root, DECRYPT);

        memcpy(&fractal.top_left.x_point, &data[index],sizeof(fractal.top_left.x_point));
        index += sizeof(fractal.top_left.x_point);
        memcpy(&fractal.top_left.y_point, &data[index],sizeof(fractal.top_left.y_point));
        index += sizeof(fractal.top_left.y_point);
        memcpy(&fractal.top_right.x_point, &data[index],sizeof(fractal.top_right.x_point));
        index += sizeof(fractal.top_right.x_point);
        memcpy(&fractal.top_right.y_point, &data[index],sizeof(fractal.top_right.y_point));
        index += sizeof(fractal.top_right.y_point);
        memcpy(&fractal.bottom_left.x_point, &data[index],sizeof(fractal.bottom_left.x_point));
        index += sizeof(fractal.bottom_left.x_point);
        memcpy(&fractal.bottom_left.y_point, &data[index],sizeof(fractal.bottom_left.y_point));
        index += sizeof(fractal.bottom_left.y_point);
        memcpy(&fractal.bottom_right.x_point, &data[index],sizeof(fractal.bottom_right.x_point));
        index += sizeof(fractal.bottom_right.x_point);
        memcpy(&fractal.bottom_right.y_point, &data[index],sizeof(fractal.bottom_right.y_point));

		fclose(file);
        printf("Key Loaded\n");
	}
	return fractal;
}

void crack()
{
    double spaces = 1000000000;
    double spacing = 1.0/spaces;

    FRACTAL_TYPE fractal;
    int root = 13;
    unsigned char *data;

    double temp = 0;

    data = (unsigned char *)malloc(sizeof(char) * (root*root));

    for(fractal.top_left.y_point = 1.5; fractal.top_left.y_point >= -1.5; fractal.top_left.y_point -= spacing)
    {
        for(fractal.top_left.x_point = -2; fractal.top_left.x_point <= 1; fractal.top_left.x_point += spacing)
        {
            for(fractal.top_right.y_point = 1.5; fractal.top_right.y_point >= -1.5; fractal.top_right.y_point -= spacing)
            {
                for(fractal.top_right.x_point = 1; fractal.top_right.x_point >= -2; fractal.top_right.x_point -= spacing)
                {
                    for(fractal.bottom_left.y_point = -1.5; fractal.bottom_left.y_point <= 1.5; fractal.bottom_left.y_point += spacing)
                    {
                        for(fractal.bottom_left.x_point = -2; fractal.bottom_left.x_point <= 1; fractal.bottom_left.x_point += spacing)
                        {
                            for(fractal.bottom_right.y_point = -1.5; fractal.bottom_right.y_point < 1.5; fractal.bottom_right.y_point += spacing)
                            {
                                for(fractal.bottom_right.x_point = 1; fractal.bottom_right.x_point >= -1; fractal.bottom_right.x_point -= spacing)
                                {
                                    crunch(data, fractal, root, ENCRYPT);
                                    temp += 1;
                                    printf("percent done = %lf / %lf\n", temp, (spaces*spaces*spaces*spaces*spaces*spaces*spaces*spaces));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void print_menu()
{
	printf("\n");
	printf("1 Encode Text\n");
	printf("2 Decode Text\n");
	printf("3 Encode File\n");
	printf("4 Decode File\n");
	printf("5 Load Key\n");
	printf("6 Create Key\n");
    printf("7 Enter Manual Key\n");
	printf("\nEnter: ");
}

int main()
{
    FRACTAL_TYPE fractal;
	FRACTAL_TYPE master_fractal;
	int choice;

    master_fractal = search_fractal();

    while(1)
    {
		print_menu();

        scanf("%d", &choice);
        while(getchar() != '\n');

		switch(choice)
		{
			case 1:
			{
                crunch_text(fractal, ENCRYPT);
				break;
			}
			case 2:
			{
                crunch_text(fractal, DECRYPT);
				break;
			}
            case 3:
            {
                crunch_file(fractal, ENCRYPT);
                break;
            }
            case 4:
            {
                crunch_file(fractal, DECRYPT);
                break;
            }
			case 5:
			{
				fractal = load_fractal(master_fractal);
				break;
			}
			case 6:
			{
				fractal = search_fractal();
                save_fractal(fractal, master_fractal);
                break;
			}
            case 7:
            {
                fractal = set_fractal();
                print_fractal(fractal);
                break;
            }
			default:
			{
                printf("\nFutile Option\n");
                crack();
			}
		}
    }
}
