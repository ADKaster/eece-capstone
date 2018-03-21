#include "PyroDriver_interface.hpp"

static PyroDriver pyro = PyroDriver();

#ifdef __cplusplus
extern "C" {
#endif
//        PyroDriverHandle newPyroDriver() {
//                return new PyroDriver();
//        }
//
//        int PyroDriver_activate(PyroDriverHandle v, int idx) {
//                return v->activate(idx);
//        }

        void PyroDriver_setTime(int pyroIndex) {
            pyro.activate(pyroIndex);

        }
#ifdef __cplusplus
}
#endif
//
//        void deletePyroDriver(PyroDriverHandle v) {
//                delete  v;
//        }
//}
