#include <iostream>
#include <string>

const int max_speed = 4;

int get_break_way(int speed) {
  int way = 0;
  int current_speed = speed;

  while (current_speed >= 0) {
    way += current_speed;
    current_speed--;
  }

  return way;
}

void run_car(int dist) {
  int way_past = 0;
  int speed = 0;
  int total_steps = 0;

  std::cout << "Step | Action | Length | Total Distance\n";

  for (int i = 0; way_past < dist || speed > 0; i += 1) {
    const int speed_before = speed;
    const int way_remain = dist - way_past;
    std::string action_label;

    if (speed < max_speed && get_break_way(speed + 1) <= way_remain) {
      speed++;
      if (speed_before == 0 && way_past == 0) {
        action_label = "start";
      } else {
        action_label = "accel";
      }
    } else {
      if (get_break_way(speed) < way_remain) {
        action_label = "const_speed";
      } else if (get_break_way(speed) == way_remain && way_remain == speed) {
        action_label = "const_speed";
      } else {
        speed--;
        action_label = "brake";
      }
    }

    const int length = speed;
    way_past += speed;
    total_steps++;

    const int step_number = i + 1;
    std::cout << step_number << " | " << action_label << " | " << length << " | "
              << way_past << std::endl;
  }

  const bool ok = (way_past == dist) && (speed == 0);
  const std::string status = ok ? "SUCCESS" : "FAILED";

  std::cout << "\nFINAL RESULTS:\n";
  std::cout << "  Total steps: " << total_steps << "\n";
  std::cout << "  Distance traveled: " << way_past << " units\n";
  std::cout << "  Target distance: " << dist << " units\n";
  std::cout << "  Status: " << status << "\n";
}

int main() {
  int distance = 0;

  std::cin >> distance;

  run_car(distance);

  return 0;
}