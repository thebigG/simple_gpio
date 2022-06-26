//At the moment these defines assume linux is the target platform.
#include <array>
#ifdef ZEDBOARD
#define BASE_GPIO (906)
#define N_GPIO (118)
#define BOARD "ZEDBOARD"
std::array<int, 4> GPIO{};
#endif
#ifdef PI4_B
#define BOARD "Raspberry Pi 4"
#define BASE_GPIO (0)
#define N_GPIO (40)
std::array<int, 13> GPIO{5,6,16,17,19,20,21,27,22,23,24,25,26};
#endif

