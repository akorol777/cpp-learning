#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

enum batteryType { S4, S5, S6 };
enum voltageStatusType { OK, LOW_VOLTAGE };
enum radioProtocol { ELRS, CRSF };
enum fiberProtocol { MAVLink, UDP };
enum connectStatus { SUCCESS, FAILED };
enum ammoTypeEnum { MISSILES, BOMBS, NONE };

const batteryType DEFAULT_RADIO_BATTERY_TYPE = S5;
const double DEFAULT_RADIO_VOLTAGE = 18.2;
const radioProtocol DEFAULT_RADIO_PROTOCOL = ELRS;
const int DEFAULT_RADIO_FREQUENCY = 868;
const std::string DEFAULT_RADIO_BIND = "MyBind01";

const batteryType DEFAULT_FIBER_BATTERY_TYPE = S6;
const double DEFAULT_FIBER_VOLTAGE = 21.1;
const fiberProtocol DEFAULT_FIBER_PROTOCOL = MAVLink;
const std::string DEFAULT_FIBER_IP = "192.168.1.10";
const int DEFAULT_FIBER_PORT = 5760;
const std::string DEFAULT_FIBER_USER = "pilot";
const std::string DEFAULT_FIBER_PASSWORD = "secret123";

std::string batteryTypeToString(batteryType type) {
  switch (type) {
    case S4: return "4S";
    case S5: return "5S";
    case S6: return "6S";
  }
  return "?";
}

std::string radioProtocolToString(radioProtocol protocol) {
  switch (protocol) {
    case ELRS: return "ELRS";
    case CRSF: return "CRSF";
  }
  return "?";
}

std::string fiberProtocolToString(fiberProtocol protocol) {
  switch (protocol) {
    case MAVLink: return "MAVLink";
    case UDP: return "UDP";
  }
  return "?";
}

std::string voltageStatusToString(voltageStatusType status) {
  switch (status) {
    case OK: return "OK";
    case LOW_VOLTAGE: return "LOW_VOLTAGE";
  }
  return "?";
}

std::string ammoTypeToString(ammoTypeEnum type) {
  switch (type) {
    case MISSILES: return "MISSILES";
    case BOMBS:    return "BOMBS";
    case NONE:     return "NONE";
  }
  return "?";
}

class Battery {
  private:
    static constexpr double LOW_VOLTAGE_PER_CELL = 3.5;
    
    batteryType type;
    double voltage;
    double lowVoltage;
    voltageStatusType voltageStatus;
    
    void setType(batteryType type) {
      this->type = type;
    }
    
    void setVoltage(double voltage) {
      this->voltage = voltage;
    }
    
    void calcLowVoltage() {
      switch(this->type) {
        case S4:
          this->lowVoltage = 4*LOW_VOLTAGE_PER_CELL;
          break;
        case S5:
          this->lowVoltage = 5*LOW_VOLTAGE_PER_CELL;
          break;
        case S6:
          this->lowVoltage = 6*LOW_VOLTAGE_PER_CELL;
          break;
      }
    }
    
    void updateVoltageStatus() {
      if (voltage > lowVoltage) {
        this->voltageStatus = OK;
      } else {
        this->voltageStatus = LOW_VOLTAGE;
      }
    }
    
  public:
    Battery (batteryType type, double voltage) {
      setType(type);
      setVoltage(voltage);
      calcLowVoltage();
      updateVoltageStatus();
    }
    
    batteryType getType () const {
      return this->type;
    }
    
    double getVoltage () const {
      return this->voltage;
    }
    
    voltageStatusType getVoltageStatus () const {
      return this->voltageStatus;
    }
    
  ~Battery() {
    std::cout << "Battery removed. Remaining voltage: "
      << std::fixed << std::setprecision(2) << voltage << " V\n";
  }
};

class RadioRx {
  private: 
    radioProtocol protocol;
    int frequency;
    std::string bindPhrase;
    
    void setProtocol (radioProtocol protocol) {
      this->protocol = protocol;
    }
    
    void setFrequency (int frequency) {
      this->frequency = frequency;
    }
    
    void setBindPhrase (std::string bindPhrase) {
      this->bindPhrase = bindPhrase;
    }
    
