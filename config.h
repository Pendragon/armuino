#define STATUS_BOOT         0
#define STATUS_SHUTDOWN     1
#define STATUS_READY        2
#define STATUS_ERROR        3
#define READ_BUFFER_SIZE    64

#define WORD_MOVE   "move"
#define WORD_RESET  "reset"
#define WORD_OPEN   "open"
#define WORD_CLOSE  "close"
#define WORD_HELP   "help"
#define WORD_STATUS "status"

#define ACCEL       2000
#define MAX_SPEED   4000


class Axis {
  public:
    int B;
    int X;
    int Y;
    int Z;  
};
