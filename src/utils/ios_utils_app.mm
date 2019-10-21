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

#include <TargetConditionals.h>

#if TARGET_OS_IPHONE

#include <UIKit/UIApplication.h>
#include "bctoolbox/ios_utils.hh"
#include "bctoolbox/logging.h"

unsigned long IOSUtilsApp::beginBackgroundTask(const char *name, bctbx_background_task_end_callback_t cb, void *data) {
    __block UIBackgroundTaskIdentifier bgid = UIBackgroundTaskInvalid;

    dispatch_block_t block = ^{
        UIApplication *app=[UIApplication sharedApplication];

        @try {
            if (cb==NULL){
                bctbx_error("belle_sip_begin_background_task(): the callback must not be NULL. Application must be aware that the background task needs to be terminated.");
                bgid = UIBackgroundTaskInvalid;
                @throw [NSException new];
            }

            void (^handler)() = ^{
                cb(data);
            };

            if([app respondsToSelector:@selector(beginBackgroundTaskWithName:expirationHandler:)]){
                bgid = [app beginBackgroundTaskWithName:[NSString stringWithUTF8String:name] expirationHandler:handler];
            } else {
                bgid = [app beginBackgroundTaskWithExpirationHandler:handler];
            }

            if (bgid==UIBackgroundTaskInvalid){
                bctbx_error("Could not start background task %s.", name);
                bgid = 0;
                @throw [NSException new];
            }

            // backgroundTimeRemaining is properly set only when running background... but not immediately!
            if (app.applicationState != UIApplicationStateBackground || (app.backgroundTimeRemaining == DBL_MAX)) {
                bctbx_message("Background task %s started. Unknown remaining time since application is not fully in background.", name);
            } else {
                bctbx_message("Background task %s started. Remaining time %.1f secs", name, app.backgroundTimeRemaining);
            }
        }
        @catch (NSException*) {
            // do nothing
        }
    };

    if( [NSThread isMainThread] ) {
        block();
    }
    else {
        dispatch_sync(dispatch_get_main_queue(), block);
    }

    return (unsigned long)bgid;
}

void IOSUtilsApp::endBackgroundTask(unsigned long id) { 
    dispatch_block_t block = ^{
        UIApplication *app=[UIApplication sharedApplication];
        if (id != UIBackgroundTaskInvalid){
            [app endBackgroundTask:(UIBackgroundTaskIdentifier)id];
        }
    };

    if( [NSThread isMainThread] ) {
        block();
    }
    else {
        dispatch_sync(dispatch_get_main_queue(), block);
    }
}

bool IOSUtilsApp::isApplicationStateActive() {
    return ([UIApplication sharedApplication].applicationState == UIApplicationStateActive);
}

extern "C" {
    IOSUtilsApp *create() {
        return new IOSUtilsApp;
    }

    void destroy(IOSUtilsApp* p) {
        delete p;
    }
}

#endif
