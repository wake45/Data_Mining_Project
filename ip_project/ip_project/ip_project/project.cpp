#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <Windows.h>

int face_detect(int, char **);  //������ ���� & ī�޶� ����
void _face_detect_and_draw(IplImage *); //�ν� �� �׸� �׸��� ���� �Լ�
void Mosaic(IplImage* image, CvRect *face); //������ũ ó�� �Լ�

static CvMemStorage *storage = 0; //����ϱ� ���� �޸� ���� ����
static CvHaarClassifierCascade *cascade = 0; //Haar ��� cascade �з��� ���� ���� 

void gotoxy(int x, int y); //ȭ������ �Լ�

int main(int argc, char **argv) {
	int menu = 0;

	const char *Menu[] = { "1.�� �ν� : ", "2.���� : "};

	system("cls");
	gotoxy(40, 10); //ȭ����� ���� ��� ��ġ ���� �� �޴� ���
	printf("����������������������������������������������������������������������������\n");
	gotoxy(40, 11);
	printf("��           �ڵ� ������ũ            ��\n");
	gotoxy(40, 12);
	printf("����������������������������������������������������������������������������\n");
	gotoxy(40, 13);
	printf("��%-36s��\n", Menu[0]);
	gotoxy(40, 14);
	printf("����������������������������������������������������������������������������\n");
	gotoxy(40, 15);
	printf("��%-36s��\n", Menu[1]);
	gotoxy(40, 16);
	printf("����������������������������������������������������������������������������\n");
	gotoxy(40, 17);
	printf(" �޴� ���� : ");
	scanf("%d", &menu);


	switch (menu) { //�Է� �� �� ����
	case 1:
		face_detect(argc, argv);
		break;
	case 2:
		printf("���� \n");
		break;
	default:
		gotoxy(40, 18);
		printf("ERROR : Wrong Input\n"); //���ϴ� �� �̿��� �� �Է½� ���
		break;
	}
	gotoxy(40, 19);
}
int face_detect(int argc, char **argv) { //������ ���� & ī�޶� ����
	CvCapture *capture = 0; //������ ������ ���� ����ü
	IplImage *frame; //���������� ������ ����
	const char *cascade_name = "C:/data/haarcascades/haarcascade_frontalface_alt2.xml"; //xml������ cascade_name�� �Ҵ� //xml���� ����

	if (!(cascade = (CvHaarClassifierCascade *)cvLoad(cascade_name, 0, 0, 0))) { //xml������ �о� ��� HaarClassifierCascade�� �Ҵ�
		fprintf(stderr, "ERROR: Couldn't load classifier cascade\n"); //�о� ���� �� �� ��� ��� �� ����
		return -1;
	}

	storage = cvCreateMemStorage(0); //�޸� ���� ���� �Ҵ�
	capture = cvCaptureFromCAM(0); //ī�޶� ���� �ʱ�ȭ

	cvNamedWindow("M180518_�̻��_Result"); //������ ����ϱ� ���� �����츦 ����� �Լ�

	while (!0) {
		if (!(frame = cvQueryFrame(capture))) //������ �������� ��� �������ش�.
			break;
		_face_detect_and_draw(frame); //��ȯ�� �������� ���� �� �� ���� �� ���� ��� ǥ���Լ��� ������.
		if (cvWaitKey(10) >= 0) //Ű���� �Է��� ��ٸ� �� Ű���� ���� ��ȯ�Ѵ�.
			break;

	}
	cvReleaseCapture(&capture); //capture�� �޸��Ҵ� ����
	cvDestroyWindow("M180518_�̻��_Result"); //�����츦 ��ü�մϴ�.
	return 0;
}

void Mosaic(IplImage* img, CvRect *face) { 
	int cnt = 0;
	int temp = 9;
	int w_point = 0, h_point = 0;
	int y_point = 0, x_point = 0;
	double R = 0, G = 0, B = 0;

	//������ ���� ����� ���ؼ� �� ���� ä���ش�.
	//���� ����̱� ������ ������ �帴�ϰ� ���´�.
	for (int i = 0; i < face->height / temp; i++) {
		for (int j = 0; j < face->width / temp; j++) {
			cnt = 0;
			B = 0; G = 0; R = 0;

			x_point = face->x + (j * temp);
			y_point = face->y + (i * temp);

			//������ ��� �����ݴϴ�.
			for (int k = y_point; k < y_point + temp; k++) {
				for (int m = x_point; m < x_point + temp; m++) {
					CvScalar color;
					w_point = m;
					h_point = k;

					if (m >= img->width) {
						w_point = img->width - 1;
					}

					if (k >= img->height) {
						h_point = img->height - 1;
					}

					color = cvGet2D(img, h_point, w_point); //h,w�� �ִ� �ȼ��� ���� �����մϴ�.

					B += color.val[0];
					G += color.val[1];
					R += color.val[2];
					cnt++;
				}
			}

			//����� ����մϴ�.
			B /= cnt; 
			G /= cnt;
			R /= cnt;

			CvScalar color;

			color.val[0] = B;
			color.val[1] = G;
			color.val[2] = R;

			CvPoint front;
			CvPoint rear;
				
			front.x = x_point;
			front.y = y_point;
			rear.x = x_point + temp;
			rear.y = y_point + temp;

			cvRectangle(img, front, rear, color, CV_FILLED, 8, 0);
			//���� ����� �׷��� ������ũ �簢���� �׸��ϴ�.
		}
	}
}

