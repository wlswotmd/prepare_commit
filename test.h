#define DEVICE_NAME "test"
#define DEVICE_PATH "/dev/test"

#define IOCTL_TEST          100
#define IOCTL_DO            _IO(IOCTL_TEST, 0)
#define IOCTL_SET_FUNCTION  _IOW(IOCTL_TEST, 1, unsigned long)
#define IOCTL_SET_ARGUMENT  _IOW(IOCTL_TEST, 2, unsigned long)
#define IOCTL_GET_RESPONSE  _IOR(IOCTL_TEST, 3, unsigned long*)
