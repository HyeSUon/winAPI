#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

typedef void (*funcPtr[6])(const int);


void shapeX(const int);
void invertedTriangle(const int);
void rhombus(const int);
void butterfly(const int);
void hourglass(const int);
void square(const int);




int main()
{
	funcPtr fp = { shapeX, invertedTriangle, rhombus, butterfly, hourglass, square };
	while (1) {
		int shape, size;
		if (1 != scanf("%d %d",&shape, &size ))
			return;
		if (shape > 6 || shape < 1 || size < 0 || size > 40) {
			printf("값의 범위가 올바르지 않습니다. (shape:1~6 size:1~40)\n");
			continue;
		}
		fp[shape-1](size);
	}

}

void shapeX(const int w)
{
	if (w > 10) {
		int h = w / 2;
		bool center = false;
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				if ((i * 2 + center) == j || 2 * i + j + center == w - 1)
					printf("%c", '*');
				else
					printf("%c", ' ');
				if (i == (h + 1) / 2)
					center = true;
			}
			printf("\n\n");
		}
	}
	else {
		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < w; ++j) {
				if (i == j || i + j == w - 1)
					printf("%c", '*');
				else
					printf("%c", ' ');
			}
			printf("\n");
		}
	}
}

void invertedTriangle(const int w)
{
	if (w > 10) {
		int h = w / 2;
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					printf("%c", ' ');
				else if (j < w - i)
					printf("%c", '*');
			}
			printf("\n\n");
		}
	}
	else {
		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					printf("%c", ' ');
				else if (j < w - i)
					printf("%c", '*');
			}
			printf("\n");
		}
	}
}
void rhombus(const int w)
{
	if (w > 10) {
		int h = w / 2;

		for (int i = (h - 1) / 2; i > 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i)
					printf("%c", ' ');
				else if (j < w - 2 * i)
					printf("%c", '*');
			}
			printf("\n\n");
		}



		for (int i = 0; i < (h + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i)
					printf("%c", ' ');
				else if (j < w - 2 * i)
					printf("%c", '*');
			}
			printf("\n\n");
		}
	}
	else {
		for (int i = (w - 1); i > 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					printf("%c", ' ');
				else if (j < w - i)
					printf("%c", '*');
			}
			printf("\n");
		}
		for (int i = 0; i < (w + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					printf("%c", ' ');
				else if (j < w - 1 * i)
					printf("%c", '*');
			}
			printf("\n");
		}
	}
}

void butterfly(const int w)
{
	if (w > 10) {
		int h = w / 2;

		for (int i = 0; i < (h + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i + 1)
					printf("%c", '*');
				else if (j < w - 2 * i - 1)
					printf("%c", ' ');
			}
			for (int j = 0; j < w; ++j)
				if (j < 2 * i + 1)
					printf("%c", '*');
			printf("\n\n");
		}
		for (int i = (h - 3) / 2; i >= 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i + 1)
					printf("%c", '*');
				else if (j < w - 2 * i - 1)
					printf("%c", ' ');
			}
			for (int j = 0; j < w; ++j)
				if (j < 2 * i + 1)
					printf("%c", '*');
			printf("\n\n");
		}
	}
	else {
		for (int i = 0; i < (w + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < i + 1)
					printf("%c", '*');
				else if (j < w - i - 1)
					printf("%c", ' ');
			}
			for (int j = 0; j < w; ++j)
				if (j < i + 1)
					printf("%c", '*');
			printf("\n");
		}
		for (int i = (w - 3) / 2; i >= 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j <  i + 1)
					printf("%c", '*');
				else if (j < w - i - 1)
					printf("%c", ' ');
			}
			for (int j = 0; j < w; ++j)
				if (j < i + 1)
					printf("%c", '*');
			printf("\n");
		}
	}
}
void hourglass(const int w)
{
	if (w > 10) {
		int h = w / 2;

		for (int i = 0; i < (h + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i)
					printf("%c", ' ');
				else if (j < w - 2 * i)
					printf("%c", '*');
			}
			printf("\n\n");
		}
		for (int i = (h - 3) / 2; i >= 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j < 2 * i)
					printf("%c", ' ');
				else if (j < w - 2 * i)
					printf("%c", '*');
			}
			printf("\n\n");
		}
	}
	else {
		for (int i = 0; i < (w + 1) / 2; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					printf("%c", ' ');
				else if (j < w - i)
					printf("%c", '*');
			}
			printf("\n");
		}
		for (int i = (w - 3) / 2; i >= 0; --i) {
			for (int j = 0; j < w; ++j) {
				if (j < i)
					printf("%c", ' ');
				else if (j < w - i)
					printf("%c", '*');
			}
			printf("\n");
		}
	}
}

void square(const int w)
{
	int h = w / 2;
	for (int i = 0; i < w; ++i)
		printf("%c", '*');
	printf("\n");

	for (int i = 0; i < h - 2; ++i) {
		for (int j = 0; j < w; ++j) {
			if (j == 0 || j == w - 1)
				printf("%c", '*');
			else
				printf("%c", ' ');
		}
		printf("\n");
	}



	for (int i = 0; i < w; ++i)
		printf("%c", '*');
	printf("\n");

}