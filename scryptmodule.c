#include <Python.h>

#include "scrypt.h"

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
static unsigned char getNfactor(char* blockheader) {
    int n,l = 0,s;
    int nChainStartTime = 1307300111;
    unsigned char nTimestamp = *(unsigned int*)(&blockheader[68]);
    unsigned char minNfactor = 10;
    unsigned char maxNfactor = 30;
    unsigned char N;

    if (nTimestamp <= nChainStartTime) {
        return minNfactor;
    }

    s = nTimestamp - nChainStartTime;

    while ((s >> 1) > 3) {
      l += 1;
      s >>= 1;
    }

    s &= 3;

    n = (l * 158 + s * 28 - 2670) / 100;

    if (n < 0) n = 0;

    N = (unsigned char) n;
    //n = N > minNfactor ? N : minNfactor;
    //N = n < maxNfactor ? n : maxNfactor;
   
    return min(max(N, minNfactor),maxNfactor);
}

static PyObject *scrypt_getpowhash(PyObject *self, PyObject *args)
{
    char *output;
    PyObject *value;
    PyStringObject *input;
    unsigned int N;
    if (!PyArg_ParseTuple(args, "S", &input)) 
	return NULL;
    Py_INCREF(input);
    output = PyMem_Malloc(32);
    N = 1 << (getNfactor((char *)PyString_AsString((PyObject*) input)) + 1);

    scrypt_N_1_1_256((char *)PyString_AsString((PyObject*) input), output, N);
    Py_DECREF(input);
    value = Py_BuildValue("s#", output, 32);
    PyMem_Free(output);
    return value;
}

static PyMethodDef ScryptMethods[] = {
    { "getPoWHash", scrypt_getpowhash, METH_VARARGS, "Returns the proof of work hash using scrypt" },
    { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initspd_scrypt(void) {
    (void) Py_InitModule("spd_scrypt", ScryptMethods);
}