void _face_detect_and_draw(IplImage *img) {
	static CvScalar colors[] = { { { 0,0,255 } } ,{ { 0,128,255 } },{ { 0,255,255 } },{ { 0,255,0 } },
	{ { 255,128,0 } },{ { 255,255,0 } } ,{ { 255,0,0 } },{ { 255,0,255 } } }; //ǥ�� �� ���� �����մϴ�.
	double scale = 2.0; //��� �ӵ� ������ �ϱ� ���� ���� 2�� ��� ó��

	IplImage *gray = cvCreateImage(cvSize(img->width, img->height), 8, 1); //�Ѱ� ���� img�� ������ �����մϴ�.
	IplImage *small_Img = cvCreateImage(cvSize(cvRound(img->width / scale), cvRound(img->height / scale)), 8, 1); //�Ѱ� ���� img�� ������ ���� �Ŀ� �����մϴ�.

	CvRect max; //���� ū ������ ������ �����ϱ� ���� ����
	CvRect face; //������ ������ �����ϱ� ���� ����

	cvCvtColor(img, gray, CV_BGR2GRAY); //�÷� ������ ��ϵ� �������� ��ȯ�մϴ�.
	cvResize(gray, small_Img, CV_INTER_LINEAR); //�缱�� ���������� ����� ���� �����͸� ����ϴ�. //�缱�� ����������
	cvClearMemStorage(storage); //����� �޸𸮸� �ʱ�ȭ�մϴ�.

	if (cascade) {
		double t = (double)cvGetTickCount(); //����ð��� ��� ���� �ð� ����غ�
		//���� ���� �� ���� �� seq���·� ��ȯ�Ѵ�.
		CvSeq *faces = cvHaarDetectObjects(small_Img, cascade, storage, 1.1, 2, 0, cvSize(30, 30), cvSize(300, 300)); //�ٽ� �Լ�����
		t = (double)cvGetTickCount() - t; //����ð� ���
		printf("Detection tiem : %gms\n", t / ((double)cvGetTickFrequency() * 1000)); //����ð� ���

		max.width = 0; 
		max.height = 0;

		//���� �� ���� �簢�� ���·� ���ʴ�� ǥ���մϴ�.
		for (int i = 0; i < (faces ? faces->total : 0); i++) { 
			CvRect *r = (CvRect *)cvGetSeqElem(faces, i); //Memstorage���� ����� ��ü�� cvRect������ �����մϴ�.
		    //���� �� �簢���� �밢���� ��ġ�� �� �𼭸��� ��ǥ�� ����ݴϴ�.
			CvPoint front;
			CvPoint rear;
			front.x = cvRound((r->x + r->width*0.5)*scale);
			front.y = cvRound((r->y + r->width*0.5)*scale);
			int radius = cvRound((r->width + r->height)*0.25*scale);
			rear.x = front.x + radius;
			rear.y = front.y + radius;
			front.x -= radius;
			front.y -= radius;
			cvRectangle(img, front, rear, colors[i & 8], 3, 8, 0); 
			//���� ������ �� ������ �ν��� ��� �󱼿� �簢�� �׵θ� �׸���

			//���� ū �׸� ã������ face�� �����մϴ�.
			face.x = front.x;
			face.y = front.y;
			face.width = rear.x - front.x;
			face.height = rear.y - front.y;

			//���� ū �簢���� ������ũ ���� �ʽ��ϴ�.
			if (max.width * max.height < face.width * face.height) 
			{   //���� ����ߴ� ���ν� �簢���� ũ�⺸�� ���� ����ϴ� ���ν� �簢���� ũ�Ⱑ Ŭ��� 
				//���� �簢������ ������ũ�ϰ� ���� �簢���� �����մϴ�. ũ�� ���� ��� �׳� ������ũ�մϴ�.
				Mosaic(img, &max);
				max.x = face.x;
				max.y = face.y;
				max.width = face.width;
				max.height = face.height;
			}
			else
				Mosaic(img, &face);
		}
	}

	cvShowImage("M180517_�̻��_Result", img); //����� ���� ���� �����ֱ�
	cvReleaseImage(&gray); //�̹����� �����մϴ�.
	cvReleaseImage(&small_Img); //�̹����� �����մϴ�.
}

void gotoxy(int x, int y) //ȭ���� ���� �ϱ� ���� �Լ�
{
	COORD pos = { x - 1, y - 1 };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}