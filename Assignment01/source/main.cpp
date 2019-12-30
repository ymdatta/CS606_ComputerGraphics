#include <bits/stdc++.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

using namespace std;

#define cout(a) cout<<a<<endl

#define PI 3.14159265

#define clamp(x, lower, upper) (min(upper, max(x, lower)))

// IDs
float scale = 1.0, x = 0.0, y = 0.0;
int bufferWidth, bufferHeight;
const int numPoints = 50000;

// These variables to keep track of dragging.
float dgd_x = 0.0, dgd_y = 0.0;
int dgd_i = -1;

// This varialbe to keep track of 'key' rotation.
float rta = -45.0;

// These variables to keep track of 'calculator' rotation.
float c_rta = 0.0;
bool c_rotate = false;

// Keep track of Animation.
bool isAnimated = false;
int indAnimated = 0;

// Vectors
std::vector< char > calc_ids;
std::vector< std::pair<float, float> > down_pos;
std::vector< std::pair<float, float> > up_pos;

void printChar(float xpos, float ypos) {
  int length = down_pos.size();
  for(int i = 0; i < length; i++) {
    float l_x = down_pos[i].first;
    float l_y = down_pos[i].second;

    float r_x = up_pos[i].first;
    float r_y = up_pos[i].second;

    bool x_check = ((xpos >= l_x) && (xpos <= r_x));
    bool y_check = ((ypos >= l_y) && (ypos <= r_y));

    if (x_check && y_check) {
      if (calc_ids[i] == '@') {
	cout("AC");
      }
      else if (calc_ids[i] == ')') {
	cout("+/-");
      }
      else {
	cout(calc_ids[i]);
      }
      return;
    }
  }
}

int  getIndex(float xpos, float ypos) {
  int length = down_pos.size();
  for(int i = 0; i < length; i++) {
    float l_x = down_pos[i].first;
    float l_y = down_pos[i].second;

    float r_x = up_pos[i].first;
    float r_y = up_pos[i].second;

    bool x_check = ((xpos >= l_x) && (xpos <= r_x));
    bool y_check = ((ypos >= l_y) && (ypos <= r_y));

    if (x_check && y_check) {
      return i;
    }
  }
  return -1;
}

void drawBox(int index) {

  float l_x = down_pos[index].first + 0.01;
  float l_y = down_pos[index].second + 0.01;

  float r_x = up_pos[index].first - 0.01;
  float r_y = up_pos[index].second - 0.01;

  float c_x = (r_x - l_x) / 2;
  c_x += l_x;
  float c_y = (r_y - l_y) / 2;
  c_y += l_y;

  glTranslatef(c_x, c_y, 0);
  glRotatef(45.0, 0, 0, 1);

  glBegin(GL_QUADS);

  glColor4f(255.0f, 0.0f, 0.0f, 1.0f);
  glVertex3f(l_x, l_y, 0.0f);
  glVertex3f(r_x, l_y, 0.0f);
  glVertex3f(r_x, r_y, 0.0f);
  glVertex3f(l_x, r_y, 0.0f);
  glColor4f(255.0f, 255.0f, 255.0f, 1.0f);

  glEnd();

  glTranslatef(x, y, 0);
}

