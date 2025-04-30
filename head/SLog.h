// 开关
#define LOG_ENABLE

#ifdef LOG_ENABLE
  #include <iostream>
  using namespace std;
#define LOG(msg) \
cout << "\033[34m" << msg << "\033[0m" << endl;
#else
#define LOG(void*)0
#endif
