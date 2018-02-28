#include "PyroDriver_interface.hpp"

extern "C" {
        PyroDriverHandle newPyroDriver() {
                return new PyroDriver();
        }

        int PyroDriver_activate(PyroDriverHandle v) {
                return v->activate();
        }

        void PyroDriver_setTime(PyroDriverHandle v, int timeMS) {
                v->setTime(timeMS);
        }

        void deletePyroDriver(PyroDriverHandle v) {
                delete  v;
        }
}
