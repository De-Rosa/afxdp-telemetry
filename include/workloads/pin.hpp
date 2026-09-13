#ifndef PIN
#define PIN

#include <thread>

namespace workloads {
void pin_to_core(std::thread &thr, int core);
} // namespace workloads

#endif // !PIN
