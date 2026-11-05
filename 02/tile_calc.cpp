#include <cmath>
#include <iostream>

const double PI = 3.14;

double get_tile_area(double x, double y) { return x * y; }

double get_rect_surface(double h, double w, double l) {
  return 2 * (h * w + h * l + w * l);
}

double get_sphere_surface(double r) { return 4 * PI * r * r; }

int get_tiles_needed(double surface, double tile_area) {
  return std::ceil(surface / tile_area);
}

int main() {
  double tile_width = 0.0;
  double tile_length = 0.0;
  double object_height = 0.0;
  double object_width = 0.0;
  double object_length = 0.0;
  double sphere_radius = 0.0;

  std::cout << "Enter tile width: ";
  std::cin >> tile_width;
  std::cout << "Enter tile length: ";
  std::cin >> tile_length;
  std::cout << "Enter object height: ";
  std::cin >> object_height;
  std::cout << "Enter object width: ";
  std::cin >> object_width;
  std::cout << "Enter object length: ";
  std::cin >> object_length;
  std::cout << "Enter sphere radius: ";
  std::cin >> sphere_radius;

  double tile_area = get_tile_area(tile_width, tile_length);
  double rect_surface =
      get_rect_surface(object_height, object_width, object_length);
  double sphere_surface = get_sphere_surface(sphere_radius);
  int tiles_for_rect = get_tiles_needed(rect_surface, tile_area);
  int tiles_for_sphere = get_tiles_needed(sphere_surface, tile_area);

  std::cout << "Tile area: " << tile_area << std::endl;
  std::cout << "Rect surface: " << rect_surface << std::endl;
  std::cout << "Tiles for rect: " << tiles_for_rect << std::endl;
  std::cout << "Sphere surface: " << sphere_surface << std::endl;
  std::cout << "Tiles for sphere: " << tiles_for_sphere << std::endl;

  return 0;
}