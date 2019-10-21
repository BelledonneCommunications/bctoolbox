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

#ifndef ios_utils_h
#define ios_utils_h

typedef void (*bctbx_background_task_end_callback_t)(void *);

class IOSUtilsApp {
public:
    virtual unsigned long beginBackgroundTask(const char *name, bctbx_background_task_end_callback_t cb, void *data);
    virtual void endBackgroundTask(unsigned long id);
    virtual bool isApplicationStateActive();

    virtual ~IOSUtilsApp() = default;
};

typedef IOSUtilsApp *create_t();
typedef void destroy_t(IOSUtilsApp *);

class IOSUtils {
public:
    unsigned long beginBackgroundTask(const char *name, bctbx_background_task_end_callback_t cb, void *data);
    void endBackgroundTask(unsigned long id);
    bool isApplicationStateActive();

private:
    bool isApp();
    IOSUtilsApp *createIOSUtils(void *handle);
    void destroyIOSUtils(void *handle, IOSUtilsApp *appUtils);
    void *openDynamicLib();
    void closeDynamicLib(void *handle);
    void *loadSymbol(void *handle, const char *symbol);
};

#endif /* ios_utils_h */