// This is the main-logic of code.
void drawCalculator()
{
  glLoadIdentity();//load identity matrix

  glTranslatef(x, y, 0);//move forward 4 units
  glScalef(scale, scale, scale);

  glRotatef(c_rta, 0.0, 0.0, 1.0);
  if (c_rotate) 
    {
      c_rta = (int(c_rta) + 5) % 360;
    }
  

  float x_len = 0.0f, y_len = 0.126f;
  if (dgd_i == 16) {
    x_len = 0.258;
  }
  else {
    x_len = 0.124;
  }

  // Clear the vectors, as we are pushing the
  //  elements into the vector every time.
  down_pos.clear();
  up_pos.clear();

  glBegin(GL_QUADS);

  glColor4f(255.0f, 255.0f, 255.0f, 1.0f);
  // a
  if (dgd_i != 0) {
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.124f, 0.0f, 0.0f);
    glVertex3f(0.124f, 0.126f, 0.0f);
    glVertex3f(0.0f, 0.126f, 0.0f);
 
    down_pos.push_back(std::make_pair(0.0f, 0.0f));
    up_pos.push_back(std::make_pair(0.124f, 0.126f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('6');    

  // b
  glColor4f(255.0f, 165.0f, 0.0f, 1.0f);
  if (dgd_i != 1) {
    glVertex3f(0.134f, 0.0f, 0.0f);
    glVertex3f(0.258f, 0.0f, 0.0f);
    glVertex3f(0.258f, 0.126f, 0.0f);
    glVertex3f(0.134f, 0.126f, 0.0f);

    down_pos.push_back(std::make_pair(0.134f, 0.0f));
    up_pos.push_back(std::make_pair(0.258f, 0.126f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('-');

  glColor4f(255.0f, 255.0f, 255.0f, 1.0f);    

  // c
  if (dgd_i != 2) {
    glVertex3f(0.0f, 0.137f, 0.0f);
    glVertex3f(0.124f, 0.137f, 0.0f);
    glVertex3f(0.124f, 0.263f, 0.0f);
    glVertex3f(0.0f, 0.263f, 0.0f);

    down_pos.push_back(std::make_pair(0.0f, 0.137f));
    up_pos.push_back(std::make_pair(0.124f, 0.263f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('9');    

  // d
  if (dgd_i != 3) {
    glVertex3f(-0.01f, 0.0f, 0.0f);
    glVertex3f(-0.134f, 0.0f, 0.0f);
    glVertex3f(-0.134f, 0.126f, 0.0f);
    glVertex3f(-0.01f, 0.126f, 0.0f);

    down_pos.push_back(std::make_pair(-0.134f, 0.0f));
    up_pos.push_back(std::make_pair(-0.01f, 0.126f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  
  calc_ids.push_back('5');    

  // e
  if (dgd_i != 4) {
    glVertex3f(0.0f, -0.01f, 0.0f);
    glVertex3f(0.124f, -0.01f, 0.0f);
    glVertex3f(0.124f, -0.136f, 0.0f);
    glVertex3f(0.0f, -0.136f, 0.0f);

    down_pos.push_back(std::make_pair(0.0f, -0.136f));
    up_pos.push_back(std::make_pair(0.124f, -0.01f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('3');    

  // f
  glColor4f(255.0f, 165.0f, 0.0f, 1.0f);
  if (dgd_i != 5) {
    glVertex3f(0.134f, -0.01f, 0.0f);
    glVertex3f(0.258f, -0.01f, 0.0f);
    glVertex3f(0.258f, -0.136f, 0.0f);
    glVertex3f(0.134f, -0.136f, 0.0f);

    down_pos.push_back(std::make_pair(0.134f, -0.136f));
    up_pos.push_back(std::make_pair(0.258f, -0.01f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('+');

  // g
  if (dgd_i != 6) {
    glVertex3f(0.134f, 0.137f, 0.0f);
    glVertex3f(0.258f, 0.137f, 0.0f);
    glVertex3f(0.258f, 0.263f, 0.0f);
    glVertex3f(0.134f, 0.263f, 0.0f);

    down_pos.push_back(std::make_pair(0.134f, 0.137f));
    up_pos.push_back(std::make_pair(0.258f, 0.263f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('x');    

  // h
  if (dgd_i != 7) {
    glVertex3f(0.134f, 0.274f, 0.0f);
    glVertex3f(0.258f, 0.274f, 0.0f);
    glVertex3f(0.258f, 0.4f, 0.0f);
    glVertex3f(0.134f, 0.4f, 0.0f);

    down_pos.push_back(std::make_pair(0.134f, 0.274f));
    up_pos.push_back(std::make_pair(0.258f, 0.4f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('/');

  glColor4f(255.0f, 255.0f, 255.0f, 1.0f);
  // i
  if (dgd_i != 8) {
    glVertex3f(0.0f, 0.274f, 0.0f);
    glVertex3f(0.124f, 0.274f, 0.0f);
    glVertex3f(0.124f, 0.4f, 0.0f);
    glVertex3f(0.0f, 0.4f, 0.0f);

    down_pos.push_back(std::make_pair(0.0f, 0.274f));
    up_pos.push_back(std::make_pair(0.124f, 0.4f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('%');    

  // j
  if (dgd_i != 9) {
    glVertex3f(-0.01f, 0.137f, 0.0f);
    glVertex3f(-0.134f, 0.137f, 0.0f);
    glVertex3f(-0.134f, 0.263f, 0.0f);
    glVertex3f(-0.01f, 0.263f, 0.0f);

    down_pos.push_back(std::make_pair(-0.134f, 0.137f));
    up_pos.push_back(std::make_pair(-0.01f, 0.263f)); }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('8');    

  // k
  if (dgd_i != 10) {
    glVertex3f(-0.01f, 0.274f, 0.0f);
    glVertex3f(-0.134f, 0.274f, 0.0f);
    glVertex3f(-0.134f, 0.4f, 0.0f);
    glVertex3f(-0.01f, 0.4f, 0.0f);

    down_pos.push_back(std::make_pair(-0.134f, 0.274f));
    up_pos.push_back(std::make_pair(-0.01f, 0.4f)); }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back(')');    // ) --> +/-

  // l
  if (dgd_i != 11) {
    glVertex3f(-0.01f, -0.01f, 0.0f);
    glVertex3f(-0.134f, -0.01f, 0.0f);
    glVertex3f(-0.134f, -0.136f, 0.0f);
    glVertex3f(-0.01f, -0.136f, 0.0f);

    down_pos.push_back(std::make_pair(-0.134f, -0.136f));
    up_pos.push_back(std::make_pair(-0.01f, -0.01f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('2');    

  // m
  if (dgd_i != 12) {
    glVertex3f(-0.268f, -0.136f, 0.0f);
    glVertex3f(-0.144f, -0.136f, 0.0f);
    glVertex3f(-0.144f, -0.01f, 0.0f);
    glVertex3f(-0.268f, -0.01f, 0.0f);

    down_pos.push_back(std::make_pair(-0.268f, -0.136f));
    up_pos.push_back(std::make_pair(-0.144f, -0.01f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('1');    

  // n
  if (dgd_i != 13) {
    glVertex3f(-0.268f, -0.001f, 0.0f);
    glVertex3f(-0.144f, -0.001f, 0.0f);
    glVertex3f(-0.144f, 0.127f, 0.0f);
    glVertex3f(-0.268f, 0.127f, 0.0f);

    down_pos.push_back(std::make_pair(-0.268f, -0.001f));
    up_pos.push_back(std::make_pair(-0.144f, 0.127f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('4');    

  // o
  if (dgd_i != 14) {
    glVertex3f(-0.268f, 0.137f, 0.0f);
    glVertex3f(-0.144f, 0.137f, 0.0f);
    glVertex3f(-0.144f, 0.263f, 0.0f);
    glVertex3f(-0.268f, 0.263f, 0.0f);

    down_pos.push_back(std::make_pair(-0.268f, 0.137f));
    up_pos.push_back(std::make_pair(-0.144f, 0.263f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('7');    

  // p
  if (dgd_i != 15) {
    glVertex3f(-0.268f, 0.274f, 0.0f);
    glVertex3f(-0.144f, 0.274f, 0.0f);
    glVertex3f(-0.144f, 0.4f, 0.0f);
    glVertex3f(-0.268f, 0.4f, 0.0f);

    down_pos.push_back(std::make_pair(-0.268f, 0.274f));
    up_pos.push_back(std::make_pair(-0.144f, 0.4f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('@');    // @ --> AC

  // x
  if (dgd_i != 16) {
    glVertex3f(-0.268f, -0.272f, 0.0f);
    glVertex3f(-0.01f, -0.272f, 0.0f);
    glVertex3f(-0.01f, -0.146f, 0.0f);
    glVertex3f(-0.268f, -0.146f, 0.0f);

    down_pos.push_back(std::make_pair(-0.268f, -0.272f));
    up_pos.push_back(std::make_pair(-0.01f, -0.146f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('0');    

  // y
  if (dgd_i != 17) {
    glVertex3f(0.0f, -0.272f, 0.0f);
    glVertex3f(0.124f, -0.272f, 0.0f);
    glVertex3f(0.124f, -0.146f, 0.0f);
    glVertex3f(0.0f, -0.146f, 0.0f);



    down_pos.push_back(std::make_pair(0.0f, -0.272f));
    up_pos.push_back(std::make_pair(0.124f, -0.146f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('.');    

  glColor4f(255.0f, 165.0f, 0.0f, 1.0f);    

  // z
  if (dgd_i != 18) {
    glVertex3f(0.134f, -0.272f, 0.0f);
    glVertex3f(0.258f, -0.272f, 0.0f);
    glVertex3f(0.258f, -0.146f, 0.0f);
    glVertex3f(0.134f, -0.146f, 0.0f);  

    down_pos.push_back(std::make_pair(0.134f, -0.272f));
    up_pos.push_back(std::make_pair(0.258f, -0.146f));
  }
  else {
    glVertex3f(dgd_x - (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y - (y_len / 2), 0.0f);
    glVertex3f(dgd_x + (x_len / 2), dgd_y + (y_len / 2), 0.0f);
    glVertex3f(dgd_x - (x_len / 2), dgd_y + (y_len / 2), 0.0f);

    down_pos.push_back(std::make_pair(dgd_x - (x_len / 2), dgd_y - (y_len / 2)));
    up_pos.push_back(std::make_pair(dgd_x + (x_len / 2), dgd_y + (y_len / 2)));
  }
  calc_ids.push_back('=');
  glColor4f(255.0f, 255.0f, 255.0f, 1.0f);
  glEnd();


  // Here we draw the overlay rectangle.
  if (isAnimated) {

    float l_x = down_pos[indAnimated].first + 0.01;
    float l_y = down_pos[indAnimated].second + 0.01;

    float r_x = up_pos[indAnimated].first - 0.01;
    float r_y = up_pos[indAnimated].second - 0.01;

    float c_x = (l_x + r_x) / 2;
    float c_y = (l_y + r_y) / 2;

    // Translate before rotate.
    glTranslatef(c_x, c_y, 0);
    
    l_x -= c_x;
    l_y -= c_y;

    r_x -= c_x;
    r_y -= c_y;
    
    if (rta > 44) {
      glRotatef(0, 0.0, 0.0, 1.0);
      isAnimated = false;
    }
    else {
      rta += 1;
      glRotatef(rta, 0.0, 0.0, 1.0);     
    }
	
    // glPolygonMode - Decides how to fill the polygon
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);

    glColor4f(255.0f, 0.0f, 0.0f, 1.0f);

    glVertex3f(l_x, l_y, 0.0f);
    glVertex3f(r_x, l_y, 0.0f);    

    glVertex3f(r_x, r_y, 0.0f);
    glVertex3f(l_x, r_y, 0.0f);

    glColor4f(255.0f, 255.0f, 255.0f, 1.0f);
    // Translate back.
    glTranslatef(x, y, 0);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);     
  }

}

// When right button of mouse is clicked,
// then this function checks whether the pixel
// pointed by mouse is inside the calculator (or)
// whether if it belongs to a part of calculator.
bool isInside(double xpos, double ypos) {
  int length = down_pos.size();
  for(int i = 0; i < length; i++) {
    float l_x = down_pos[i].first;
    float l_y = down_pos[i].second;

    float r_x = up_pos[i].first;
    float r_y = up_pos[i].second;

    bool x_check = ((xpos >= l_x) && (xpos <= r_x));
    bool y_check = ((ypos >= l_y) && (ypos <= r_y));

    if (x_check && y_check) {
      return true;
    }
  }
  return false;
}

// This function handles mouse keys.
void mouseKeys(GLFWwindow* window, int button, int action, int mode)
{
  double xpos_t = 0, ypos_t = 0, xpos = 0, ypos = 0, zpos = 0;
  glfwGetCursorPos(window, &xpos_t, &ypos_t);

  double modelview[16], projection[16];
  int viewport[4];

  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetIntegerv(GL_VIEWPORT, viewport);

  gluUnProject(xpos_t, viewport[3] - ypos_t, 0, modelview, projection,
	       viewport, &xpos, &ypos, &zpos);

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    if (isInside(xpos, ypos)) {
      printChar(xpos, ypos);
      dgd_i = getIndex(xpos, ypos);
      dgd_x = xpos;
      dgd_y = ypos;
    }
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    dgd_x = xpos;
    dgd_y = ypos;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (isInside(xpos, ypos)) {
      isAnimated = true;
      indAnimated = getIndex(xpos, ypos);
      rta = -45;
    }
  }

}



void handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(window, GL_TRUE);
    }

  if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
    {
      scale += 0.05;
    }

  if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
    {
      scale -= 0.05;
    }

  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
      x -= 0.05;
    }

  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
      x += 0.05;
    }

  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
      y += 0.05;
    }

  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
      y -= 0.05;
    }
  if (key == GLFW_KEY_R && action == GLFW_PRESS) 
    {
      c_rotate = true;
    }
  if (key == GLFW_KEY_R && action == GLFW_RELEASE) 
    {
      c_rotate = false;
    }
}


int main(void) 
{
  const GLint WIDTH = 800, HEIGHT = 600;

  // Initializing GLFW
  if(!glfwInit())
    {
      cout("GLFW initialization failed.");
      glfwTerminate();
      return 1;
    }

  // Setup GLFW window properties
  // OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  // Check what are the last parameters.
  GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
  if(!mainWindow)
    {
      cout("GLFW window creation failed.");
      glfwTerminate();
      return 1;
    }

  // Get buffer size information

  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set context for GLEW to use
  glfwMakeContextCurrent(mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if(glewInit() != GLEW_OK)
    {
      cout("GLEW initialization failed.");
      glfwDestroyWindow(mainWindow);
      glfwTerminate();
      return 1;
    }

  // Setup viewport size

  // 0, 0 is where you want window to be anchored. Its actually top-left
  // co-ordinates.
  glViewport(0, 0, bufferWidth, bufferHeight);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // L, R, B, T, N, F
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Loop until window is closed
  while(!glfwWindowShouldClose(mainWindow))
    {
      // Get and handle user input
      glfwPollEvents();
      glfwSetKeyCallback(mainWindow, handleKeys);
      glfwSetMouseButtonCallback(mainWindow, mouseKeys);

      // Clear window
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      // Clear colour buffer before next frame
      glClear(GL_COLOR_BUFFER_BIT);

      drawCalculator();

      glfwSwapBuffers(mainWindow);
    }

  return 0;
}

