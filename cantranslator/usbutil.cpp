#include "usbutil.h"
#include "buffers.h"
#include "log.h"

void sendMessage(CanUsbDevice* usbDevice, uint8_t* message, int messageSize) {
    for(int i = 0; i < messageSize; i++) {
        if(!QUEUE_PUSH(uint8_t, &usbDevice->sendQueue, (uint8_t)message[i])) {
            debug("Dropped incoming CAN message -- send queue full");
            return;
        }
    }
    QUEUE_PUSH(uint8_t, &usbDevice->sendQueue, (uint8_t)'\n');
}
