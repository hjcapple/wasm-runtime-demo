//
//  main.cpp
//  WasmTest
//
//  Created by HJC on 2018/9/6.
//  Copyright © 2018年 HJC. All rights reserved.
//

#include <iostream>
#include <string.h>
#include "WasmRuntime.h"

int main(int argc, const char *argv[]) {

    wasm::WasmRuntime runtime;
    float result[4];
    runtime.colorFromHexString("#ff00ff", result);

    return 0;
}
