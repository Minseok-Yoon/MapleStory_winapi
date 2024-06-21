#pragma once

using namespace std;

#include <Windows.h>
#include <wchar.h>
#include <iostream>

#include <utility>
using std::pair;

#include <memory>
using std::unique_ptr;
using std::make_unique;

#include <functional>
using std::function;
using std::bind;

#include <stdexcept>

#include <string>
using std::string;
using std::wstring;

#include <list>
using std::list;

#include <vector>
using std::vector;

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <map>
using std::map;
using std::make_pair;

#include <algorithm>
using std::sort;

#include <math.h>
#include <assert.h>

#pragma comment(lib, "Msimg32.lib")

#include "Define.h"
#include "Struct.h"
#include "func.h"