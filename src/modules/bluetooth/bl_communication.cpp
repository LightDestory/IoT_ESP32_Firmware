#include "bl_communication.h"
#include  "../utils/serial_logger.h"
#include "../display/display.h"
#include "../display/images.h"

BL_COM::BL_STATE BL_COM::state = BL_COM::UNSETUP;
int8_t BL_COM::selection_workload_cursor = 0;
int8_t BL_COM::selected_workload = -1;
BluetoothSerial BL_COM::serialBT;
DATA_STRUCTURES::workload BL_COM::sub_menus[BL_COM::IMPLEMENTED_SUBS] = {
        {"Start Master to Slave", BL_COM::MtoS},
        {"Display BL Mac Addr",   BL_COM::displayMacAddr}
};

void BL_COM::MtoS() {
    if (state == READY) {
        serialBT.end();
        serialBT.begin(DEVICE_NAME, true);
        state = LOOPING;
    }
    DISPLAY_ESP::drawCenteredTitleSubtitle("", DEVICE_NAME);
}

void BL_COM::connectionCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    if (event == ESP_SPP_SRV_OPEN_EVT) {
        SERIAL_LOGGER::log("Device Connected");
    } else if (event == ESP_SPP_CLOSE_EVT) {
        SERIAL_LOGGER::log("Device Disconnected");
    }
}

void BL_COM::displayMacAddr() {
    DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::bluetooth, "Your Mac Addr",
                                                serialBT.getBtAddressString());
}

void BL_COM::init() {
    state = READY;
    serialBT.register_callback(connectionCallback);
    serialBT.begin();
}

void BL_COM::looper() {
    if (state == UNSETUP)
        init();
    if (BL_COM::selected_workload == -1) {
        DISPLAY_ESP::requestSelectionFromMenu("BL Menu", sub_menus, &IMPLEMENTED_SUBS, &selection_workload_cursor,
                                              &selected_workload);
    } else {
        sub_menus[BL_COM::selected_workload].callback();
    }
}


/**
#define PIN 26

BluetoothSerial Slave;

//Setup bluetooth
String name1 = "ESP32_Pod_2"; //                  <------- set this to be the name of the other ESP32!!!!!!!!!
bool connect_1;
String name2 = "ESP32_Pod_3"; //                  <------- set this to be the name of the other ESP32!!!!!!!!!
bool connect_2;

//Identification for POD Number
const int pod_ID = 1;

//data to send
int pod;

// data to recieve
int reactiontime = 0;

void loop()
{
  pod = random(1,4);

  if(pod==2)
  {
    connect_1 = Slave.connect("ESP32_Pod_2");

    while(!Slave.connected(5000))
    {
      Serial.println("Failed to connect to POD 2");
    }

    Slave.write(pod);

    //Wait for Slave1
    while(!(Slave.available()))
    {
      //do nothing
    }

    Serial.println("");
    Serial.println("-----------------------------------");
    reactiontime = Slave.read();
    Serial.println(reactiontime);
    Serial.println("Slave 1");
    Serial.println("-----------------------------------");
    Serial.println("");

    connect_1 = Slave.disconnect();
  }

  else if(pod==3)
  {
    connect_2 = Slave.connect("ESP32_Pod_3");

    while(!Slave.connected(5000))
    {
      Serial.println("Failed to connect to POD 3");
    }

    Slave.write(pod);

    //Wait for Slave2
    while(!(Slave.available()))
    {
      //do nothing
    }

    Serial.println("");
    Serial.println("-----------------------------------");
    reactiontime = Slave.read();
    Serial.println(reactiontime);
    Serial.println("Slave 2");
    Serial.println("-----------------------------------");
    Serial.println("");

    connect_2 = Slave.disconnect();
  }

  else if(pod == pod_ID)
  {
    Serial.println("");
    Serial.println("-----------------------------------");
    Serial.println("Master");
    Serial.println("-----------------------------------");
    Serial.println("");
  }

  delay(1000);
}
*/