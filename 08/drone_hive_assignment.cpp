#include <iostream>
#include <vector>
#include <random>
#include <memory>

enum statusType { Operational, DEAD, Disarmed, FAIL };
enum class hiveType { Unknown, Reconnaissance, FPV, Bomber };

struct Point { int x, y; };

bool canSee(Point from, Point target) {
  int dx = from.x - target.x;
  if (dx < 0) {
    dx = -dx;
  }

  int dy = from.y - target.y;
  if (dy < 0) {
    dy = -dy;
  }

  if (dx <= 1 && dy <= 1) {
    return true;
  }

  return false;
}

class Drone {
  private:
    int id;
    statusType status;
    Point position;
    
    void setPoint (Point position) {
      this->position = position;
    }

  public:
    Drone(int id, Point position)
      {
        std::cout << "Drone created." << std::endl;
        this->id = id;
        setStatus(Disarmed);
        setPoint(position);
      }
      
    virtual statusType attack(Point target) = 0;
    
    int getId() {
      return this->id;
    }
    
    statusType getStatus() {
      return this->status;
    }
    
    Point getPosition() {
      return this->position;
    }

    virtual ~Drone() {
      std::cout << "Drone destroyed." << std::endl;
    }
    
    void arm() {
      setStatus(Operational);
      std::cout << "Drone armed." << std::endl;
    }

    void disarm() {
      setStatus(Disarmed);
      std::cout << "Drone disarmed." << std::endl;
    }
    
    void move(Point position) {
      setPoint(position);
      std::cout << "Drone moved." << std::endl;
    }
    
  protected:
    void setStatus (statusType status) {
      this->status = status;
    }
    
    bool isOperational() {
      return this->getStatus() == Operational;
    }
};

class Bomber : public Drone {
  private:
    static constexpr int MaxBombs = 6;
    int bombCount = MaxBombs;
    
    void shot() {
      this->bombCount--;
    }
  
  public:
    Bomber(int id, Point position) : Drone(id, position) {
    
    }  
    
    statusType attack(Point target) override {
      if (!this->isOperational()) {
        return FAIL;
      }
      
      if (this->bombCount > 0) {
        this->shot();
        std::cout << "Bomber " << this->getId()
                  << " attacked target (" << target.x << ", " << target.y
                  << "). Bombs left: " << this->bombCount << std::endl;
        return Operational;
      } else {
        return FAIL;
      }
    }
    
    int getBombsCount () {
      return this->bombCount;
    }
    
    void unloadBombs() {
      this->bombCount = 0;
    }
};

class FPV : public Drone {    
  public:
    FPV(int id, Point position) : Drone(id, position) {
    
    }  
    
    statusType attack(Point target) override {
      if (this->isOperational()) {
        this->setStatus(DEAD);
        std::cout << "FPV " << this->getId()
                  << " attacked target (" << target.x << ", " << target.y << ")" << std::endl;
        return DEAD;
      } else {
        return FAIL;
      }
    }
};

class Reconnaissance : public Drone {    
  public:
    Reconnaissance(int id, Point position) : Drone(id, position) {
    
    }  
    
    statusType attack(Point target) override {
      if (this->isOperational()) {
        this->move(target);
        std::cout << "Reconnaissance " << this->getId()
                  << " moved to target (" << target.x << ", " << target.y << ")" << std::endl;
        return Operational;
      } else {
        return FAIL;
      }
    }
    
    Point reportTarget() {
      static std::mt19937 gen{std::random_device{}()};
      static std::uniform_int_distribution<int> dist(0, 199);
      int x = dist(gen);
      int y = dist(gen);
      return Point{ x, y};
    }
};

class BeeQueen : public Drone {
  private:
    hiveType type = hiveType::Unknown;
    Point home;
    std::vector<std::unique_ptr<Drone>> drones;
    
    Drone* findDrone(int id) {
      for (const std::unique_ptr<Drone>& d : drones) {
        if (d->getId() == id) {
          return d.get();
        }
      }
      return nullptr;
    }
    
  public:
    BeeQueen(int id, Point position) : Drone(id, position) {
      home = position;
    }  
    
    statusType attack(Point target) override {
      if (this->isOperational()) {
        return Operational;
      } else {
        return FAIL;
      }
    }
    
