#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <functional>
#include <sstream>
#include <iostream>
using namespace cv;
using namespace std;

#ifdef _WIN32
    #include <windows.h>
    void Sleep(unsigned milliseconds);
#else
    #include <unistd.h>
    void Sleep(unsigned milliseconds);
#endif

const int Screen_W=800,Screen_H=600;

//1.5v
float _d1x[] ={0.5,2.65,4.2,5.2,6.7,6.27,4.55,2.95,1.08,0.25};
float _d1y[] ={3.65,4.22,5.15,6.13,8.7,11.0,12.55,13.42,14.3,14.55};
//2.5v
float _d2x[] = {6.29,6.6,7.25,7.95,8.24,7.9,7.1,6.38,5.37,5.0};
float _d2y[] = {0.4,2.65,4.72,6.99,8.79,10.92,12.75,14.12,15.89,17.6};
//3.5v
float _d3x[] = {9.8,9.74,9.8,10.02,10.25,10.08,9.72,9.22,8.95,8.8};
float _d3y[] = {0.1,1.95,4.0,6.3,7.95,10.17,11.85,13.7,15.49,17.8};
//4.5v
float _d4x[] = {12.75,12.83,12.75,12.7,12.6,12.55,12.36,12.2,12.0};
float _d4y[] = {3.4,2.05,4.1,5.4,8.45,10.8,12.9,14.9,16.9};
//5.5v
float _d5x[] = {15.4,15.15,14.9,14.9,14.78,14.9,14.9,15.05,15.12};
float _d5y[] = {0.75,2.82,5.15,7.5,9.85,12.38,14.53,16.55,17.95};
//6.5v
float _d6x[] = {18.82,18.5,17.78,17.35,16.92,17.35,18.15,18.81,19.0};
float _d6y[] = {0.05,1.9,4.36,6.58,9.1,11.35,13.7,15.85,17.8};
//7.5v
float _d7x[] = {25.39,23.85,22.15,21.6,20.2,18.7,18.95,21.2,22.7,24.0,24.75,25.38};
float _d7y[] = {3.15,3.95,4.5,4.75,5.9,8.05,10.6,12.95,13.64,14.32,14.6,14.88};

//https://www.safaribooksonline.com/library/view/c-cookbook/0596007612/ch04s07.html
void split(const string& s, char c,vector<string>& v) 
{
   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}
//https://www.safaribooksonline.com/library/view/c-cookbook/0596007612/ch04s07.html

Scalar ccolor(float x)
{
		if(x<=0.25)
		{
			int s = x/0.25*255;
			return Scalar(255, 0+s, 0);
		}
		else if(x<=0.5)
		{
			int s = (x-0.25)/0.25*255;
			return Scalar(255-s, 255, 0);
		}
		else if(x<=0.75)
		{
			int s = (x-0.5)/0.25*255;
			return Scalar(0, 255, s);
		}
		else
		{
			int s = (x-0.75)/0.25*255;
			return Scalar(0, 255-s, 255);
		}
}

float distant(float x1,float y1,float x2,float y2)
{
	float d=0;
	d =pow(x1-x2,2) + pow(y1-y2,2);
	return sqrt(d);
}
void draw(Mat _img,float *_x,float *_y ,float *_value,int size)
{
	for(int i=1;i<=514;i++)
	{
		for(int j=1;j<=362;j++)
		{
			int P=4;
			float Wi=0,WiUi=0;
			bool d0=false;
			float _v=0;
			for(int k=0;k<size;k++)
			{
				if(distant(i,j,_x[k]*20,_y[k]*20)==0)
				{
					_v=_value[k];
					d0=true;
					break;
				}
				WiUi += 1/pow(distant(i,j,_x[k]*20,_y[k]*20),P) *_value[k];
				Wi+=1/pow(distant(i,j,_x[k]*20,_y[k]*20),P);
			}
			if(!d0)
			{
				_v=WiUi/Wi;
			}
			line(_img,Point(i, j), Point(i, j),ccolor(_v/9.0), 1,LINE_8,0 );
		}
	}
}

int main(void)
{
	namedWindow( "From", CV_GUI_NORMAL );
	resizeWindow("From",Screen_W,Screen_H);

	Mat img = Mat::zeros(Screen_H, Screen_W, CV_8UC3);    
	img= Scalar(255,255,255);					//Scalar(blueVal,greenVal,redVal);
	for(int i=1;i<=100;i++)
	{
		line(img,Point(560, 362-i), Point(600, 362-i),ccolor(i/100.0), 1,LINE_8,0 );
	}
	putText(img , "_9" , Point(600,262), FONT_HERSHEY_DUPLEX, 0.6, Scalar(5, 0, 0),1,8,false );
	putText(img , "_4.5" , Point(600,312), FONT_HERSHEY_DUPLEX, 0.6, Scalar(5, 0, 0),1,8,false );
	putText(img , "_0 (volt)" , Point(600,362), FONT_HERSHEY_DUPLEX, 0.6, Scalar(5, 0, 0),1,8,false );

	int l=0;
	string line;
	ifstream myfile ("data.txt");
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
			l++;
		myfile.close();
	}
	float _dx[l];
	float _dy[l];
	float _value[l];
	int i=0;
	ifstream myfile2 ("data.txt");
	if (myfile2.is_open())
	{
		while ( getline (myfile2,line) )
		{
			vector<string> v;
			split(line, ' ', v);
			_value[i] = ::atof(v[0].c_str());
			_dx[i] = ::atof(v[1].c_str());
			_dy[i] = ::atof(v[2].c_str());
			i++;
		}
		myfile2.close();
	}
	draw(img,_dx,_dy ,_value,l);

	while(1)
	{
		imshow( "From", img );      
		int tt=waitKey(30);
		if(tt==27) break;
	}
	return 0;
}