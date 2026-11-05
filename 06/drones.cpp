#include <iostream>
#include <iomanip>
#include <string>

enum batteryType { S4, S5, S6 };
enum voltageStatusType { OK, LOW_VOLTAGE };
enum radioProtocol { ELRS, CRSF };
enum fiberProtocol { MAVLink, UDP };
enum connectStatus { SUCCESS, FAILED };

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

class RadioDrone {
  private:
    Battery battery;
    RadioRx radioRx;

  public:
    RadioDrone()
      : battery(DEFAULT_RADIO_BATTERY_TYPE, DEFAULT_RADIO_VOLTAGE),
        radioRx(DEFAULT_RADIO_PROTOCOL, DEFAULT_RADIO_FREQUENCY, DEFAULT_RADIO_BIND) {}
    RadioDrone(batteryType type, double voltage, radioProtocol protocol, int frequency, std::string bindPhrase)
      : battery(type, voltage),
        radioRx(protocol, frequency, bindPhrase) {}  
        
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
    
    void status() const {
      std::cout << "=== RadioDrone Status ===\n";
      std::cout << "Type:     RadioDrone\n";
      std::cout << "Battery:  " << batteryTypeToString(battery.getType()) << " | "
                << std::fixed << std::setprecision(1) << battery.getVoltage() << "V | "
                << voltageStatusToString(battery.getVoltageStatus()) << "\n";
      std::cout << "Rx:       " << radioProtocolToString(radioRx.getProtocol()) << " | "
                << radioRx.getFrequency() << " MHz | Bind: " << radioRx.getBindPhrase() << "\n";
    }

    ~RadioDrone() {
      std::cout << "Disarming RadioDrone. Remaining voltage: "
                << std::fixed << std::setprecision(2) << battery.getVoltage() << " V\n";
    }
};

class FiberDrone {
  private:
    Battery battery;
    FiberRx fiberRx;

  public:
    FiberDrone()
      : battery(DEFAULT_FIBER_BATTERY_TYPE, DEFAULT_FIBER_VOLTAGE),
        fiberRx(DEFAULT_FIBER_PROTOCOL, DEFAULT_FIBER_IP, DEFAULT_FIBER_PORT, DEFAULT_FIBER_USER, DEFAULT_FIBER_PASSWORD) {}
    FiberDrone(batteryType type, double voltage, fiberProtocol protocol, std::string ip, int port, std::string username, std::string password)
      : battery(type, voltage),
        fiberRx(protocol, ip, port, username, password) {}  
        
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
    
    void status() const {
      std::cout << "=== FiberDrone Status ===\n";
      std::cout << "Type:     FiberDrone\n";
      std::cout << "Battery:  " << batteryTypeToString(battery.getType()) << " | "
                << std::fixed << std::setprecision(1) << battery.getVoltage() << "V | "
                << voltageStatusToString(battery.getVoltageStatus()) << "\n";
      std::cout << "Rx:       " << fiberProtocolToString(fiberRx.getProtocol()) << " | "
                << fiberRx.getIp() << ":" << fiberRx.getPort()
                << " | User: " << fiberRx.getUsername() << "\n";
    }

    ~FiberDrone() {
      std::cout << "Disarming FiberDrone. Remaining voltage: "
                << std::fixed << std::setprecision(2) << battery.getVoltage() << " V\n";
    }
};



void printDroneInfo(const RadioDrone& drone) {
  drone.status();
}

int main() {
  {
    RadioDrone radioDrone;
    FiberDrone fiberDrone;

    radioDrone.status();
    fiberDrone.status();

    printDroneInfo(radioDrone);

    radioDrone.connect(DEFAULT_RADIO_FREQUENCY, DEFAULT_RADIO_BIND);
    radioDrone.connect(DEFAULT_RADIO_FREQUENCY, "WrongBind");

    fiberDrone.connect(DEFAULT_FIBER_IP, DEFAULT_FIBER_PORT, DEFAULT_FIBER_USER, DEFAULT_FIBER_PASSWORD);
    fiberDrone.connect(DEFAULT_FIBER_IP, DEFAULT_FIBER_PORT, DEFAULT_FIBER_USER, "wrong");

    radioDrone.status();
    fiberDrone.status();
  }

  return 0;
}