    bool addDrone(std::unique_ptr<Drone> drone) {
      if (!this->isOperational()) {
        std::cout << "Error, should be operational status, not " << this->getStatus() << std::endl;
        return false;
      } else {
        if (type == hiveType::Unknown) {
          if (dynamic_cast<Bomber*>(drone.get()) != nullptr) {
            type = hiveType::Bomber;
          } else if (dynamic_cast<FPV*>(drone.get()) != nullptr) {
            type = hiveType::FPV;
          } else if (dynamic_cast<Reconnaissance*>(drone.get()) != nullptr) {
            type = hiveType::Reconnaissance;
          }
        } else {
          if (type == hiveType::Bomber) {
            if (dynamic_cast<Bomber*>(drone.get()) == nullptr) {
              std::cout << "Error, should be Bomber" << std::endl;
              return false;
            }
          } else if (type == hiveType::FPV) {
            if (dynamic_cast<FPV*>(drone.get()) == nullptr) {
              std::cout << "Error, should be FPV" << std::endl;
              return false;
            }
          } else if (type == hiveType::Reconnaissance) {
            if (dynamic_cast<Reconnaissance*>(drone.get()) == nullptr) {
              std::cout << "Error, should be Reconnaissance" << std::endl;
              return false;
            }
          }
        }
      
      
        for (std::unique_ptr<Drone>& d : drones) {
          if (d->getId() == drone->getId()) {
            std::cout << "Error, dron with id " << drone->getId() <<  " is already added " << std::endl;
            return false;
          }
        }
        
        drones.push_back(std::move(drone));
        return true;
      }
    }
    
    int getDroneCount() {
      if (this->isOperational()) {
        return this->drones.size();
      } else {
        return -1;
      }
    }
    
    hiveType getHiveType() {
      if (this->isOperational()) {
        return this->type;
      } else {
        return hiveType::Unknown;
      }
    }
    
    void reportDrones() {
      if (this->isOperational()) {
        for (const std::unique_ptr<Drone>& d : drones) {
          std::cout << "Dron is: " << d->getId() << ", status: " << d->getStatus() << std::endl;
        }
      } else {
        std::cout << "Error report, should be operational status, not " << this->getStatus() << std::endl;
      }
    }
    
    void arm() {
      Drone::arm();
      home = getPosition();
    }
    
    void returnToStart() {
      if (!this->isOperational()) {
        std::cout << "Error returnToStart, should be operational status, not "
                  << this->getStatus() << std::endl;
        return;
      }

      if (type == hiveType::Bomber) {
        for (const std::unique_ptr<Drone>& d : drones) {
          dynamic_cast<Bomber*>(d.get())->unloadBombs();
        }
      } else if (type == hiveType::FPV) {
        drones.clear();
      } else if (type == hiveType::Reconnaissance) {
      }

      this->move(this->home);
    }
    
    statusType attackDrone(int id, Point target) {
      if (!isOperational()) {
        return FAIL;
      }
      Drone* d = findDrone(id);
      if (d == nullptr) {
        return FAIL;
      }
      return d->attack(target);
    }
    
    bool armDrone(int id) {
      if (!isOperational()) {
        return false;
      }
      Drone* d = findDrone(id);
      if (d == nullptr) {
        return false;
      }
      d->arm();
      return true;
    }
     
    bool disarmDrone(int id) {
      if (!isOperational()) {
        return false;
      }
      Drone* d = findDrone(id);
      if (d == nullptr) {
        return false;
      }
      d->disarm();
      return true;
    }
    
    bool moveDrone(int id, Point pos) {
      if (!isOperational()) {
        return false;
      }
      Drone* d = findDrone(id);
      if (d == nullptr) {
        return false;
      }
      d->move(pos);
      return true;
    }
    
    std::vector<Point> freeHunt(const std::vector<Point>& targets) {
      std::vector<Point> found;

      if (!isOperational() || type != hiveType::Reconnaissance) {
        return found;
      }

      Point queenPos = getPosition();

      for (int x = queenPos.x - 10; x <= queenPos.x + 10; x += 2) {
        for (int y = queenPos.y - 10; y <= queenPos.y + 10; y += 2) {
          Point cell{x, y};

          for (std::unique_ptr<Drone>& d : drones) {
            d->move({x,y});
          }

          for (const Point& t : targets) {
            for (std::unique_ptr<Drone>& d : drones) {
              if (canSee(d->getPosition(), t)) {
                bool already = false;
                for (const Point& f : found) {
                  if (f.x == t.x && f.y == t.y) {
                    already = true;
                    break;
                  }
                }
                if (!already) {
                  found.push_back(t);
                }
              }
            }
          }
        }
      }

      return found;
    }
};

