

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914B"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a7"

int scanTime = 5; //In seconds
BLEScan* pBLEScan;
const char* c;
double dis;

double getdis(double r){
    double val;
    double dis;
    val=(-70-(r))/20;
    dis=pow(10, val);
    return dis;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      
    {
      Serial.printf("Device: \nTX power => %d\nRSSI => %d\n", advertisedDevice.getTXPower(), advertisedDevice.getRSSI());
      Serial.printf("Name: %s\n", advertisedDevice.getName().c_str());
      c=advertisedDevice.getName().c_str();
      std::string str(c);
      double rssi = advertisedDevice.getRSSI();

      dis=getdis(rssi);
      
      
      if(str[0]=='E' && str[1]=='S' && str[2]=='P' && dis<=1.5){
        digitalWrite(2, HIGH);
        delay(1000);
      } 
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");
  pinMode(2, OUTPUT);
  BLEDevice::init("ESP32");
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
 
 

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2, LOW);
   pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  BLEScanResults foundDevices = pBLEScan->start(1);
  
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  
}
