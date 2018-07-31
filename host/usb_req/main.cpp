#include <glog/logging.h>
#include <iostream>
#include <libusb-1.0/libusb.h>

using namespace std;

int main()
{

    libusb_context* session = nullptr;

    libusb_init(&session);
    if (nullptr == session) {
        LOG(ERROR) << "Failed to libusb_init";
        exit(-1);
    }

    libusb_device_handle* handle = nullptr;
    uint16_t vid = 0x0483;
    uint16_t pid = 0x5740;
    handle = libusb_open_device_with_vid_pid(session, vid, pid);
    if (nullptr == handle) {
        LOG(ERROR) << "Failed to libusb_open_device_with_vid_pid";
        exit(-1);
    }

    libusb_claim_interface(handle, 0);
    libusb_set_interface_alt_setting(handle, 0, 0);
    libusb_set_debug(session, 3);

    uint32_t version = 0;
    int retval = libusb_control_transfer(
        handle,
        LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE,
        0, // Request
        0, // Value
        0, // Index
        (unsigned char*)&version, // Array with payload
        sizeof(uint32_t), // Payload size
        1000); // Timeout

    if (0 > retval) {
        LOG(ERROR) << "Failed to libusb_control_transfer";
    } else {
        LOG(INFO) << "Version = " << version;
    }

    return 0;
}