class GS {
  private:
    std::vector<std::unique_ptr<BeeQueen>> beeQueens;
    std::vector<Point> targets;
    
    BeeQueen* findQueen(int id) {
      for (const std::unique_ptr<BeeQueen>& beeQueen : beeQueens) {
        if (beeQueen->getId() == id) {
          return beeQueen.get();
        }
      }
      return nullptr;
    }

  
  public:
    GS()
      {
        std::cout << "GS created." << std::endl;
      }
      
    void addQueen(std::unique_ptr<BeeQueen> beeQueen) {
      this->beeQueens.push_back(std::move(beeQueen));
    }

    void addTarget(Point t) {
      targets.push_back(t);
    }
    
    bool armQueen(int id) {
      BeeQueen* beeQueen = findQueen(id);
      if (beeQueen == nullptr) {
        return false;
      }
      beeQueen->arm();
      return true;
    }
    
    bool addDroneToQueen(int queenId, std::unique_ptr<Drone> drone) {
      BeeQueen* beeQueen = findQueen(queenId);
      if (beeQueen == nullptr) {
        return false;
      }   
      
      return beeQueen->addDrone(std::move(drone));  
    }
    
    statusType attack(int queenId, int droneId, Point target) {
      BeeQueen* beeQueen = findQueen(queenId);
      if (beeQueen == nullptr) {
        return FAIL;
      }  
      return beeQueen->attackDrone(droneId, target);
    }
    
    bool armDrone(int queenId, int droneId) {
      BeeQueen* beeQueen = findQueen(queenId);
      if (beeQueen == nullptr) {
        return false;
      }  
      return beeQueen->armDrone(droneId);
    }
    
    bool moveDrone(int queenId, int droneId, Point target) {
      BeeQueen* beeQueen = findQueen(queenId);
      if (beeQueen == nullptr) {
        return false;
      }  
      return beeQueen->moveDrone(droneId, target);
    }
    
    void reportHive(int queenId) {
      BeeQueen* q = findQueen(queenId);
      if (q == nullptr) {
       return; 
      }

      q->reportDrones();
    }

    std::vector<Point> freeHunt(int queenId) {
      BeeQueen* q = findQueen(queenId);
      if (q == nullptr) {
        return {};
      }
      return q->freeHunt(targets);
    }
};




int main() {
  std::cout << "=== Final simulation ===" << std::endl;

  GS gs;

  gs.addQueen(std::make_unique<BeeQueen>(1, Point{0, 0}));
  gs.addQueen(std::make_unique<BeeQueen>(2, Point{0, 0}));

  gs.armQueen(1);
  gs.armQueen(2);

  gs.addDroneToQueen(1, std::make_unique<Reconnaissance>(10, Point{0, 0}));
  gs.addDroneToQueen(1, std::make_unique<Reconnaissance>(11, Point{0, 0}));
  gs.addDroneToQueen(2, std::make_unique<Bomber>(20, Point{0, 0}));
  gs.addDroneToQueen(2, std::make_unique<Bomber>(21, Point{0, 0}));

  gs.armDrone(1, 10);
  gs.armDrone(1, 11);
  gs.armDrone(2, 20);
  gs.armDrone(2, 21);

  gs.addTarget(Point{4, 4});
  gs.addTarget(Point{100, 100});

  std::cout << "\n--- Free hunt ---" << std::endl;
  std::vector<Point> found = gs.freeHunt(1);

  std::cout << "Found targets: " << found.size() << std::endl;
  for (const Point& p : found) {
    std::cout << "  (" << p.x << ", " << p.y << ")" << std::endl;
  }

  std::cout << "\n--- Attack via Bomber hive ---" << std::endl;
  if (found.empty()) {
    std::cout << "No targets found — no attack." << std::endl;
  } else {
    Point strike = found[0];
    statusType a1 = gs.attack(2, 20, strike);
    statusType a2 = gs.attack(2, 21, strike);
    std::cout << "Bomber 20 result: " << a1 << std::endl;
    std::cout << "Bomber 21 result: " << a2 << std::endl;
  }

  std::cout << "\n--- Reports ---" << std::endl;
  std::cout << "Recon hive:" << std::endl;
  gs.reportHive(1);
  std::cout << "Bomber hive:" << std::endl;
  gs.reportHive(2);

  return 0;
}