  public:
    RadioRx(radioProtocol protocol, int frequency, std::string bindPhrase) {
      setProtocol(protocol);
      setFrequency(frequency);
      setBindPhrase(bindPhrase);
    }
    
    radioProtocol getProtocol () const {
      return this->protocol;
    }
    
    int getFrequency () const {
      return this->frequency;
    }
    
    std::string getBindPhrase () const {
      return this->bindPhrase;
    }
};

class FiberRx {
  private: 
    fiberProtocol protocol;
    std::string ip;
    int port;
    std::string username;
    std::string password;
    
    void setProtocol (fiberProtocol protocol) {
      this->protocol = protocol;
    }
    
    void setIp (std::string ip) {
      this->ip = ip;
    }
    
    void setPort (int port) {
      this->port = port;
    }
    
    void setUsername (std::string username) {
      this->username = username;
    }
    
    void setPassword (std::string password) {
      this->password = password;
    }
    
  public:
    FiberRx(fiberProtocol protocol, std::string ip, int port, std::string username, std::string password) {
      setProtocol(protocol);
      setIp(ip);
      setPort(port);
      setUsername(username);
      setPassword(password);
    }
    
    fiberProtocol getProtocol () const {
      return this->protocol;
    }
    
    std::string getIp () const {
      return this->ip;
    }
    
    std::string getUsername () const {
      return this->username;
    }
    
    std::string getPassword () const {
      return this->password;
    }
    
    int getPort () const {
      return this->port;
    }
};

class Drone {
  private:
    std::string firmwareVersion = "v1.0";
    std::string telemetryData   = "No telemetry";
  protected:
    Battery battery;

  public:
    Drone()
      : battery(DEFAULT_RADIO_BATTERY_TYPE, DEFAULT_RADIO_VOLTAGE) {
        std::cout << "Drone created." << std::endl;
      }
    Drone(batteryType type, double voltage)
      : battery(type, voltage) {
        std::cout << "Drone created." << std::endl;
      }  
      
    virtual void status() const {
      std::cout << "Battery:  " << batteryTypeToString(battery.getType())
                << " " << std::fixed << std::setprecision(1) << battery.getVoltage() << "V"
                << " | " << voltageStatusToString(battery.getVoltageStatus()) << "\n";
      std::cout << "Firmware: " << firmwareVersion << "\n";
      std::cout << "Telemetry: " << telemetryData << "\n";
    }

    void arm() {
      std::cout << "Drone armed." << std::endl;
    }

    void disarm() {
      std::cout << "Drone disarmed." << std::endl;
    }

    virtual ~Drone() {
      std::cout << "Drone destroyed." << std::endl;
    }
};

class RadioDrone : public Drone {
  private:
    RadioRx radioRx;

  public:
    RadioDrone()
      : Drone(), radioRx(DEFAULT_RADIO_PROTOCOL, DEFAULT_RADIO_FREQUENCY, DEFAULT_RADIO_BIND) {
        std::cout << "RadioDrone created." << std::endl;
      }
    RadioDrone(batteryType type, double voltage, radioProtocol protocol, int frequency, std::string bindPhrase)
      : Drone(type, voltage), radioRx(protocol, frequency, bindPhrase) {
        std::cout << "RadioDrone created." << std::endl;
      }  
        
    connectStatus connect(int frequency, std::string bindphrase) {
      int droneFrequency = radioRx.getFrequency();
      std::string droneBindPhrase = radioRx.getBindPhrase();
      bool success = droneFrequency == frequency && droneBindPhrase == bindphrase;

      std::cout << "Connecting RadioDrone... "
                << (success ? "SUCCESS" : "FAILED") << "\n";

      std::cout << "  Frequency:   " << frequency << "\n";
      std::cout << "  Bindphrase:  " << bindphrase << "\n";
      std::cout << "  Voltage:     "
                << (battery.getVoltageStatus() == OK ? "OK" : "LOW_VOLTAGE") << "\n";

      return success ? SUCCESS : FAILED;
    }
    
    void status() const override {
      Drone::status();
      std::cout << "Rx:       " << radioProtocolToString(radioRx.getProtocol()) << " | "
                << radioRx.getFrequency() << " MHz | Bind: " << radioRx.getBindPhrase() << "\n";
    }

