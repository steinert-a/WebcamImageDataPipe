// standard C++ library
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <cstdarg>
#include <cstdint>
#include <chrono>
#include <ctime>

#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>

#include <codecvt>
#include <clocale>
#include <cctype>
#include <locale>

#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#include <thread>
#include <future>
#include <mutex>
#include <atomic>
#include <memory>

// microsoft
#include <tchar.h>
#include "windows.h"

// core
#include "TypeCore.h"

// Qt
#include <QtWidgets>

// Open CV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


// defines
#define UNDEF			(-1)

#define DELETE_POINTER(p) {if(p){delete (p);(p)=NULL;}}
#define DELETE_ARRAY(p) {if(p){delete[] (p);(p)=NULL;}}

#define RELEASE_POINTER(p) {if(p){(p)->Release();(p)=NULL;}}

#define FOR_EACH(it,stl) for(auto it = ((stl).begin()); it != ((stl).end()); it++)
#define FOR_EACH_REVERSE(it,stl) for(auto it = ((stl).rbegin()); it != ((stl).rend()); it++)
