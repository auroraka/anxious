#include <opencv2/opencv.hpp>

#include <cassert>

using cv::Mat;
using cv::Vec3b;
using cv::imshow;
using cv::Scalar;

const int kMemLen = 32 * (480 << 10);
char mem[kMemLen];

Mat* canvas;

inline void DrawPixel(int x, int y, const Vec3b &color) {
  canvas->at<Vec3b>(x, y) = color;
}

int main(int argc, char **argv) {
  assert(argc == 3);
  FILE* f = fopen(argv[1], "rb");
  for (int i = 0; i < kMemLen; ++i) {
    char c;
    int ret = fscanf(f, "%c", &c);
    assert(ret == 1);
    mem[i] = c;
  }
  unsigned *us = reinterpret_cast<unsigned*>(mem);
  canvas = new Mat(480, 640, CV_8UC3, Scalar(255, 255, 255));
  // MidPointCircle(300, 400, 250, Vec3b(0, 0, 0));
  for (int i = 0; i < 480; ++i) {
    for (int j = 0; j < 640; ++j) {
      unsigned data = us[(i << 10) + j];
      DrawPixel(i, j, Vec3b(data & 0xFF, (data >> 8) & 0xFF, (data >> 16) & 0xFF));
    }
  }

  // imshow("Circle", *canvas);
  // cvWaitKey();

  imwrite(argv[2], *canvas);
  return 0;
}