    ~RadioDrone() {
      std::cout << "RadioDrone destroyed." << std::endl;
    }
};

class FiberDrone : public Drone {
  private:
    FiberRx fiberRx;

  public:
    FiberDrone()
      : Drone(), fiberRx(DEFAULT_FIBER_PROTOCOL, DEFAULT_FIBER_IP, DEFAULT_FIBER_PORT, DEFAULT_FIBER_USER, DEFAULT_FIBER_PASSWORD) {
        std::cout << "FiberDrone created." << std::endl;
      }
    FiberDrone(batteryType type, double voltage, fiberProtocol protocol, std::string ip, int port, std::string username, std::string password)
      : Drone(type, voltage), fiberRx(protocol, ip, port, username, password) {
        std::cout << "FiberDrone created." << std::endl;
      }  
        
    connectStatus connect(std::string ip, int port, std::string userName, std::string password) {
      std::string droneIp = fiberRx.getIp();
      std::string droneUserName = fiberRx.getUsername();
      std::string dronePassword = fiberRx.getPassword();
      int dronePort = fiberRx.getPort();

      bool success = droneIp == ip && dronePort == port
                  && droneUserName == userName && dronePassword == password;

      std::cout << "Connecting FiberDrone... "
                << (success ? "SUCCESS" : "FAILED") << "\n";

      std::cout << "  IP:          " << ip << "\n";
      std::cout << "  Port:        " << port << "\n";
      std::cout << "  User:        " << userName << "\n";
      std::cout << "  Password:    " << password << "\n";
      std::cout << "  Voltage:     "
                << (battery.getVoltageStatus() == OK ? "OK" : "LOW_VOLTAGE") << "\n";

      return success ? SUCCESS : FAILED;
    }
    
    void status() const override {
      Drone::status();
      std::cout << "Rx:       " << fiberProtocolToString(fiberRx.getProtocol()) << " | "
                << fiberRx.getIp() << ":" << fiberRx.getPort()
                << " | User: " << fiberRx.getUsername() << "\n";
    }

    ~FiberDrone() {
      std::cout << "FiberDrone destroyed." << std::endl;
    }
};

class CombatDrone : public Drone {
  private:
    ammoTypeEnum ammoType;
    int ammoCount;

  public:
    CombatDrone(batteryType type, double voltage, ammoTypeEnum ammoType, int ammoCount)
      : Drone(type, voltage) {
        this->ammoType = ammoType;
        this->ammoCount = ammoCount;
        std::cout << "CombatDrone armed." << std::endl;
      }

    void strike() {
      if (this->ammoCount > 0) {
        this->ammoCount--;
        std::cout << "Striking target with " << ammoTypeToString(ammoType)
                  << ". Remaining: " << this->ammoCount << "\n";
      } else {
        std::cout << "Out of ammo." << std::endl;
      }
    }

    int getAmmoCount() const {
      return this->ammoCount;
    }

    void status() const override {
      Drone::status();
      std::cout << "Ammo:     " << ammoTypeToString(ammoType)
                << " x" << ammoCount << "\n";
    }

  ~CombatDrone() {
    std::cout << "CombatDrone disarmed." << std::endl;
  }
};

// Diamond inheritance: FPVDrone inherits Drone through both RadioDrone and CombatDrone.
// Result: Drone constructor/destructor called TWICE, two separate Battery objects in memory.
// Fix: virtual inheritance (next lecture).
class FPVDrone: public RadioDrone, public CombatDrone {
  public:
    FPVDrone(batteryType type, double voltage, radioProtocol protocol, int frequency, std::string bindPhrase, ammoTypeEnum ammoType, int ammoCount)
      : RadioDrone(type, voltage, protocol, frequency, bindPhrase),
      CombatDrone(type, voltage, ammoType, ammoCount) {
        std::cout << "FPVDrone ready." << std::endl;
      }
      
  void status() const override {
    RadioDrone::status();
    CombatDrone::status();
  }    
  
  ~FPVDrone () {
    std::cout << "FPVDrone going down." << std::endl;
  }
};

class BomberDrone: public FiberDrone, public CombatDrone {
  public:
    BomberDrone(batteryType type, double voltage, fiberProtocol protocol, ammoTypeEnum ammoType, int ammoCount, std::string ip, int port, std::string username, std::string password)
      : FiberDrone(type, voltage, protocol, ip, port, username, password),
      CombatDrone(type, voltage, ammoType, ammoCount) {
        std::cout << "BomberDrone ready." << std::endl;
      }
      
