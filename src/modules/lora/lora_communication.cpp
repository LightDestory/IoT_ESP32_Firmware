#include "lora_communication.h"
#include "../utils/serial_logger/serial_logger.h"

GLOBALS::SUB_ROUTINE_STATE LORA::state = GLOBALS::UN_SETUP;

void LORA::init() {
    LoRa.setPins(18, 23, 26);
    while (!LoRa.begin(866E6)) {
        Serial.println(".");
        delay(500);
    }
    LoRa.setSyncWord(0xCC);
    SERIAL_LOGGER::log("LoRa init succeeded.");
    state = GLOBALS::READY;
}

void LORA::looper() {
    if (state == GLOBALS::UN_SETUP)
        init();
    if (state == GLOBALS::FAILED_INIT or state == GLOBALS::STATIC)
        return;
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // received a packet
        Serial.print("Received packet '");
        // read packet
        while (LoRa.available()) {
            String LoRaData = LoRa.readString();
            Serial.print(LoRaData);
        }
        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
    }
}