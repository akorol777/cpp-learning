#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

const int max_coordinate = 999;
const int min_coordinate = 0;

enum class Direction { North, South, East, West };
enum class Action { move_fwd, move_bwd, turn_left, turn_right, unknown };

struct Car {
  int x = 0;
  int y = 0;
  Direction direction = Direction::North;
};

struct Car_step {
  int step = 0;
  Action action;
  int x = 0;
  int y = 0;
  std::string result;
};

bool check_coordinate_is_valid(int coordinate) {
  return coordinate >= min_coordinate && coordinate <= max_coordinate;
}

bool check_if_target_achieved(Car car, int target_x, int target_y) {
  int car_x = car.x;
  int car_y = car.y;
  bool is_x_near = target_x - 1 == car_x || target_x + 1 == car_x;
  bool is_y_near = target_y - 1 == car_y || target_y + 1 == car_y;
  bool is_x_equal = target_x == car_x;
  bool is_y_equal = target_y == car_y;
  return 
    (is_x_near && is_y_near) || 
    (is_x_near && is_y_equal) || 
    (is_y_near && is_x_equal) || 
    (is_x_equal && is_y_equal);
}

Action parse_action(const std::string action) {
  if (action == "move forward" || action == "move fwd" || action == "f") {
    return Action::move_fwd;
  }
  if (action == "move backward" || action == "move bwd" || action == "b") {
    return Action::move_bwd;
  }
  if (action == "turn left" || action == "l") {
    return Action::turn_left;
  }
  if (action == "turn right" || action == "r") {
    return Action::turn_right;
  }
  return Action::unknown;
}

Car move(Car car, Action action) {
  if (action == Action::move_fwd) {
    switch (car.direction) {
      case Direction::North: 
        if (check_coordinate_is_valid(car.y + 1)) {
          car.y++; 
        }
        break;
      case Direction::West:
        if (check_coordinate_is_valid(car.x - 1)) {
          car.x--; 
        }
        break;
      case Direction::South:
        if (check_coordinate_is_valid(car.y - 1)) {
          car.y--;
        }
        break;
      case Direction::East:  
        if (check_coordinate_is_valid(car.x + 1)) {
          car.x++;
        }
        break;
    }
  } else if (action == Action::move_bwd) {
    switch (car.direction) {
      case Direction::North:
        if (check_coordinate_is_valid(car.y - 1)) {
          car.y--; 
        }
        break;
      case Direction::West:  
        if (check_coordinate_is_valid(car.x + 1)) {
          car.x++; 
        }
        break;
      case Direction::South: 
        if (check_coordinate_is_valid(car.y + 1)) {
          car.y++; 
        }
        break;
      case Direction::East:
        if (check_coordinate_is_valid(car.x - 1)) {
          car.x--;
        }
        break;
    }
  }
  return car;
}

Car turn(Car car, Action action) {
  if (action == Action::turn_left) {
    switch (car.direction) {
      case Direction::North: car.direction = Direction::West; break;
      case Direction::West:  car.direction = Direction::South; break;
      case Direction::South: car.direction = Direction::East; break;
      case Direction::East:  car.direction = Direction::North; break;
    }
  } else {
    switch (car.direction) {
      case Direction::North: car.direction = Direction::East; break;
      case Direction::West:  car.direction = Direction::North; break;
      case Direction::South: car.direction = Direction::West; break;
      case Direction::East:  car.direction = Direction::South; break;
    }
  }
  return car;
}

Car drive(Car car, Action action) {
  if (action == Action::move_fwd || action == Action::move_bwd) {
    return move(car, action);
  }
  return turn(car, action);
}

std::string action_label(Action a) {
  switch (a) {
    case Action::move_fwd:
      return "move fwd";
    case Action::move_bwd:
      return "move bwd";
    case Action::turn_left:
      return "turn left";
    case Action::turn_right:
      return "turn right";
    case Action::unknown:
      return "unknown";
  }
  return "unknown";
}

std::string make_target_result(int target_x, int target_y) {
  return "target hit (" + std::to_string(target_x) + "," +
         std::to_string(target_y) + ")";
}

std::string format_position(int x, int y) {
  return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}

void print_journey_table(std::vector<Car_step> steps) {
  std::cout << "Step | Action     | Position (x,y) | Result\n";
  std::cout << "─────┼────────────┼────────────────┼──────────────────\n";
  for (std::size_t i = 0; i < steps.size(); ++i) {
    Car_step s = steps[i];
    std::cout << std::left << std::setw(5) << s.step << "| "
              << std::setw(12) << action_label(s.action) << "| "
              << std::setw(15) << format_position(s.x, s.y) << "| "
              << s.result << '\n';
  }
}

void print_journey_summary(std::vector<Car_step> steps, Car car, int target_x,
                           int target_y) {
  std::cout << "\n═════════════════════════════════════════════════\n";
  std::cout << "Journey Complete!\n";
  std::cout << "Total steps: " << steps.size() << '\n';
  std::cout << "Final position: " << format_position(car.x, car.y) << '\n';
  std::cout << "Target location: " << format_position(target_x, target_y)
            << '\n';
  int hit_step = 0;
  if (!steps.empty()) {
    hit_step = steps.back().step;
  }
  std::cout << "Target hit at step: " << hit_step << '\n';
  std::cout << "═════════════════════════════════════════════════\n";
}

int main() {
  int target_x = 0;
  int target_y = 0;
  std::string entered_action = "";
  Action action;
  Car car{};
  std::vector<Car_step> car_steps{};

  std::cout << "Enter coordinates, x and y: ";
  std::cin >> target_x >> target_y;

  while (!(check_coordinate_is_valid(target_x) &&
           check_coordinate_is_valid(target_y))) {
    std::cout << "Enter valid coordinates, x and y: ";
    std::cin >> target_x >> target_y;
  }
  
  while (!check_if_target_achieved(car, target_x, target_y)) {
    std::cout << "Enter car action: ";
    std::getline(std::cin >> std::ws, entered_action);

    action = parse_action(entered_action);
    if (action == Action::unknown) {
      std::cout << "Unknown command, try again.\n";
      continue;
    }
    
    car = drive(car, action);

    Car_step step{};
    step.step = static_cast<int>(car_steps.size());
    step.action = action;
    step.x = car.x;
    step.y = car.y;
    if (check_if_target_achieved(car, target_x, target_y)) {
      step.result = make_target_result(target_x, target_y);
    } else {
      step.result = "no target";
    }
    car_steps.push_back(step);
  }

  std::cout << '\n';
  std::cout << "Target: (" << target_x << ", " << target_y << ")\n\n";
  print_journey_table(car_steps);
  print_journey_summary(car_steps, car, target_x, target_y);

  return 0;
}