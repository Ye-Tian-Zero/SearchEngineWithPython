#ifndef EZ_JIEBA_H
#define EZ_JIEBA_H
#include<Python.h>
#include<string>
#include<vector>
using std::string;
using std::vector;
using pyobj = PyObject;

class EZ_err{
public:
	EZ_err(const string& err):err_info(err){}
	const string& getErr() const{
		return err_info;
	}
private:
	string err_info;
};

class jieba{
public:
	jieba(){
		Py_Initialize();
		if (!Py_IsInitialized())
			throw EZ_err("Initialized error");

		PyRun_SimpleString("import sys");
		PyRun_SimpleString("print 'come in python'");
		PyRun_SimpleString("sys.path.append('./')");


		pName = PyString_FromString("split");
		pModule = PyImport_Import(pName);

		if (!pModule)
			throw EZ_err("can not find split.py");

		pDct = PyModule_GetDict(pModule);
		
		if (!pDct)
		{
			throw EZ_err("pDct error");
		}

		pFunc = PyDict_GetItemString(pDct, "split");
		if (!pFunc || !PyCallable_Check(pFunc))
			throw EZ_err("pFunc error");

		pArgs = PyTuple_New(1);
	}

	void cut(const string& cstr, vector<string>& svec)
	{
		svec.clear();

		PyTuple_SetItem(pArgs, 0/*Index?*/, Py_BuildValue("s", cstr.c_str()));
		pyobj* pValue = NULL;
		pValue = PyObject_CallObject(pFunc, pArgs);
		
		size_t sz = PyList_Size(pValue);

		for (int i(0); i != sz; ++i)
		{
			pyobj *ret = PyList_GetItem(pValue, i);
			string s = PyString_AsString(ret);
			svec.push_back(s);
		}

	}

private:
	pyobj *pName, *pModule, *pDct, *pFunc, *pArgs;
};
#endif