  void status() const override {
    FiberDrone::status();
    CombatDrone::status();
  }    
  
  ~BomberDrone () {
    std::cout << "BomberDrone going down." << std::endl;
  }
};

class MasterDrone {
  private:
    std::vector<Drone*> registeredDrones;
  
  public:
    MasterDrone() {
      
    };
    
    void registerDrone(Drone* drone) {
      this->registeredDrones.push_back(drone);
    }
    
    const std::vector<Drone*>& getDrones() const {
      return this->registeredDrones;
    };
    
    void statusAll() const {
      for (Drone* d : registeredDrones) {
        d->status();
      }
    }
    
    void disarmAll() {
      for (Drone* d : registeredDrones) {
        delete d;
      }
      registeredDrones.clear();
      std::cout << "MasterDrone: all drones disarmed." << std::endl;
    };
    
  ~MasterDrone(){
    std::cout << "MasterDrone offline." << std::endl;
  };  
};

class Operator {
  private:
    MasterDrone& masterDrone;
  public:
    Operator(MasterDrone& master) : masterDrone(master) {}
    
    void printFleetStatus() const {
      for (Drone* drone : masterDrone.getDrones()) {
        drone->status();
      }
    };
    
    void commandStrike(int droneIndex) const {
      std::cout << "Operator commanding strike on drone [" << droneIndex << "]...\n";
      Drone* base = masterDrone.getDrones()[droneIndex];
      // static_cast is safe ONLY if the real object is a CombatDrone (FPVDrone or BomberDrone).
      // If base points to a plain RadioDrone or FiberDrone — undefined behavior at runtime.
      // dynamic_cast (next lecture) would make this safe by returning nullptr on mismatch.
      CombatDrone* combat = static_cast<CombatDrone*>(base);
      combat->strike();
    };
};

int main() {
  MasterDrone master;
  Operator op(master);

  RadioDrone* fpv1 = new FPVDrone(S5, 18.5, ELRS, 868, "Bind01", MISSILES, 4);
  RadioDrone* fpv2 = new FPVDrone(S5, 18.2, ELRS, 915, "Bind02", MISSILES, 3);
  RadioDrone* fpv3 = new FPVDrone(S4, 14.8, CRSF, 868, "Bind03", MISSILES, 2);

  FiberDrone* bomber1 = new BomberDrone(S6, 22.1, MAVLink, BOMBS, 2, "192.168.1.10", 5760, "pilot", "secret123");
  FiberDrone* bomber2 = new BomberDrone(S6, 21.8, MAVLink, BOMBS, 3, "192.168.1.11", 5760, "pilot", "secret123");
  FiberDrone* bomber3 = new BomberDrone(S6, 22.0, UDP,     BOMBS, 1, "192.168.1.12", 5760, "pilot", "secret123");

  RadioDrone* radio1 = new RadioDrone(S5, 18.5, ELRS, 868, "RadioBind01");
  RadioDrone* radio2 = new RadioDrone(S4, 14.8, CRSF, 915, "RadioBind02");

  FiberDrone* fiber1 = new FiberDrone(S6, 22.1, MAVLink, "192.168.1.20", 5760, "pilot", "secret123");
  FiberDrone* fiber2 = new FiberDrone(S6, 21.5, UDP,     "192.168.1.21", 5760, "pilot", "secret123");

  master.registerDrone(fpv1);
  master.registerDrone(fpv2);
  master.registerDrone(fpv3);
  master.registerDrone(bomber1);
  master.registerDrone(bomber2);
  master.registerDrone(bomber3);
  master.registerDrone(radio1);
  master.registerDrone(radio2);
  master.registerDrone(fiber1);
  master.registerDrone(fiber2);

  std::cout << "\n=== Fleet Status ===\n";
  op.printFleetStatus();

  std::cout << "\n=== Strikes ===\n";
  op.commandStrike(0);
  op.commandStrike(3);
  op.commandStrike(6);

  std::cout << "\n=== Disarm All ===\n";
  master.disarmAll();

  return 0;
}