#include <iostream>

const double PI = 3.141592653589793;

int get_rect_perimeter(int w, int l) { return 2 * (w + l); }

double get_circ_circumference(int r) { return 2 * PI * r; }

int main() {
  int base_radius = 0;
  int expected_cone_height = 0;
  int base_width = 0;
  int base_length = 0;
  int expected_pyramid_height = 0;

  double cone_filament = 0.0;
  int pyramid_filament = 0;

  std::cin >> base_radius >> expected_cone_height >> base_width >>
      base_length >> expected_pyramid_height;

  // Cone
  int real_cone_height = 0;
  if (expected_cone_height <= base_radius) {
    real_cone_height = expected_cone_height;
  } else {
    real_cone_height = base_radius;
  }

  for (int i = 0, r = base_radius; i < real_cone_height; i = i + 1) {
    cone_filament += get_circ_circumference(r);
    r--;
  }

  // Pyramid
  int pyramid_height_max = 0;
  if (base_width <= base_length) {
    pyramid_height_max = base_width;
  } else {
    pyramid_height_max = base_length;
  }

  int real_pyramid_height = 0;
  if (expected_pyramid_height <= pyramid_height_max) {
    real_pyramid_height = expected_pyramid_height;
  } else {
    real_pyramid_height = pyramid_height_max;
  }

  for (int i = 0, w = base_width, l = base_length; i < real_pyramid_height;
       i = i + 1) {
    pyramid_filament += get_rect_perimeter(w, l);
    w--;
    l--;
  }

  // Output
  if (expected_cone_height <= base_radius) {
    std::cout << "Cone: partial print with entered height = "
              << real_cone_height << std::endl;
  } else {
    std::cout << "Cone: entered height is too large, will print max height = "
              << real_cone_height << std::endl;
  }
  std::cout << "Cone filament length = " << cone_filament << std::endl;

  if (expected_pyramid_height <= pyramid_height_max) {
    std::cout << "Pyramid: partial print with entered height = "
              << real_pyramid_height << std::endl;
  } else {
    std::cout
        << "Pyramid: entered height is too large, will print max height = "
        << real_pyramid_height << std::endl;
  }
  std::cout << "Pyramid filament length = " << pyramid_filament << std::endl;

  return 0;
}