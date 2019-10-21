/*
 * Copyright (c) 2010-2019 Belledonne Communications SARL.
 *
 * This file is part of Liblinphone.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef __APPLE__

#import <Foundation/Foundation.h>
#include <dlfcn.h>
#include <exception>

#include "bctoolbox/ios_utils.hh"
#include "bctoolbox/logging.h"

using namespace std;

struct DylibException : public exception {
    virtual void printError() = 0;
};

struct DlopenException : public DylibException {
    void printError() {
        bctbx_error("bctoolbox error dlopen : %s\n", dlerror());
    }
};

struct DlsymException : public DylibException {
    void printError() {
        bctbx_error("bctoolbox error dlsym : %s\n", dlerror());
    }
};


bool IOSUtils::isApp() {
    return [[[NSBundle mainBundle] bundlePath] hasSuffix:@".app"];
}

IOSUtilsApp *IOSUtils::createIOSUtils(void *handle) {
    create_t *createUtils = (create_t *) loadSymbol(handle, "create");
    IOSUtilsApp *appUtils = createUtils();
    return appUtils;
}

void IOSUtils::destroyIOSUtils(void *handle, IOSUtilsApp *appUtils) {
    destroy_t* destroyUtils = (destroy_t *) loadSymbol(handle, "destroy");
    destroyUtils(appUtils);
}

void *IOSUtils::openDynamicLib() {
    NSString *frameworkPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingString: @"/Frameworks/bctoolbox-ios.framework/bctoolbox-ios"];
    const char *frameworkChar = [frameworkPath cStringUsingEncoding:[NSString defaultCStringEncoding]];

    void *handle = dlopen(frameworkChar, RTLD_LAZY);
    if (!handle) {
        throw DlopenException();
    }

    // reset errors
    dlerror();
    return handle;
}

void IOSUtils::closeDynamicLib(void *handle) {
    dlclose(handle);
}

void *IOSUtils::loadSymbol(void *handle, const char *symbol) {
    void *loadedSymbol = dlsym(handle, symbol);
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        closeDynamicLib(handle);
        throw DlsymException();
    }
    
    return loadedSymbol;
}

unsigned long IOSUtils::beginBackgroundTask(const char *name, bctbx_background_task_end_callback_t cb, void *data) {
    unsigned long returnValue = 0;
    if (!isApp()) {
        return returnValue;
    }
    
    try {
        void *handle = openDynamicLib();
        IOSUtilsApp *utilsApp = createIOSUtils(handle);
        returnValue = utilsApp->beginBackgroundTask(name, cb, data);
        destroyIOSUtils(handle, utilsApp);
        closeDynamicLib(handle);
    } catch (DylibException &e) {
        e.printError();
    }
    
    return returnValue;
}

void IOSUtils::endBackgroundTask(unsigned long id) {
    if (!isApp()) {
        return;
    }
    
    try {
        void *handle = openDynamicLib();
        IOSUtilsApp *utilsApp = createIOSUtils(handle);
        utilsApp->endBackgroundTask(id);
        destroyIOSUtils(handle, utilsApp);
        closeDynamicLib(handle);
    } catch (DylibException &e) {
        e.printError();
    }
    
}

bool IOSUtils::isApplicationStateActive() {
    bool returnValue = false;
    if (!isApp()) {
        return returnValue;
    }
    
    try {
        void *handle = openDynamicLib();
        IOSUtilsApp *utilsApp = createIOSUtils(handle);
        returnValue = utilsApp->isApplicationStateActive();
        destroyIOSUtils(handle, utilsApp);
        closeDynamicLib(handle);
    } catch (DylibException &e) {
        e.printError();
    }

    return returnValue;
}

#endif
